#include "aoc/input.h"

static void password_shift(char _password[], char *_start);
static int  password_correct(char const _pw[]);

int
main(int _argc, char *_argv[])
{
	FILE        *fp;
	int          e;
	char         password[256], *end;

	fp = aoc_input("2015", 11, 1);
	if (!fp/*err*/) { return 1; }

	e = !fgets(password, sizeof(password)-1, fp); fclose(fp);
	if (e/*err*/) { fprintf(stderr, "error: Invalid data.\n"); return 1; }
	if ((end = strchr(password, '\n'))) { *end = '\0'; }
	end = password + strlen(password) - 1;

	do { password_shift(password, end); } while (!password_correct(password));
	printf("%s", password);

	do { password_shift(password, end); } while (!password_correct(password));
	printf(" %s\n", password);

	return 0;
}

static void
password_shift(char _password[], char *_start)
{
	if ((*_start) != 'z') {
		*_start = (*_start)+1;
		return;
	}
	*_start = 'a';
	if (_start == _password) {
		return;
	}
	password_shift(_password, --_start);
}

static int
password_ok_1(char const _pw[])
{
	for (char const *p = _pw; *p && *(p+1) && *(p+2); p++) {
		char a = *(p);
		char b = *(p+1);
		char c = *(p+2);
		if (a == (b-1) && b == (c-1)) {
			return 1;
		}
	}
	
	return 0;
}

static int
password_ok_2(char const _pw[])
{
	for (char const *p = _pw; *p; p++) {
		if (*p == 'i' || *p == 'o' || *p == 'l') {
			return 0;
		}
	}
	return 1;
}

static int
password_ok_3(char const _pw[])
{
	char found = '\0';
	for (char const *p = _pw; *p && *(p+1); p++) {
		if (*p != *(p+1) || *p == found) {
		} else if (!found) {
			found = *p;
			p++;
		} else {
			return 1;
		}
	}
	return 0;
}

static int
password_correct(char const _pw[])
{
	return password_ok_1(_pw) && password_ok_2(_pw) && password_ok_3(_pw);
}
