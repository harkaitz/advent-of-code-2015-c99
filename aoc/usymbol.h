/**
 * @file usymbol.h
 * @brief Identify strings with numbers.
 *
 * @date 2025/05/18
 * @bug No known bugs.
 */
#ifndef USYMBOL_H
#define USYMBOL_H

#include <stdio.h>

#ifndef USYMBOL_MAX
#  define USYMBOL_MAX 100
#endif

#define USYMBOL_ERROR_NOT_FOUND -1
#define USYMBOL_ERROR_LIMIT     -2
#define USYMBOL_ERROR_MEMORY    -3

#define USYMBOL_END -1

extern int    usymbol_get(char const _s[]);
extern int    usymbol_new(char const _s[]);
extern int    usymbol_count(void);
extern size_t usymbol_a_insert(int _out[], size_t _outsz, int _big[], int _mini[], size_t _pos, size_t _count);
extern void   usymbol_a_printf(int _lst[], char const _start[], char const _sep[], char const _end[], FILE *_fp);
extern unsigned long usymbol_a_djb2(int _lst[]);
extern int usymbol_a_mismatch(int _a[], int _b[]);

#ifdef USYMBOL_MAX
#include <string.h>
char    *usymbol_array[USYMBOL_MAX];
int      usymbol_arraysz = 0;

int
usymbol_get(char const *_symbol)
{
	for (int i=0; i<usymbol_arraysz; i++) {
		if (!strcmp(usymbol_array[i], _symbol)) {
			return i;
		}
	}
	return USYMBOL_ERROR_NOT_FOUND;
}

int
usymbol_new(char const *_symbol)
{
	int found = usymbol_get(_symbol);
	if (found>=0) {
		return found;
	}
	if (usymbol_arraysz == USYMBOL_MAX) return USYMBOL_ERROR_LIMIT;
	#ifdef USYMBOL_USE_MALLOC
	char *symbol = malloc(strlen(_symbol)+1);
	if (!symbol/*err*/) { return USYMBOL_ERROR_MEMORY; }
	strcpy(symbol, _symbol);
	#else
	char *symbol = (char*)_symbol;
	#endif
	found = usymbol_arraysz;
	usymbol_array[usymbol_arraysz++] = symbol;
	return found;
}

int
usymbol_count(void)
{
	return usymbol_arraysz;
}

char const *
usymbol_name(int _id)
{
     return usymbol_array[_id];
}

size_t
usymbol_a_length(int _f[]) {
	size_t l = 0;
	while (_f[l] != USYMBOL_END) {
		l++;
	}
	return l;
}

void
usymbol_a_copy(int _t[], int _f[])
{
	for (int *t = _t, *f = _f; 1; f++, t++) {
		*t = *f;
		if (*t == USYMBOL_END) {
			break;
		}
	}
}

size_t
usymbol_a_insert(int _out[], size_t _outsz, int _big[], int _mini[], size_t _pos, size_t _count)
{
	size_t o = 0;
	size_t skip = 0;
	for (size_t a = 0; _big[a] != USYMBOL_END; a++) {
		if (a == _pos) {
			for (int *m = _mini; *m != USYMBOL_END; m++) {
				if (o<_outsz) {
					_out[o] = *m;
				}
				o++;
			}
			skip = _count;
		}
		if (skip) {
			skip--;
			continue;
		}
		if (o<_outsz) {
			_out[o] = _big[a];
		}
		o++;
	}
	if (o<_outsz) {
		_out[o] = USYMBOL_END;
	}
	return o;
}

void
usymbol_a_printf(int _lst[], char const _start[], char const _sep[], char const _end[], FILE *_fp)
{
	fprintf(_fp, "%s", _start);
	for (int *s = _lst; *s != USYMBOL_END; s++) {
		fprintf(_fp, "%s%s", (s==_lst)?"":_sep, usymbol_name(*s));
	}
	fprintf(_fp, "%s", _end);
}

int
usymbol_a_prefixed(int _big[], int _prefix[])
{
	for (int *big = _big, *prefix = _prefix; 1; big++, prefix++) {
		if (*big == USYMBOL_END && *prefix == USYMBOL_END) {
			return 1;
		} else if (*prefix == USYMBOL_END) {
			return 1;
		} else if (*big == USYMBOL_END) {
			return 0;
		} else if (*big != *prefix) {
			return 0;
		}
	}
}


#endif

#endif
