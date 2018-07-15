// Raul Cano Montero
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

typedef struct CmdType CmdType;
struct CmdType {
	char path[1024];
	char *argv[64];
};
CmdType cmds[32];
int ncmds, naux, background, rediroutput, redirinput, stdinput, redirvar;
int p[64][2], pids[128], pipestdinput[2], piperedirvar[2];
char fileinput[1024], fileoutput[1024], varname[32], var[1024];
char *tokens[32];
int nprcs = 0;
int npipes = 0;

void
resetvars()
{
	background = 0;
	rediroutput = 0;
	redirinput = 0;
	stdinput = 0;
	redirvar = 0;
}

void
create_pipes(void)
{
	int i;
	for (i = 0; i < ncmds - 1; i++) {
		pipe(p[i]);
	}
}

void
close_pipes(void)
{
	int i;
	for (i = 0; i < ncmds - 1; i++) {
		close(p[i][0]);
		close(p[i][1]);
	}
}

void
redin(int fd)
{
	if (fd < 0) {
		fprintf(stderr, "error: open: %s\n", strerror(errno));
		exit(1);
	}
	dup2(fd, 0);
	close(fd);
}

void
forksuccess(int i)
{
	int fd;

	if ((i == 0) && (stdinput)) {
		dup2(pipestdinput[0], 0);
	}
	if ((i == 0) && (redirinput)) {
		fd = open(fileinput, O_RDONLY);
		redin(fd);
	}
	if ((i == 0) && (background) && (!redirinput)) {
		fd = open("/dev/null", O_RDONLY);
		redin(fd);
	}
	if ((i == (ncmds - 1)) && (rediroutput)) {
		fd = creat(fileoutput, 0644);
		if (fd < 0) {
			fprintf(stderr, "error: creat: %s\n", strerror(errno));
			exit(1);
		}
		dup2(fd, 1);
		close(fd);
	}
	if ((i == (ncmds - 1)) && (redirvar)) {
		dup2(piperedirvar[1], 1);
		close(piperedirvar[0]);
		close(piperedirvar[1]);
	}
	if (ncmds > 1) {
		if (i == 0) {
			dup2(p[0][1], 1);
		} else if ((ncmds - 1) == i) {
			dup2(p[i - 1][0], 0);
		} else {
			dup2(p[i][1], 1);
			dup2(p[i - 1][0], 0);
		}
		close_pipes();
	}
	if (stdinput) {
		close(pipestdinput[0]);
		close(pipestdinput[1]);
	}
	execv(cmds[i].path, cmds[i].argv);
	fprintf(stderr, "error: exec: %s: %s\n", cmds[i].argv[0], strerror(errno));
	exit(1);
}

void
execute()
{
	int i, pid, sts;
	char status[16];

	create_pipes();
	for (i = 0; i < ncmds; i++) {
		pid = fork();
		switch (pid) {
		case -1:
			fprintf(stderr, "error: fork: %s\n", strerror(errno));
			exit(1);
		case 0:
			forksuccess(i);
		default:
			pids[i] = pid;
		}
	}
	close_pipes();
	if (stdinput) {
		close(pipestdinput[0]);
		close(pipestdinput[1]);
	}
	if (redirvar) {
		close(piperedirvar[1]);
		read(piperedirvar[0], var, 1024);
		setenv(varname, var, 1);
		close(piperedirvar[0]);
	}
	if (background == 0) {
		for (i = 0; i < ncmds; i++) {
			if (waitpid(pids[i], &sts, 0) < 0) {
				fprintf(stderr, "error: wait: %s\n", strerror(errno));
				exit(1);
			}
			snprintf(status, 16, "%d", sts);
			setenv("RESULT", status, 1);
		}
	}
}

int
tokenize(char *string, char *delim, char *out[])
{
	char *aux;
	char *token;
	int n = 0;

	token = strtok_r(string, delim, &aux);
	while (token != NULL) {
		out[n] = token;
		n++;
		string = aux;
		token = strtok_r(string, delim, &aux);
	}
	return n;
}

void
checkenvvar(char *args[64], int n)
{
	int i;
	char *c;

	for (i = 0; i < n; i++) {
		if (args[i][0] == '$') {
			c = &args[i][1];
			args[i] = getenv(c);
		}
	}
}

void
exepath(char *cmd, char *cmdout)
{
	char copypath[1024], cmdaux[128];
	char *paths[64];
	int n, i;

	memset(cmdout, 0, 1024);
	strcpy(copypath, getenv("PATH"));
	n = tokenize(copypath, ":", paths);
	for (i = 0; i < n; i++) {
		sprintf(cmdaux, "%s/%s", paths[i], cmd);
		if (access(cmdaux, X_OK) == 0) {
			strcpy(cmdout, cmdaux);
		}
	}
}

void
readinput(char *buffer)
{
	char line[1024];
	int i;
	char *aux[32];

	if (buffer[strlen(buffer) - 1] == '&') {
		background = 1;
	}
	if (buffer[strlen(buffer) - 1] == '[') {
		stdinput = 1;
		pipe(pipestdinput);
		while (1) {
			fgets(line, 1024, stdin);
			if (strcmp(line, "]\n") == 0) {
				break;
			}
			write(pipestdinput[1], line, strlen(line));
		}
	}
	ncmds = tokenize(buffer, "[]|\n&", tokens);
	nprcs = nprcs + ncmds;
	npipes = npipes + ncmds - 1;
	naux = tokenize(tokens[ncmds - 1], ">\n", aux);
	if (naux >= 2) {
		tokens[ncmds - 1] = aux[0];
		rediroutput = 1;
		tokenize(aux[1], " \t", aux);
		strcpy(fileoutput, aux[0]);
	}
	naux = tokenize(tokens[ncmds - 1], "<", aux);
	if (naux >= 2) {
		tokens[ncmds - 1] = aux[0];
		redirinput = 1;
		tokenize(aux[1], " \t", aux);
		strcpy(fileinput, aux[0]);
	}
	naux = tokenize(tokens[0], "%", aux);
	if (naux >= 2) {
		tokenize(aux[0], " ", aux);
		strcpy(varname, aux[0]);
		redirvar = 1;
		pipe(piperedirvar);
		strcpy(tokens[0], aux[1]);
	}
	for (i = 0; i < ncmds; i++) {
		naux = tokenize(tokens[i], " \t", cmds[i].argv);
		checkenvvar(cmds[i].argv, naux);
		cmds[i].argv[naux] = NULL;
		exepath(cmds[i].argv[0], cmds[i].path);
	}
}

void
chd()
{
	int e;
	char *home;

	if (cmds[0].argv[1] != NULL) {
		e = chdir(cmds[0].argv[1]);
		if (e < 0) {
			fprintf(stderr, "chd: %s error\n", cmds[0].argv[1]);
			exit(1);
		}
	} else {
		home = getenv("HOME");
		e = chdir(home);
		if (e < 0) {
			fprintf(stderr, "chd: %s error\n", home);
			exit(1);
		}
	}
}

void
ifok()
{
	int i;

	if (getenv("RESULT") == NULL) {
		setenv("RESULT", "0", 1);
	} else {
		if (atoi(getenv("RESULT"))) {
			setenv("RESULT", "0", 1);
		} else {
			for (i = 0; i < naux; i++) {
				cmds[0].argv[i] = cmds[0].argv[i + 1];
			}
			exepath(cmds[0].argv[0], cmds[0].path);
			execute();
		}
	}
}

void
ifnot()
{
	int i;

	if (getenv("RESULT") != NULL) {
		if (atoi(getenv("RESULT"))) {
			for (i = 0; i < naux; i++) {
				cmds[0].argv[i] = cmds[0].argv[i + 1];
			}
			exepath(cmds[0].argv[0], cmds[0].path);
			execute();
		}
	}
}

void
processcmd()
{
	if (strcmp(cmds[0].argv[0], "chd") == 0) {
		chd();
	} else if (strcmp(cmds[0].argv[0], "ifok") == 0) {
		ifok();
	} else if (strcmp(cmds[0].argv[0], "ifnot") == 0) {
		ifnot();
	} else if (strcmp(cmds[0].argv[0], "true") == 0) {
		setenv("RESULT", "0", 1);
	} else if (strcmp(cmds[0].argv[0], "false") == 0) {
		setenv("RESULT", "1", 1);
	} else {
		execute();
	}
}

int
main(int argc, char *argv[])
{
	char buffer[1024];

	while (1) {
		printf("$ ");
		if (fgets(buffer, 1024, stdin) == NULL) {
			break;
		}
		if (strlen(buffer) == 1) {
			continue;
		}
		naux = tokenize(buffer, "=\n", tokens);
		if (naux == 2) {
			setenv(tokens[0], tokens[1], 1);
			continue;
		}
		resetvars();
		readinput(buffer);
		processcmd();
		printf("%d procesos creados, %d pipes\n", nprcs, npipes);
	}
	printf("\n");
	exit(0);
}
