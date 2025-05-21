#include "aoc/input.h"
#include "openssl/md5.h"

int
main(int _argc, char *_argv[])
{
	FILE   *fp;
	int     e;
	char    buffer[64] = {0};
	char   *p = NULL;
	unsigned char digest[MD5_DIGEST_LENGTH];
	MD5_CTX ctx;

	fp = aoc_input("2015", 4, 1);
	if (!fp/*err*/) { return 1; }
	e = !fgets(buffer, sizeof(buffer)-1, fp);
	fclose(fp);
	if (e/*err*/) { fprintf(stderr, "error: Invalid data.\n"); return 1; }
        
	if ((p = strchr(buffer, '\n'))) *p = 0;
	p = buffer + strlen(buffer);

	long found1 = -1;
	long found2 = -1;
	for (long n=0; n<10000000 && (found1 == -1 || found2 == -1); n++) {
		sprintf(p, "%li", n);
		MD5_Init(&ctx);
		MD5_Update(&ctx, buffer, strlen(buffer));
		MD5_Final(digest, &ctx);
		if (found1 == -1 && !digest[0] && !digest[1] && !(digest[2] & 0xf0)) {
			found1 = n;
		}
		if (found2 == -1 && !digest[0] && !digest[1] && !digest[2]) {
			found2 = n;
		}
	}

	printf("%li %li\n", found1, found2);

	return 0;
}
