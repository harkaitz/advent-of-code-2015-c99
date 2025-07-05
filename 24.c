#include <omp.h>
#include "aoc/input.h"
#include <limits.h>

static int weights[100];
static int weightsz = 0;
static long long best_count, best_qe;

static long weights_sum(void);
static void calculate(long target_weight);

int
main(int _argc, char *_argv[])
{
	FILE         *fp;
	long          weight;
	long          result1, result2;

	fp = aoc_input(_argv[1], "2015", 24, 1);
	if (!fp/*err*/) { return 1; }
	while (fscanf(fp, "%i\n", &weights[weightsz])==1 && weights[weightsz]) {
		weightsz++;
	}
	fclose(fp);
	weight = weights_sum();

	best_count = best_qe = INT_MAX;
	calculate(weight / 3);
	result1 = best_qe;

	best_count = best_qe = INT_MAX;
	calculate(weight / 4);
	result2 = best_qe;

	printf("%li %li\n", result1, result2);
	return 0;
}

static long
weights_sum(void)
{
	long weight = 0;
	for (size_t s=0; s<weightsz; s++) {
		weight += weights[s];
	}
	return weight;
}

static void
calculate(long target_weight)
{
	unsigned long limit = 0x01 << (weightsz);
	for (unsigned long bignum = limit-1; bignum > 0; bignum--) {
		long long count = 0;
		long long qe = 1;
		long long weight = 0;
		int       skip = 0;
		for (int shift = (weightsz-1); shift >= 0; shift--) {
			if (skip) {
				bignum = bignum & (~(0x01 << shift));
			} else if (bignum & (0x01 << shift)) {
				count++;
				qe *= weights[shift];
				weight += weights[shift];
				if ((weight > target_weight) ||
				    (count > best_count) ||
				    (count == best_count && qe > best_qe)) {
					skip = 1;
				} else if (weight == target_weight) {
					best_count = count;
					best_qe = qe;
					skip = 1;
				}
			}
		}
	}
}
