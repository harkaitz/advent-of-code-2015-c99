#include "aoc/input.h"

static char const *string_size  (size_t *_dl, size_t *_cl, char const _s[]);
static size_t      encoded_size (char const _s[], size_t _l);

int
main(int _argc, char *_argv[])
{
	FILE        *fp;
	char         buffer[512];
	size_t       datalen, codelen;
	size_t       total_datalen = 0, total_codelen = 0, total_encoded = 0;
	char const  *start;

	fp = aoc_input(_argv[1], "2015", 8, 1);
	if (!fp/*err*/) { return 1; }

	while (fgets(buffer, sizeof(buffer)-1, fp)) {
		start = string_size(&datalen, &codelen, buffer);
		if (start) {
			total_datalen += datalen;
			total_codelen += codelen;
			total_encoded += encoded_size(start, codelen);
		}
	}

	fclose(fp);

	printf(
	    "%ld %ld\n",
	    total_codelen - total_datalen,
	    total_encoded - total_codelen
	);
	return 0;
}

static char const *
string_size(size_t *_dl, size_t *_cl, char const _s[])
{
	size_t dl = 0, cl = 1;
	char const *start, *p;

	start = strchr(_s, '"');
	if (!start) {
		return NULL;
	}
	
	p = start+1;
	while (*p) {
		if (*p == '\\' && *(p+1) == 'x' && *(p+2) && *(p+3)) {
			p  +=4;
			dl +=1;
			cl +=4;
		} else if (*p == '\\' && *(p+1)) {
			p  +=2;
			dl +=1;
			cl +=2;
		} else if (*p == '"') {
			cl+=1;
			if (_dl) *_dl = dl;
			if (_cl) *_cl = cl;
			return start;
		} else {
			p  += 1;
			dl += 1;
			cl += 1;
		}
	}

	return NULL;
}

static size_t
encoded_size(char const _s[], size_t _l)
{
	size_t l = 2;
	for (size_t i=0; i<_l; i++) {
		l += (strchr("\"\\", _s[i]))?2:1;
	}
	return l;
}
