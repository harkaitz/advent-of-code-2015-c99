#define USYMBOL_MAX 200
#define USYMBOL_USE_MALLOC 1
#include "aoc/input.h"
#include "aoc/usymbol.h"
#include "aoc/uhash.h"
#include <limits.h>

#define MAX_REACTIONS       100
#define MAX_REACTION_OUTPUT 20
#define MAX_FORMULA         300
#define MAX_FORMULA_TABLE   2000

static int        reactions[MAX_REACTIONS][2][MAX_REACTION_OUTPUT];
static size_t     reactionsz = 0;

static unsigned long table[MAX_FORMULA_TABLE];
static size_t        tablesz = 0;

static int electron;

static int part1(int _f[]);
static int part2(int _f[], int _depth);

static int formula_scan(int _f[], size_t _fsz, char _s[]);
static int reaction_next(size_t *_out, int _fr, int _f[], size_t *_pos);

int
main(int _argc, char *_argv[])
{
	FILE          *fp;
	char           buffer[1024] = {0};
	char           fr_s[32], to_s[32];
	int            e;
	int            formula[MAX_FORMULA];

	electron = usymbol_new("e");

	fp = aoc_input(_argv[1], "2015", 19, 1);
	if (!fp/*err*/) { return 1; }
	for (int line=1; fgets(buffer, sizeof(buffer)-1, fp); line++) {
		if (strchr(buffer, '=')) {
			e = sscanf(buffer, "%s => %s\n", fr_s, to_s);
			if (e != 2/*err*/) { fprintf(stderr, "error: Invalid line: %i\n", line); return 1; }
			e = formula_scan(reactions[reactionsz][0], MAX_REACTION_OUTPUT, fr_s);
			if (e<0/*err*/) { return 1; }
			e = formula_scan(reactions[reactionsz][1], MAX_REACTION_OUTPUT, to_s);
			if (e<0/*err*/) { return 1; }
			reactionsz++;
		} else if (buffer[0] == '\n') {
			continue;
		} else {
			formula_scan(formula, MAX_FORMULA, buffer);
			break;
		}
	}
	fclose(fp);

	int result1 = part1(formula);
	int result2 = part2(formula, 0);

	printf("%i %i\n", result1, result2);
	return 0;
}

static int
part1(int _formula[])
{
	int            result = 0;
	int            nformula[MAX_FORMULA];
	size_t         nformulasz;
	unsigned int   hash;
	size_t         reaction, __r;
	size_t         position;
	for (position = 0; _formula[position] != USYMBOL_END; position++) {
		for (__r = 0; reaction_next(&reaction, 0, _formula+position, &__r);) {
			nformulasz = usymbol_a_insert(
			    nformula, MAX_FORMULA, _formula,
			    reactions[reaction][1], position, 1
			);
			hash = uhash_crc32b(nformula, nformulasz*sizeof(int));
			if (uhash_a_add(table, MAX_FORMULA_TABLE, &tablesz, hash)) {
				result++;
			}
		}
	}
	return result;
}

static int
part2(int _formula[], int _depth)
{
	size_t         reaction, __r;
	long           position;
	int            result = 0;
	/* Until e for each reaction of each position. */
	while (_formula[0] != electron && _formula[1] != USYMBOL_END) {
		for (position = 0; _formula[position] != USYMBOL_END; position++) {
			for (__r = 0; reaction_next(&reaction, 1, _formula+position, &__r);) {
				usymbol_a_insert(
				    _formula, MAX_FORMULA, _formula,
				    reactions[reaction][0], position,
				    usymbol_a_length(reactions[reaction][1])
				);
				result++;
			}
		}
	}
	return result;
}

static int
formula_scan(int *_f, size_t _fsz, char _s[])
{
	size_t ret = 0;
	char   sym_s[3];
	for (char *c = _s; ret < (_fsz-1) && *c && *c!='\n'; c++) {
		sym_s[0] = *c;
		if (*(c+1) && strchr("abcdefghijklmnopqrstuvxyz", *(c+1))) {
			sym_s[1] = *(c+1);
			sym_s[2] = '\0';
			c++;
		} else {
			sym_s[1] = '\0';
		}
		_f[ret++] = usymbol_new(sym_s);
	}
	_f[ret] = -1;
	return ret;
}

static int
reaction_next(size_t *_out, int _fr, int _f[], size_t *_pos)
{
	for (size_t pos = *_pos; pos < reactionsz; pos++) {
		if (usymbol_a_prefixed(_f, reactions[pos][_fr])) {
			*_out = pos;
			*_pos = pos+1;
			return 1;
		}
	}
	*_pos = 0;
	return 0;
}
