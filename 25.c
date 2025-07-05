#include "aoc/input.h"

static void next_coordinates(int *r, int *c);

int
main(int _argc, char *_argv[])
{
	int           e;
	FILE         *fp;
	int           row, col;
	long          code = 20151125L;
	long          result1 = -1;

	fp = aoc_input(_argv[1], "2015", 25, 1);
	if (!fp/*err*/) { return 1; }
	e = fscanf(fp,
	    "To continue, please consult the code grid in the manual.  "
	    "Enter the code at row %i, column %i.",
	    &row, &col
	);
	fclose(fp);
	if (e!=2/*err*/) { fprintf(stderr, "error: Invalid input format.\n"); return 1; }

	while (1) {
		int r, c;
		next_coordinates(&r, &c);
		if (r == row && c == col) {
			result1 = code;
			break;
		}
		code = (code * 252533) % 33554393;
	}

	printf("%li\n", result1);
	return 0;
}

static void
next_coordinates(int *r, int *c)
{
	static int row = 0, col = 0, row_c = 1;
	*r = (row+1);
	*c = (col+1);
	if (row == 0) {
		row = row_c;
		col = 0;
		row_c++;
	} else {
		row--;
		col++;
	}
}
