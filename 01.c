#include "aoc/input.h"

int
main(int _argc, char *_argv[])
{
	FILE *fp;
	int   counter = 0;
	int   position = 1;
	int   first_minus_one = -1;
	char  character;

	fp = aoc_input("2015", 1, 1);
	if (!fp/*err*/) { return 1; }

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
