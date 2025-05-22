#define USYMBOL_MAX         100
#define USYMBOL_USE_MALLOC  1
#define UPERMUTE_TYPE       int
#define UPERMUTE_UDATA_TYPE int *
#include "aoc/input.h"
#include "aoc/usymbol.h"
#include "aoc/upermute.h"
#include <limits.h>

typedef struct road_s road_t;

struct road_s {
	int l1,l2,d;
};

road_t roads[100] = {0};
size_t roadsz = 0;

static void route_best (int best[2], int route[], int r);

int
main(int _argc, char *_argv[])
{
	FILE        *fp;
	char         city1[60], city2[60];
	int          distance;
	int          best[2] = {INT_MAX,0};
        int          route[100];

	fp = aoc_input(_argv[1], "2015", 9, 1);
	if (!fp/*err*/) { return 1; }

	while (roadsz < 100 && fscanf(fp, "%59s to %59s = %i\n", city1, city2, &distance)==3) {
		roads[roadsz].l1 = usymbol_new(city1);
		roads[roadsz].l2 = usymbol_new(city2);
		roads[roadsz].d = distance;
		roadsz++;
	}
	fclose(fp);

	for (int i=0; i<usymbol_count(); i++)
		route[i] = i;

	permute(route, 0, usymbol_count()-1, route_best, best);

	printf("%i %i\n", best[0], best[1]);

	return 0;
}

static
int road_match(road_t const *road, int city1, int city2)
{
	return
	    (road->l1 == city1 && road->l2 == city2) ||
	    (road->l1 == city2 && road->l2 == city1);
}

static
int route_distance(int route[], int start, int end)
{
	int total = 0;
	for (int i = start; i<end; i++) {
		int c1   = route[i];
		int c2   = route[i+1];
		int dist = INT_MAX;
		for (size_t r=0; r<roadsz; r++) {
			if (road_match(roads+r, c1, c2)) {
				dist = roads[r].d;
				break;
			}
		}
		if (dist == INT_MAX) {
			return dist;
		}
		total += dist;
	}
	return total;
}

static
void route_best (int best[2], int route[], int r)
{
	int dist = route_distance(route, 0, r);
	if (dist < best[0]) {
		best[0] = dist;
	}
	if (dist > best[1]) {
		best[1] = dist;
	}
}
