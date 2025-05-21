#ifndef AOC_INPUT_H
#define AOC_INPUT_H

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int
aoc_input_directory(char _b[], size_t _bsz, char const _year[])
{
	char *p;
	if ((p = getenv("AOC_INPUT_DIR"))) {
		return snprintf(_b, _bsz, "%s/%s", p, _year);
	} else {
		return snprintf(_b, _bsz, ".");
	}
}

static FILE *
aoc_input(char const _year[], int _day, int _part)
{
	char fn[512]; int pos, len; FILE *fp;

	pos = aoc_input_directory(fn, sizeof(fn), _year);
	if (pos >= sizeof(fn)/*err*/) {
		fprintf(stderr, "error: The input directory path is too long.\n");
		return NULL;
	}

	len = pos + snprintf(fn+pos, sizeof(fn)-pos, "/%02i" "p" "%i" ".data", _day, _part);
	if (len >= sizeof(fn)/*err*/) {
             fprintf(stderr, "error: The calculated input filename is too long.\n");
             return NULL;
        }
	fp = fopen(fn, "rb");
	if (fp) { return fp; }

	len = pos + snprintf(fn+pos, sizeof(fn)-pos, "/%02i" ".data", _day);
	if (len >= sizeof(fn)/*err*/) {
		fprintf(stderr, "error: The calculated input filename is too long.\n");
		return NULL;
	}
	fp = fopen(fn, "rb");
	if (fp) { return fp; }

	fprintf(stderr, "error: %s: File not found.\n", fn);
	return NULL;
}

#endif
