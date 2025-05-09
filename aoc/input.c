#include "input.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int
aoc_input_directory(char _b[], size_t _bsz, char const _year[])
{
	char *p;
	if ((p = getenv("AOC_INPUT_DIR"))) {
		return snprintf(_b, _bsz, "%s/%s", p, _year);
	} else {
		return snprintf(_b, _bsz, ".");
	}
	
}

err_t
aoc_input(FILE **_fp, char const _year[], int _day, int _part)
{
	char fn[512]; int pos, len;

	pos = aoc_input_directory(fn, sizeof(fn), _year);
	if (pos >= sizeof(fn)/*err*/) { return "The input directory path is too long."; }

	len = pos + snprintf(fn+pos, sizeof(fn)-pos, "/%02i" "p" "%i" ".data", _day, _part);
	if (len >= sizeof(fn)/*err*/) { return "The calculated input filename is too long."; }
	*_fp = fopen(fn, "rb");
	if ((*_fp)) { return NULL; }

	len = pos + snprintf(fn+pos, sizeof(fn)-pos, "/%02i" ".data", _day);
	if (len >= sizeof(fn)) { return "The calculated input filename is too long."; }
	*_fp = fopen(fn, "rb");
	if ((*_fp)) { return NULL; }

	return "Can't find the input file.";
}
