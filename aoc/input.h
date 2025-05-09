#ifndef AOC_INPUT_H
#define AOC_INPUT_H

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef char const *err_t;

extern int   aoc_input_directory(char _b[], size_t _bsz, char const _year[]);
extern err_t aoc_input(FILE **_fp, char const _year[], int _day, int _part);

#endif
