#include "aoc/input.h"
#include <limits.h>
#include <stdlib.h>

typedef struct MFCSAM MFCSAM;

struct MFCSAM {
	int  number;
	long val[10];
	int  val_p[10];
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

static int  MFCSAM_read(MFCSAM *_a, FILE *_fp);
static int  MFCSAM_equal_1(MFCSAM const *_a, MFCSAM const *_b);
static int  MFCSAM_equal_2(MFCSAM const *_a, MFCSAM const *_b);

int
main(int _argc, char *_argv[])
{
	FILE        *fp;
	err_t        err;
	MFCSAM       tape_db;
	int          result1 = -1, result2 = -1;

	MFCSAM tape_card = {
		-1,
		{ 3, 7, 2, 3, 0, 0, 5, 3, 2, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	};

	err = aoc_input(&fp, "2015", 16, 1);
	if (err/*err*/) { fprintf(stderr, "error: %s\n", err); return 1; }
	
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
	for (int i=0; MFCSAM_properties[i]; i++) {
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
MFCSAM_equal_1(MFCSAM const *_a, MFCSAM const *_b)
{
	for (int i=0; MFCSAM_properties[i]; i++) {
		if (_a->val_p[i] && _b->val_p[i] && (_a->val[i]!=_b->val[i])) {
			return 0;
		}
	}
	return 1;
}

static int
MFCSAM_equal_2(MFCSAM const *_a, MFCSAM const *_b)
{
	for (int i=0; MFCSAM_properties[i]; i++) {
		if (_b->val_p[i] && _a->val_p[i])
		switch (MFCSAM_property_type[i]) {
		case '=': if (_b->val[i] != _a->val[i]) { return 0; } break;
		case '>': if (_b->val[i] <= _a->val[i]) { return 0; } break;
		case '<': if (_b->val[i] >= _a->val[i]) { return 0; } break;
		}
	}
	return 1;
}
