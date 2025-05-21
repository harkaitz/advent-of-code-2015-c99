#include "aoc/input.h"
#include <string.h>

static void play(char _fr[], char _to[]);

char buffer1[10*1024*1024] = {0}; /* 10MB */
char buffer2[10*1024*1024] = {0}; /* 10MB */

static char  *b1(int i) { return (i%2)?buffer2:buffer1; }
static char  *b2(int i) { return (i%2)?buffer1:buffer2; }
static size_t bl(void)  { return sizeof(buffer1); }

int
main(int _argc, char *_argv[])
{
	FILE        *fp;
	int          e;
	int          games1 = 40;
	int          games2 = 50;
	size_t       result1 = 0, result2 = 0;

	fp = aoc_input("2015", 10, 1);
	if (!fp/*err*/) { return 1; }

	e = !fgets(b1(0), bl()-1, fp);
	if (e/*err*/) { fprintf(stderr, "error: Invalid data.\n"); return 1; }

	for (int i=0; 1; i++) {
		if (i==games1) {
			result1 = strlen(b1(i));
		}
		if (i==games2) {
			result2 = strlen(b1(i));
			break;
		}
		play(b1(i), b2(i));
	}

	printf("%ld %ld\n", result1, result2);
	return 0;
}

static void
play(char _fr[], char _to[])
{
	char *fr = _fr;
	char *to = _to;

	while (1) {
		char chr = fr[0];
		if (!chr || !strchr("1234567890", chr)) return;

		size_t count = 1;
		while (chr==fr[count]) count++;

		int shift = sprintf(to, "%ld%c", count, chr);

		fr = fr+count;
		to = to+shift;
	}
}
