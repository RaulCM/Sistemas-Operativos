// Raul Cano Montero
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void
upper(char *string)
{
	int i;
	for (i = 0; i < strlen(string); i++) {
		string[i] = toupper(string[i]);
	}
}

int
makepath(char *buf, int len)
{
	char *home;
	char *user;
	pid_t pid;
	home = getenv("HOME");
	user = getenv("USER");
	pid = getpid();

	if ((home == NULL) || (user == NULL) || (pid < 0)) {
		return -1;
	}
	upper(user);
	sprintf(buf, "%s/Downloads/%s.%d", home, user, pid);
	return strlen(buf);
}

int
main(int argc, char *argv[])
{
	int pathlen = 1024;
	char path[pathlen];
	if (argc != 1) {
		fprintf(stderr, "%s: Usage: \t%s\n", argv[0], argv[0]);
		exit(1);
	}
	if (makepath(path, pathlen) < 0) {
		fprintf(stderr, "%s: Error makepath\n", argv[0]);
		exit(1);
	}
	printf("%s\n", path);
	exit(0);
}
