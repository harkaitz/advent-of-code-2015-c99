#include "aoc/input.h"
#include "aoc/utable.h"

static int  utable_neighbours(utable_t *_t, size_t _x, size_t _y);
static void utable_step(utable_t *_t, int _borders);
static long utable_count(utable_t *_t);

int
main(int _argc, char *_argv[])
{
	FILE          *fp;
	utable_t      *table1,*table2,*origin;
	long           result1, result2;

	fp = aoc_input(_argv[1], "2015", 18, 1);
	if (!fp/*err*/) { return 1; }
	origin = utable_read(101*101, fp);
	if (!origin/*err*/) { return 1; }
	fclose(fp);

	table1 = utable_copy(origin);
	table2 = utable_copy(origin);
	if (!table1 || !table2/*err*/) { return 1; }
	for (int i=0; i<100; i++) {
		utable_step(table1, 0);
		utable_step(table2, 1);
	}
	result1 = utable_count(table1);
	result2 = utable_count(table2);

	printf("%li %li\n", result1, result2);

	return 0;
}

static int
utable_neighbours(utable_t *_t, size_t _x, size_t _y)
{
	long dx[] = {-1,-1,-1,    1, 1, 1,     0, 0};
	long dy[] = {-1, 0, 1,   -1, 0, 1,    -1, 1};
	int count = 0;
	for (int i=0; i<8; i++) {
		long nx = _x+dx[i];
		long ny = _y+dy[i];
		if (UTABLE_I(_t, nx, ny) && UTABLE_V(_t, nx, ny)=='#') {
			count++;
		}
	}
	return count;
}

static void
utable_step(utable_t *_t, int _borders)
{
	utable_t *cp = utable_copy(_t);
	UTABLE_FOREACH_Y(y, cp) {
		UTABLE_FOREACH_X(x, cp) {
			char c = UTABLE_V(cp, x, y);
			int  n = utable_neighbours(cp, x, y);
			if (c == '#' && n != 2 && n != 3) {
				UTABLE_V(_t, x, y) = '.';
			} else if (c == '.' && (n==3)) {
				UTABLE_V(_t, x, y) = '#';
			}
		}
	}
	if (_borders) {
		UTABLE_V(_t,  0      , 0      ) = '#';
		UTABLE_V(_t,  _t->X-1, 0      ) = '#';
		UTABLE_V(_t,  0      , _t->Y-1) = '#';
		UTABLE_V(_t,  _t->X-1, _t->Y-1) = '#';
	}
	free(cp);
}

static long
utable_count(utable_t *_t)
{
	long count = 0;
	UTABLE_FOREACH_Y(y, _t) {
		UTABLE_FOREACH_X(x, _t) {
			if (UTABLE_V(_t, x, y)=='#') {
				count++;
			}
		}
	}
	return count;
}
