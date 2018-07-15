// Raul Cano Montero
#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void
changeperm(char *path, mode_t perm)
{
	if (chmod(path, perm) < 0) {
		fprintf(stderr, "error: chmod: %s: %s\n", path, strerror(errno));
		exit(1);
	}
}

void
copy_file(char *source, char *destination, mode_t permfich)
{
	int nr;
	char buff[1024];
	int fdin;
	int fdout;

	fdin = open(source, O_RDONLY);
	if (fdin < 0) {
		fprintf(stderr, "error: open %s\n", source);
		exit(1);
	}
	fdout = creat(destination, 0777);
	if (fdout < 0) {
		fprintf(stderr, "error: creat: %s: %s\n", destination, strerror(errno));
		exit(1);
	}
	for (;;) {
		nr = read(fdin, buff, sizeof buff);
		if (nr < 0)
			err(1, "error: read ");
		if (nr == 0)
			break;
		if (write(fdout, buff, nr) != nr)
			err(1, "error: write ");
	}
	changeperm(destination, permfich);
	close(fdin);
	close(fdout);
}

void
directories(char *source, char *destination, mode_t permdir, mode_t permfich)
{
	DIR *d;
	struct dirent *entry;
	char fullsourcepath[1024];
	char fulldestinationpath[1024];
	struct stat st;
	int e;

	d = opendir(source);
	if (d == NULL) {
		fprintf(stderr, "error: opendir: %s: %s\n", source, strerror(errno));
		exit(1);
	}
	for (;;) {
		entry = readdir(d);
		if (entry == NULL) {
			break;
		}
		sprintf(fullsourcepath, "%s/%s", source, entry->d_name);
		sprintf(fulldestinationpath, "%s/%s", destination, entry->d_name);
		if (entry->d_name[0] != '.') {
			if (stat(fullsourcepath, &st) < 0) {
				fprintf(stderr, "error: stat: %s: %s\n", fullsourcepath, strerror(errno));
				exit(1);
			}
			if ((st.st_mode & S_IFMT) == S_IFREG) {
				copy_file(fullsourcepath, fulldestinationpath, permfich);
			}
			if ((st.st_mode & S_IFMT) == S_IFDIR) {
				e = mkdir(fulldestinationpath, 0777);
				if (e < 0) {
					fprintf(stderr, "error: mkdir: %s: %s\n", fulldestinationpath, strerror(errno));
					exit(1);
				}
				directories(fullsourcepath, fulldestinationpath, permdir, permfich);
			}
		}
	}
	changeperm(destination, permdir);
	closedir(d);
}

void
readinput(char *source, char *destination, mode_t permdir, mode_t permfich)
{
	struct stat st;
	int e;

	if (stat(source, &st) < 0) {
		fprintf(stderr, "error: stat: %s: %s\n", source, strerror(errno));
		exit(1);
	}
	if ((st.st_mode & S_IFMT) == S_IFREG) {
		copy_file(source, destination, permfich);
	}
	if ((st.st_mode & S_IFMT) == S_IFDIR) {
		e = mkdir(destination, 0777);
		if (e < 0) {
			fprintf(stderr, "error: mkdir: %s: %s\n", destination, strerror(errno));
			exit(1);
		}
		directories(source, destination, permdir, permfich);
	}
}

mode_t
readperm(char *permoctal)
{
	mode_t perm;

	perm = strtol(permoctal, NULL, 8);
	if (perm == 0) {
		fprintf(stderr, "error: strtol: %s\n", permoctal);
		exit(1);
	}
	return perm;
}

int
main(int argc, char *argv[])
{
	mode_t permfich;
	mode_t permdir;

	if (argc != 5) {
		fprintf(stderr, "%s: Usage:\t%s [permisos_directorio] [permisos_fichero] [origen] [destino]\n", argv[0], argv[0]);
		exit(1);
	}
	permdir = readperm(argv[1]);
	permfich = readperm(argv[2]);
	readinput(argv[3], argv[4], permdir, permfich);
	exit(0);
}
