/**
 * @file usymbol.h
 * @brief Identify strings with numbers.
 *
 * @date 2025/05/18
 * @bug No known bugs.
 */
#ifndef USYMBOL_H
#define USYMBOL_H

#define USYMBOL_MAX 100

#define USYMBOL_ERROR_NOT_FOUND -1
#define USYMBOL_ERROR_LIMIT     -2
#define USYMBOL_ERROR_MEMORY    -3

extern int usymbol_get(char const _s[]);
extern int usymbol_new(char const _s[]);
extern int usymbol_count(void);

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


#endif

#endif
