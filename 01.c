#include "aoc/input.h"

int
main(int _argc, char *_argv[])
{
	FILE *fp;
	err_t err;
	int   counter = 0;
	int   position = 1;
	int   first_minus_one = -1;
	char  character;

	err = aoc_input(&fp, "2015", 1, 1);
	if (err/*err*/) { fprintf(stderr, "error: %s\n", err); return 1; }

	while ((character = fgetc(fp)) != EOF) {
		switch (character) {
		case '(': counter++; break;
		case ')': counter--; break;
		}
		if (counter == -1 && first_minus_one == -1) {
			first_minus_one = position;
		}
		position++;
	}
	fclose(fp);

	printf("%i %i\n", counter, first_minus_one);
	return 0;
}
