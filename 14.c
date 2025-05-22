#include "aoc/input.h"

typedef struct reindeer_s {
	char    name[60];
	int     speed_km_s;
	int     stamina_s;
	int     resting_s;
} reindeer_t;

static int reindeer_read(reindeer_t *_r, FILE *_fp);
static int reindeer_distance_traveled(reindeer_t const *_r, int race_s);

int
main(int _argc, char *_argv[])
{
	FILE        *fp;
	int          result1 = 0, result2 = 0, d;
	int          race_s = 2503;
	reindeer_t   reindeers[20];
	size_t       reindeersz = 0;

	fp = aoc_input(_argv[1], "2015", 14, 1);
	if (!fp/*err*/) { return 1; }
	while (reindeersz < 20 && reindeer_read(reindeers+reindeersz, fp)) { reindeersz++; }
	fclose(fp);

	for (size_t r = 0; r<reindeersz; r++) {
		if ((d = reindeer_distance_traveled(reindeers+r, race_s)) > result1)
			result1 = d;
	}

	int travelled[20] = {0};
	int running[20] = {0};
	int resting[20] = {0};
	int points[20] = {0};
	int lead = 0;

	for (int s=0; s<race_s; s++) {
		for (size_t r=0; r<reindeersz; r++) {
			if (running[r]<reindeers[r].stamina_s) {
				running[r]++;
				travelled[r] += reindeers[r].speed_km_s;
				continue;
			}
			resting[r]++;
			if (resting[r]==reindeers[r].resting_s) {
				running[r]=0;
				resting[r]=0;
			}
		}
		for (size_t r=0; r<reindeersz; r++) {
			if (travelled[r] > travelled[lead])
				lead = r;
		}
		for (size_t r=0; r<reindeersz; r++) {
			if (travelled[r] == travelled[lead])
				points[r]++;
		}
	}

	for (size_t r=0; r<reindeersz; r++) {
		if (points[r] > result2)
			result2 = points[r];
	}

	printf("%i %i\n", result1, result2);
	return 0;
}

static int
reindeer_read(reindeer_t *_r, FILE *_fp)
{
	return fscanf(
	    _fp,
	    "%59s can fly %i km/s for %i seconds, but then must rest for %i seconds.",
	    _r->name, &_r->speed_km_s, &_r->stamina_s, &_r->resting_s
	) == 4;
}

static int
reindeer_distance_traveled(reindeer_t const *_r, int race_s)
{
	int full_runstop_c = race_s / (_r->stamina_s + _r->resting_s);
	int last_runstop_s = race_s % (_r->stamina_s + _r->resting_s);
	int run_s =
	    (full_runstop_c)*(_r->stamina_s)
	    +
	    ((last_runstop_s>(_r->stamina_s))?_r->stamina_s:last_runstop_s);
	return run_s * _r->speed_km_s;
}
