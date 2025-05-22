/**
 * @file upermute.h
 * @brief Generic permutation function.
 *
 * @date 2025/05/19
 * @bug No known bugs.
 */
#ifndef UPERMUTE_H
#define UPERMUTE_H

#ifndef UPERMUTE_TYPE
#  error Please define UPERMUTE_TYPE before including upermute.h
#endif
#ifndef UPERMUTE_UDATA_TYPE
#  define UPERMUTE_UDATA_TYPE void*
#endif

#include <string.h>

typedef UPERMUTE_TYPE        upermute_t;
typedef UPERMUTE_UDATA_TYPE  upermute_udata_t;

static inline void
swap(upermute_t *x, upermute_t *y) {
	upermute_t t[1];
	memcpy(t, y, sizeof(upermute_t));
	memcpy(y, x, sizeof(upermute_t));
	memcpy(x, t, sizeof(upermute_t));
}

static void
permute(upermute_t a[], int l, int r, void (*f) (upermute_udata_t u, upermute_t a[], int r), upermute_udata_t u) 
{
	if (l == r) {
		f(u, a, r);
		return;
	}

	for (int i=l; i<=r; i++) {
		swap((a + l), (a + i));
		permute(a, l + 1, r, f, u);
		swap((a + l), (a + i));
	}
}

#endif
