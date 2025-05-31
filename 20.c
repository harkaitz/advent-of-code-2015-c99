#include "aoc/input.h"
#include <stdlib.h>

int
main(int _argc, char *_argv[])
{
	FILE          *fp;
	long           input, result1, result2, *houses;
	int            e;

	fp = aoc_input(_argv[1], "2015", 20, 1);
	if (!fp/*err*/) { return 1; }
	e = fscanf(fp, "%li", &input);
	fclose(fp);
	if (e != 1/*err*/) { fprintf(stderr, "error: Invalid input.\n"); return 1; }
	input = input / 10;
	houses = calloc(input, sizeof(long));
	if (!houses/*err*/) { fprintf(stderr, "error: Not enough memory.\n"); return 1; }

	result1 = input;
	for (long i = 1; i < input; i++) {
		for (long j = i; j < input; j += i) {
			if ((houses[j] += i) >= input && j < result1) {
				result1 = j;
			}
		}
	}

	memset(houses, 0, sizeof(long)*input);
	result2 = input;
	for (long i = 1; i < input; i++) {
		long visits = 0;
		for (long j = i; j < input && visits < 50; j += i) {
			if ((houses[j] = ((houses[j])?houses[j]:11) + i * 11) >= (input * 10) && (j < result2)) {
				result2 = j;
			}
			visits++;
		}
	}

	free(houses);

	printf("%li %li\n", result1, result2);
	return 0;
}
