#include "aoc/input.h"
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct MFCSAM MFCSAM;

struct MFCSAM {
	int  number;
	long val[10];
	bool val_p[10];
};

static char const *MFCSAM_properties[] = {
	"children", "cats",    "samoyeds", "pomeranians",
	"akitas",   "vizslas", "goldfish", "trees",
	"cars",     "perfumes", NULL
};
static char MFCSAM_property_type[] = {
	'=', '>', '=', '<',
	'=', '=', '<', '>',
	'=', '='
};

static MFCSAM tape_card = {
	-1,
	{ 3, 7, 2, 3, 0, 0, 5, 3, 2, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};

static int  MFCSAM_read(MFCSAM *_a, FILE *_fp);
static int  MFCSAM_equal_1(MFCSAM *_a, MFCSAM *_b);
static int  MFCSAM_equal_2(MFCSAM *_a, MFCSAM *_b);

int
main(int _argc, char *_argv[])
{
	FILE        *fp;
	MFCSAM       tape_db;
	int          result1 = -1, result2 = -1;

	fp = aoc_input(_argv[1], "2015", 16, 1);
	if (!fp/*err*/) { return 1; }

	while ((result1 == -1 || result2 == -1) && MFCSAM_read(&tape_db, fp))
	{
		if (result1 == -1 && MFCSAM_equal_1(&tape_card, &tape_db))
			result1 = tape_db.number;
		if (result2 == -1 && MFCSAM_equal_2(&tape_card, &tape_db))
			result2 = tape_db.number;
	}
	fclose(fp);

	printf("%i %i\n", result1, result2);
	return 0;
}

static int
MFCSAM_property(char const _p[])
{
	for (int i=0; i<10; i++) {
		if (!strcmp(_p, MFCSAM_properties[i])) {
			return i;
		}
	}
	return -1;
}

static int
MFCSAM_read(MFCSAM *_a, FILE *_fp)
{
	char prop[3][30];
	long val[3];
	int id;
	int m = fscanf(_fp, "Sue %i: %29[^:]: %li, %29[^:]: %li, %29[^:]: %li\n",
	    &id, prop[0], &val[0], prop[1], &val[1], prop[2], &val[2]
	);
	if (m != 7) return 0;

	memset(_a, 0, sizeof(MFCSAM));
	_a->number = id;
	for (int i=0; i<3; i++) {
		int v = MFCSAM_property(prop[i]);
		if (v!=-1) {
			_a->val[v] = val[i];
			_a->val_p[v] = 1;
		}
	}
	return 1;
}
static int
MFCSAM_equal_1(MFCSAM *_a, MFCSAM *_b)
{
	for (int i=0; i<10; i++) {
		if (_a->val_p[i] && _b->val_p[i] && (_a->val[i]!=_b->val[i])) {
			return 0;
		}
	}
	return 1;
}

static int
MFCSAM_equal_2(MFCSAM *_a, MFCSAM *_b)
{
	for (int i=0; i<10; i++) {
		if (_b->val_p[i] && _a->val_p[i])
		switch (MFCSAM_property_type[i]) {
		case '=': if (_b->val[i] != _a->val[i]) { return 0; } break;
		case '>': if (_b->val[i] <= _a->val[i]) { return 0; } break;
		case '<': if (_b->val[i] >= _a->val[i]) { return 0; } break;
		}
	}
	return 1;
}
