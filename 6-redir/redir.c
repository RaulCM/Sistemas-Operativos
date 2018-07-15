//Raul Cano Montero
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

int nprocesos = 0;

char *
tokenize(char *string)
{
	char *token;
	char *tokens[1024];
	int n = 0;

	token = strtok(string, ".");
	while (token != NULL) {
		tokens[n] = token;
		n++;
		token = strtok(NULL, ".");
	}
	if ((n > 1) && (strcmp(tokens[n - 1], "c") == 0)) {
		return tokens[n - 2];
	} else {
		return NULL;
	}
}

void
compile(char *exe, char *path, char *word, char *filename)
{
	int sts;
	char *cflags;
	int p[2];
	int pidgcc;
	int pidgrep;

	if (pipe(p) < 0){
		fprintf(stderr, "error: fork: %s\n", strerror(errno));
		exit(1);
	}
	pidgcc = fork();
	switch(pidgcc){
		case -1:
			fprintf(stderr, "error: fork: %s\n", strerror(errno));
			exit(1);
		case 0:
			dup2(p[1],2);
			close(p[0]);
			close(p[1]);
			cflags = getenv("CFLAGS");
			if(cflags){
				execl("/usr/bin/gcc", "gcc", cflags, "-o", exe, path, NULL);
			} else {
				execl("/usr/bin/gcc", "gcc", "-o", exe, path, NULL);
			}
			fprintf(stderr, "error: execl: gcc: %s\n", strerror(errno));
			exit(1);
	}
	pidgrep = fork();
	switch(pidgrep){
		case -1:
			fprintf(stderr, "error: fork: %s\n", strerror(errno));
			exit(1);
		case 0:
			dup2(p[0],0);
			close(p[0]);
			close(p[1]);
			execl("/bin/grep", "grep", word, NULL);
			fprintf(stderr, "error: execl: grep: %s\n", strerror(errno));
			exit(1);
	}
	close(p[0]);
	close(p[1]);
	if(waitpid(pidgcc, &sts, 0)<0){
		fprintf(stderr, "error: waitpid: %s\n", strerror(errno));
		exit(1);
	}
	if(sts == 0)
		printf("%s: compila\n", filename);
	else
		printf("%s: no compila\n", filename);
	if(waitpid(pidgrep, &sts, 0)<0){
		fprintf(stderr, "error: waitpid: %s\n", strerror(errno));
		exit(1);
	}
}

void
createprocesses(char *filename, char *path, char *fullpath, char *word, struct dirent *entry)
{
	char *tokens[1024];
	char exepath[1024];
	char *exe;

	exe = tokenize(filename);
	if(exe != NULL){
		nprocesos++;
		switch(fork()){
			case -1:
				fprintf(stderr, "error: fork: %s\n", strerror(errno));
				exit(1);
			case 0:
				sprintf(exepath, "%s/%s", path, tokens[0]);
				compile(exepath, fullpath, word, entry->d_name);
				exit(0);
		}
	}
}

void
readdirectory(char *path, char *word)
{
	DIR *d;
	char filename[1024];
	struct dirent *entry;
	struct stat st;
	char fullpath[1024];
	int i;

	d = opendir(path);
	if (d == NULL) {
		fprintf(stderr, "error: opendir: %s: %s\n", path, strerror(errno));
		exit(1);
	}
	while((entry = readdir(d))!=NULL){
		sprintf(fullpath, "%s/%s", path, entry->d_name);
		if (stat(fullpath, &st) < 0) {
			fprintf(stderr, "error: stat: %s: %s\n", fullpath, strerror(errno));
			exit(1);
		}
		if(entry->d_name[0] != '.'){
			if((st.st_mode & S_IFMT) == S_IFREG){
				strcpy(filename, entry->d_name);
				createprocesses(filename, path, fullpath, word, entry);
			}
		}
	}
	closedir(d);
	for(i = 0; i < nprocesos; i++){
		if(wait(NULL) < 0){
			fprintf(stderr, "error: wait: %s\n", strerror(errno));
			exit(1);
		}
	}
}

int
main(int argc, char *argv[])
{
	char directory[1024];
	char word[128];

	if ((argc > 3) || (argc == 1) ){
		fprintf(stderr, "%s: Usage:\t%s [filter word]\n\t\t%s [directory] [filter word]\n", argv[0], argv[0], argv[0]);
		exit(1);
	}
	if (argc == 2) {
		strcpy(directory, ".");
		strcpy(word, argv[1]);
	} else {
		strcpy(directory, argv[1]);
		strcpy(word, argv[2]);
	}
	readdirectory(directory, word);
	exit(0);
}
