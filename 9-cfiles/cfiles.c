//Raul Cano Montero
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/file.h>
#include <pthread.h>

char word[32];

void
create_cnt_file(char filepath[])
{
	int fd;

	fd = creat(filepath, 0644);
	if(fd < 0) {
		err(1, "creat");
	}
}

int
cnt_filepath(char filepath[])
{
	char *home;
	int fd;

	home = getenv("HOME");
	if (home == NULL)
		err(1, "getenv");
	sprintf(filepath, "%s/cfiles.cnt", home);
	if (access(filepath, F_OK) == -1) {
		create_cnt_file(filepath);
	}
	fd = open(filepath, O_RDWR);
	if(fd < 0) {
		err(1, "open");
	}
	return fd;
}

void
write_file(void)
{
	int fd;
	char filepath[1024];
	char buffer[128];
	int n;
	int nb;

	fd = cnt_filepath(filepath);
	if(flock(fd, LOCK_EX) != 0){
		err(1, "lock");
	}
	n = read(fd, buffer, sizeof(buffer) - 1);
	if(n < 0){
		err(1, "read");
	}
	buffer[n] = 0;
	nb = atoi(buffer);
	snprintf(buffer, sizeof buffer, "%d", nb + 1);
	lseek(fd, 0, 0);
	if(write(fd, buffer, strlen(buffer)) != strlen(buffer)) {
		err(1, "write");
	}
	if(flock(fd, LOCK_UN) != 0){
		err(1, "lock");
	}
	close(fd);
}

int
tok(char *string, char *tokens[])
{
	char *aux;
	char *token;
	int count = 0;

	token = strtok_r(string, " \n", &aux);
	while(token != NULL){
		tokens[count] = token;
		count++;
		string = aux;
		token = strtok_r(string, " \n", &aux);
	}
	return count;
}

void *
function (void *a)
{
	char *filename = (char*) a;
	FILE *f;
	char buffer[1024];
	int nr;
	char *tokens[1024];
	int n, i, found;

	found = 0;
	f = fopen(filename, "r");
	if (f == NULL) {
		pthread_exit(NULL);
	}
	while(1) {
		nr=fread(buffer, sizeof(char), sizeof buffer, f);
		if (nr == 0){
			if (ferror(f)) {
				pthread_exit(NULL);
			}
			break;
		}
		n=tok(buffer, tokens);
		for (i = 0; i < n; i++) {
			printf("%s\n", tokens[i]);
			if ((strcmp(tokens[i], word)) == 0) {
				found = 1;
			}
		}
	}
	fclose(f);
	if (found) {
		write_file();
	}
	pthread_exit(NULL);
}

void
create_threads(int argc, char *argv[])
{
	pthread_t t[256];
	int i;
	int n_threads;

	strcpy(word, argv[1]);
	n_threads = argc - 2;
	for(i = 0; i < n_threads; i++) {
		pthread_create(&t[i], NULL, function, argv[i + 2]);
	}
	for(i = 0; i < n_threads; i++)
		pthread_join(t[i], NULL);
}

int
main(int argc, char *argv[])
{
	if (argc < 3) {
		fprintf(stderr, "%s: Usage: %s [word] [file 1] [file 2]...[file n]\n", argv[0], argv[0]);
		exit(1);
	}
	create_threads(argc, argv);
	exit(0);
}
