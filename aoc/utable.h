/**
 * @file utable.h
 * @brief Character table.
 *
 * @date 2025/05/19
 * @bug No known bugs.
 */
#ifndef UTABLE_H
#define UTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct utable_s utable_t;

struct utable_s {
	size_t  X,Y;
	size_t  bsz;
	size_t  sep;
	char    b[1];
};

static inline utable_t *
utable_read(size_t _bsz, FILE *_fp)
{
	utable_t *t;
	char *nl;
	t = malloc(sizeof(utable_t)+_bsz);
	if (!t/*err*/) {
		fprintf(stderr, "error: Not enough memory.\n");
		return NULL;
	}
	t->bsz = fread(t->b, 1, _bsz, _fp);
	t->b[t->bsz] = '\0';
	nl = strchr(t->b, '\n');
	if (t->bsz==0 || t->bsz == _bsz || !nl/*err*/) {
		fprintf(stderr, "error: Invalid table data.\n");
		free(t);
		return NULL;
	}
	t->sep = (nl - t->b)+1;
	t->X = nl - t->b;
	if (t->b[(t->X)-1]=='\r')
		t->X--;
	t->Y = t->bsz / t->sep;
	for (size_t y=0; y<t->Y; y++) {
		if (!strchr("\r\n", t->b[(y*t->sep)+(t->X)])/*err*/) {
			fprintf(stderr, "error: Not a square, line %li\n", y+1);
			free(t);
			return NULL;
		}
	}
	return t;
}

static inline utable_t *
utable_copy(utable_t *_f) {
	utable_t *t = malloc(sizeof(utable_t)+_f->bsz);
	if (!t/*err*/) {
		fprintf(stderr, "error: Not enough memory.\n");
		return NULL;
	}
	memcpy(t, _f, sizeof(utable_t)+_f->bsz);
	return t;
}

static inline void
utable_print(utable_t const *_t, FILE *_fp)
{
	fputs(_t->b, _fp);
}

#define UTABLE_I(_t,_x,_y) ((_x) >= 0 && (_y) >= 0 && (_x) < (_t)->X && (_y) < (_t)->Y)
#define UTABLE_V(_t,_x,_y) (_t)->b[((_y)*(_t)->sep) + (_x) ]

#define UTABLE_FOREACH_X(_x,_t) for(long _x = 0; _x<(_t)->X; _x++)
#define UTABLE_FOREACH_Y(_y,_t) for(long _y = 0; _y<(_t)->Y; _y++)





#endif
