#ifndef AOC_INPUT_H
#define AOC_INPUT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int
aoc_input_directory(char _b[], size_t _bsz, char const _year[])
{
	char *p; FILE *fp;
	if ((p = getenv("AOC_INPUT_DIR"))) {
		return snprintf(_b, _bsz, "%s/%s", p, _year);
	} else if ((fp = fopen("./examples/01.data", "rb"))) {
		fclose(fp);
		return snprintf(_b, _bsz, "./examples");
	} else {
		return snprintf(_b, _bsz, ".");
	}
}

static FILE *
aoc_input(char const _arg1[], char const _year[], int _day, int _part)
{
	char fn[512]; int pos, len; FILE *fp;

	if (_arg1) {
		fp = fopen(_arg1, "rb");
		if (fp) { return fp; }
	}

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

	fprintf(
	    stderr,
	    "Usage: aoc-%s-%02i [INPUT]\n"
	    "\n"
	    "Solution to the Advent of Code %s day %i puzzle.\n"
	    "\n"
	    "Please specify your input as argument or set $AOC_INPUT_DIR\n"
	    "environment variable to the place where you stored your inputs\n"
	    "as `$AOC_INPUT_DIR/%s/%02i.data`.\n",
	    _year, _day, _year, _day, _year, _day
	);
	return NULL;
}

#endif
