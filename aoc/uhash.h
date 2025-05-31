/**
 * @file uhash.h
 * @brief Calculate djb2 value.
 *
 * @date 2025/05/30
 * @bug No known bugs.
 */
#ifndef UHASH_H
#define UHASH_H

#include <stdio.h>

static inline unsigned int
uhash_crc32b(void const *data, size_t _len)
{
	unsigned int byte, crc, mask;
	int i = 0, j;
	crc = 0xFFFFFFFF;
	for (size_t x=0; x<_len; x++) {
		byte = ((unsigned char *) data)[x];
		crc = crc ^ byte;
		for (j = 7; j >= 0; j--) {
			mask = -(crc & 1);
			crc = (crc >> 1) ^ (0xEDB88320 & mask);
		}
		i = i + 1;
	}
	return ~crc;
}

static inline int
uhash_a_add(unsigned long _a[], size_t _asz, size_t *_pos, unsigned long _hash)
{
	size_t sz = (*_pos<_asz)?*_pos:_asz;
	for (size_t f = 0; f < sz; f++) {
		if (_a[f] == _hash) {
			return 0;
		}
	}
	if ((*_pos) < _asz) {
		_a[(*_pos)++] = _hash;
		return 1;
	} else {
		(*_pos)++;
		return -1;
	}
}



#endif
