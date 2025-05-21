#include "aoc/input.h"

char table[2*10000][2*10000] = {0};

int
main(int _argc, char *_argv[])
{
	FILE *fp;
	char  chr;
	int   x1 = 9999, y1 = 9999;
	int   x_santa = 9999, y_santa = 9999;
	int   x_robot = 9999, y_robot = 9999;
	int   houses_1 = 1;
	int   houses_2 = 1;

	fp = aoc_input("2015", 3, 1);
	if (!fp/*err*/) { return 1; }

	table[x1][y1] = 0x1|0x2;
	for (int pos = 0; (chr = fgetc(fp)) != EOF; pos++) {
		switch(chr) {
		case '^':
			y1--;
			if ((pos % 2) == 0) y_santa--; else y_robot--;
			break;
		case 'v':
			y1++;
			if ((pos % 2) == 0) y_santa++; else y_robot++;
			break;
		case '>':
			x1++;
			if ((pos % 2) == 0) x_santa++; else x_robot++;
			break;
		case '<':
			x1--;
			if ((pos % 2) == 0) x_santa--; else x_robot--;
			break;
		}
		if (!strchr("><^v", chr)) {
			continue;
		}
		if (!(table[x1][y1] & 0x1)) {
			table[x1][y1] |= 0x1;
			houses_1++;
		}
		if ((pos % 2) == 0 && !(table[x_santa][y_santa] & 0x2)) {
			table[x_santa][y_santa] |= 0x2;
			houses_2++;
		}
		else if ((pos % 2) == 1 && !(table[x_robot][y_robot] & 0x2)) {
			table[x_robot][y_robot] |= 0x2;
			houses_2++;
		}
	}
	fclose(fp);

	printf("%i %i\n", houses_1, houses_2);
	return 0;
}
     
