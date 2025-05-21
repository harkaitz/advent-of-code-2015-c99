#define USYMBOL_MAX 100
#define USYMBOL_USE_MALLOC 1
#define UPERMUTE_TYPE int
#define UPERMUTE_UDATA_TYPE int *
#include "aoc/input.h"
#include "aoc/usymbol.h"
#include "aoc/upermute.h"

typedef struct friendship_s friendship_t;

struct friendship_s {
	int person1, person2;
	int friendship;
};

friendship_t relations[100];
size_t       relationsz = 0;

static int  table_happiness(int _t[], int r);
static void table_search(int *_best, int _t[], int _r);

int
main(int _argc, char *_argv[])
{
	FILE        *fp;
	char         person1[60], person2[60], mode[30]; int happiness;
	int          table[20];
	size_t       tablesz;
	int          best1 = 0, best2 = 0;

	fp = aoc_input("2015", 13, 1);
	if (!fp/*err*/) { return 1; }

	while (fscanf(
	    fp,
	    "%59s would %29s %i happiness units by sitting next to %59[^.].",
	    person1, mode, &happiness, person2) == 4)
	{
		relations[relationsz].person1 = usymbol_new(person1);
		relations[relationsz].person2 = usymbol_new(person2);
		relations[relationsz].friendship = happiness * (!strcmp(mode, "gain")?1:-1);
		relationsz++;
	}
	fclose(fp);

	tablesz = usymbol_count();
	for (int i=0; i<tablesz; i++) table[i] = i;

	permute(table, 0, tablesz-1, table_search, &best1);
	table[tablesz++] = usymbol_new("Me");
	permute(table, 0, tablesz-1, table_search, &best2);

	printf("%i %i\n", best1, best2);
	return 0;
}

static int
table_happiness(int _t[], int r)
{
	int total = 0;
	for (int i=0; i<=r; i++) {
		int a = _t[i];
		int b = (i==r)?_t[0]:_t[i+1];
		for (int f = 0; f < relationsz; f++) {
			if ((relations[f].person1 == a && relations[f].person2 == b) ||
			    (relations[f].person1 == b && relations[f].person2 == a)) {
				total += relations[f].friendship;
			}
		}
	}
	return total;
}

static void
table_search(int *_best, int _t[], int _r)
{
	int happiness = table_happiness(_t, _r);
	if (happiness > *_best) {
		*_best = happiness;
	}
}
