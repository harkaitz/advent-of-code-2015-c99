#include "aoc/input.h"

static int         vowels(char _s[]);
static char const *repeated(char const _s[]);
static int         banned(char _s[]);

static char const *repeated2(char const _s[]);
static char const *repeated3(char const _s[]);

int
main(int _argc, char *_argv[])
{
	FILE *fp;
	char  string[512] = {0};
	int   count1 = 0, count2 = 0;

	fp = aoc_input("2015", 5, 2);
	if (!fp/*err*/) { return 1; }

	while (fgets(string, sizeof(string)-1, fp)) {
		if (vowels(string)>=3 && repeated(string) && !banned(string)) {
			count1++;
		}
		if (repeated2(string) && repeated3(string)) {
			count2++;
		}
	}

	fclose(fp);

	printf("%i %i\n", count1, count2);
	return 0;
}

static int
vowels(char _s[])
{
	int count = 0;
	for (char const *p = _s; *p; p++) {
		if (strchr("aeiou", *p)) {
			count++;
		}
	}
	return count;
}
static char const *
repeated(char const _s[])
{
	for (char const *p = _s; *p; p++) {
		if (*p == *(p+1)) {
			return p;
		}
	}
	return NULL;
}

static int
banned(char _s[])
{
	return (
	    strstr(_s, "ab") ||
	    strstr(_s, "cd") ||
	    strstr(_s, "pq") ||
	    strstr(_s, "xy")
	);
}

static char const *
repeated2(char const _s[])
{
	for (char const *p = _s; *p && *(p+1) && *(p+2); p++) {
		char needle[3] = { *p, *(p+1), '\0'};
		if (strstr(p+2, needle)) {
			return p;
		}
	}
	return NULL;
}

static char const *
repeated3(char const _s[])
{
	for (char const *p = _s; *p && *(p+1) && *(p+2); p++) {
		if (*p == *(p+2)) {
			return p;
		}
	}
	return NULL;
}
