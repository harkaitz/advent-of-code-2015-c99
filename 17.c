#include "aoc/input.h"
#include <stdbool.h>
#include <limits.h>

static int combinations(bool _c[], size_t _csz, unsigned long *_i);

int
main(int _argc, char *_argv[])
{
	FILE           *fp;
	err_t           err;
	int             containers[30];
	size_t          containersz = 0;
	bool            combination[30];
	unsigned long   combination_pos = 0;
	long            result1 = 0;
	long            result2 = 0;
	int             minimum = INT_MAX;
	long            eggnog  = 150;

	err = aoc_input(&fp, "2015", 17, 1);
	if (err/*err*/) { fprintf(stderr, "error: %s\n", err); return 1; }
	while (containersz < 30 && fscanf(fp, "%i\n", containers+containersz)==1)
		containersz++;
	fclose(fp);

	while (combinations(combination, containersz, &combination_pos)) {
		long liters = 0, count = 0;
		for (size_t i=0; i<containersz; i++) {
			if (combination[i]) {
				liters += containers[i];
				count++;
			}
		}
		if (liters == eggnog) {
			result1++;
			if (count < minimum) {
				result2 = 1;
				minimum = count;
			} else if (count == minimum) {
				result2++;
			}
		}
	}

	printf("%li %li\n", result1, result2);
	return 0;
}

static int
combinations(bool _c[], size_t _csz, unsigned long *_i)
{
	unsigned long v = *_i;
	for (size_t i=0; i<_csz; i++, v = v >> 1) {
		_c[i] = v & 0x01;
	}
	(*_i)++; 
	return !(v & 0x01);
}
