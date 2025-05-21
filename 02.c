#include "aoc/input.h"

static int min_side(int, int, int);
static int min_perimeter(int, int, int);

int
main(int _argc, char *_argv[])
{
	FILE *fp;
	int   l,w,h,s,p;
	long long paper = 0;
	long long ribbon = 0;

	fp = aoc_input("2015", 2, 1);
	if (!fp/*err*/) { return 1; }

	while (fscanf(fp, "%i" "x" "%i" "x" "%i" "\n", &l, &w, &h)==3) {
		s = 2*l*w + 2*w*h + 2*h*l + min_side(l, w, h);
		p = l*w*h + min_perimeter(l, w, h);
		paper += s;
		ribbon += p;
	}

	fclose(fp);

	printf("%lli %lli\n", paper, ribbon);
	return 0;
}

static int
min_side(int a, int b, int c)
{
	if (a >= b && a >= c) {
		return b*c;
	} else if (b >= a && b >= c) {
		return a*c;
	} else if (c >= a && c >= b) {
		return a*b;
	} else {
		return 0;
	}
}

static int
min_perimeter(int a, int b, int c)
{
	if (a >= b && a >= c) {
		return 2*(b+c);
	} else if (b >= a && b >= c) {
		return 2*(a+c);
	} else if (c >= a && c >= b) {
		return 2*(a+b);
	} else {
		return 0;
	}
}
