#include "aoc/input.h"
#include "aoc/pos.h"

typedef struct Instruction {
	enum {
		TURN_ON = 1,
		TURN_OFF = 2,
		TOGGLE = 3
	} command;
	aoc_pos_t from,to;
} Instruction;

static int  instruction_parse(Instruction *_i, char _s[]);
static void instruction_apply_1(const Instruction *_i);
static void instruction_apply_2(const Instruction *_i);

char grid_1[1000][1000] = {0};
char grid_2[1000][1000] = {0};
int  lit_count = 0;
int  brightness = 0;

int
main(int _argc, char *_argv[])
{
	FILE        *fp;
	char         buffer[512];
	Instruction  instruction;
	int          e;

	fp = aoc_input("2015", 6, 1);
	if (!fp/*err*/) { return 1; }

	for (int line=1; fgets(buffer, sizeof(buffer)-1, fp); line++) {
		e = instruction_parse(&instruction, buffer);
		if (e<0/*err*/) {
			fprintf(stderr, "error: Line %i: Invalid format.\n", line);
			return 1;
		}
		instruction_apply_1(&instruction);
		instruction_apply_2(&instruction);
	}

	fclose(fp);

	printf("%i %i\n", lit_count, brightness);
	return 0;
}

static int
instruction_parse(Instruction *_i, char _s[])
{
	if (sscanf(
	    _s, "turn on %li,%li through %li,%li",
	    &_i->from.x, &_i->from.y,
	    &_i->to.x, &_i->to.y
	) == 4) {
		_i->command = TURN_ON;
		return 0;
	}

	if (sscanf(
	    _s, "turn off %li,%li through %li,%li",
	    &_i->from.x, &_i->from.y,
	    &_i->to.x, &_i->to.y
	) == 4) {
		_i->command = TURN_OFF;
		return 0;
	}

	if (sscanf(
	    _s, "toggle %li,%li through %li,%li",
	    &_i->from.x, &_i->from.y,
	    &_i->to.x, &_i->to.y
	) == 4) {
		_i->command = TOGGLE;
		return 0;
	}

	return -1;
}

static void
instruction_apply_1(const Instruction *_i)
{
	for (int x = _i->from.x; x<=_i->to.x; x++) {
		for (int y = _i->from.y; y<=_i->to.y; y++) {
			switch (_i->command) {
			case TURN_ON:
				if (!grid_1[x][y]) lit_count++;
				grid_1[x][y] = 1;
				break;
			case TURN_OFF:
				if (grid_1[x][y]) lit_count--;
				grid_1[x][y] = 0;
				break;
			case TOGGLE:
				if (grid_1[x][y]) lit_count--; else lit_count++;
				grid_1[x][y] = !grid_1[x][y];
				break;
			}
		}
	}
}

static void
instruction_apply_2(const Instruction *_i)
{
	for (int x = _i->from.x; x<=_i->to.x; x++) {
		for (int y = _i->from.y; y<=_i->to.y; y++) {
			switch (_i->command) {
			case TURN_ON:
				grid_2[x][y]++;
				brightness++;
				break;
			case TURN_OFF:
				if (grid_2[x][y]) {
					grid_2[x][y]--;
					brightness--;
				}
				break;
			case TOGGLE:
				grid_2[x][y]+=2;
				brightness+=2;
				break;
			}
		}
	}
}
