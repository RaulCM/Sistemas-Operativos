// Raul Cano Montero
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char *argv[])
{
	int i, j;

	for (i = 1; i < argc; i++) {
		for (j = strlen(argv[i]); j >= 0; j--) {
			printf("%c", argv[i][j]);
		}
		printf("\n");
	}
	exit(0);
}
