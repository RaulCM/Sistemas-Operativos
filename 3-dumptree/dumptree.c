// Raul Cano Montero
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void
readfile(char *fullpath)
{
	int fd;
	int nr;
	char buff[1024];

	fd = open(fullpath, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "error: open %s: %s\n", fullpath, strerror(errno));
		exit(1);
	}
	while (1) {
		nr = read(fd, buff, sizeof buff);
		if (nr < 0) {
			fprintf(stderr, "error: read %s: %s\n", fullpath, strerror(errno));
			exit(1);
		} else if (nr == 0) {
			break;
		} else {
			if (write(1, buff, nr) != nr) {
				fprintf(stderr, "error: write %s: %s\n", fullpath, strerror(errno));
			}
		}
	}
	close(fd);
}

void
readdirectory(char *path)
{
	DIR *d;
	struct dirent *entry;
	char fullpath[1024];
	struct stat st;

	d = opendir(path);
	if (d == NULL) {
		fprintf(stderr, "error: opendir: %s: %s\n", path, strerror(errno));
		exit(1);
	}
	while(1) {
		entry = readdir(d);
		if (entry == NULL) {
			break;
		}
		sprintf(fullpath, "%s/%s", path, entry->d_name);
		if (entry->d_name[0] != '.') {
			printf("%s\n", fullpath);
			if (stat(fullpath, &st) < 0) {
				fprintf(stderr, "error: stat: %s: %s\n", fullpath, strerror(errno));
				exit(1);
			}
			if ((st.st_mode & S_IFMT) == S_IFREG) {
				readfile(fullpath);
			}
			if ((st.st_mode & S_IFMT) == S_IFDIR) {
				readdirectory(fullpath);
			}
		}
	}
	closedir(d);
}

int
main(int argc, char *argv[])
{
	char path[1024];
	DIR *dir;

	if (argc == 1) {
		strcpy(path, ".");
	} else if (argc == 2) {
		strcpy(path, argv[1]);
	} else {
		fprintf(stderr, "%s: Usage:\t%s\n\t\t\t%s [directory name]\n", argv[0], argv[0], argv[0]);
		exit(1);
	}
	dir = opendir(path);
	if (dir == NULL) {
		fprintf(stderr, "error: opendir: %s: %s\n", path, strerror(errno));
		exit(1);
	}
	printf("%s\n", path);
	readdirectory(path);
	closedir(dir);
	exit(0);
}
