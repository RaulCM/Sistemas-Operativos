// Raul Cano Montero
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <unistd.h>

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
compilar(char *exename, char *path)
{
	int sts;
	char *cflags;

	switch (fork()) {
	case -1:
		fprintf(stderr, "error: fork: %s\n", strerror(errno));
		exit(1);
	case 0:
		cflags = getenv("CFLAGS");
		if (cflags) {
			execl("/usr/bin/gcc", "gcc", cflags, "-o", exename, path, NULL);
		} else {
			execl("/usr/bin/gcc", "gcc", "-o", exename, path, NULL);
		}
		fprintf(stderr, "error: execl: gcc: %s\n", strerror(errno));
		exit(1);
	default:
		wait(&sts);
		if (sts == 0)
			printf("%s.c: compila\n", exename);
		else
			printf("%s.c: no compila\n", exename);
	}
}

void
createprocesses(char *filename, char *path, char *fullpath)
{
	char exepath[1024];
	char *exe;

	exe = tokenize(filename);
	if(exe != NULL){
		nprocesos++;
		switch (fork()) {
		case -1:
			fprintf(stderr, "error: fork: %s\n", strerror(errno));
			exit(1);
		case 0:
			sprintf(exepath, "%s/%s", path, exe);
			compilar(exepath, fullpath);
			exit(0);
		}
	}
}

void
readdirectory(char *path)
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
	while ((entry = readdir(d)) != NULL) {
		sprintf(fullpath, "%s/%s", path, entry->d_name);
		if (stat(fullpath, &st) < 0) {
			fprintf(stderr, "error: stat: %s: %s\n", fullpath, strerror(errno));
			exit(1);
		}
		if (entry->d_name[0] != '.') {
			if ((st.st_mode & S_IFMT) == S_IFREG) {
				strcpy(filename, entry->d_name);
				createprocesses(filename, path, fullpath);
			}
		}
	}
	closedir(d);
	for (i = 0; i < nprocesos; i++) {
		if (wait(NULL) < 0) {
			fprintf(stderr, "error: wait: %s\n", strerror(errno));
			exit(1);
		}
	}
}

int
main(int argc, char *argv[])
{
	char directory[1024];

	if (argc > 2) {
		fprintf(stderr, "%s: Usage:\t%s\n \t\t%s [directorio]\n", argv[0], argv[0], argv[0]);
		exit(1);
	}
	if (argc == 1) {
		strcpy(directory, ".");
	} else {
		strcpy(directory, argv[1]);
	}
	readdirectory(directory);
	exit(0);
}
