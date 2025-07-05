#include "aoc/input.h"

typedef struct computer_s    computer_t;
typedef struct instruction_s instruction_t;

struct computer_s {
	unsigned int a, b;
};
struct instruction_s {
	enum { I_HLF, I_TPL, I_INC, I_JMP, I_JIE, I_JIO } name;
	char reg;
	int val;
};

static int instruction_parse(instruction_t *_i, char const _l[]);
static int computer_run(computer_t *_comp, instruction_t const _program[], size_t _programsz);

int
main(int _argc, char *_argv[])
{
	int           result1, result2;
	computer_t    computer;
	instruction_t program[100];
	size_t        programsz = 0;
	int           e;
	FILE         *fp;
	char          buffer[100] = {0};

	fp = aoc_input(_argv[1], "2015", 23, 1);
	if (!fp/*err*/) { return 1; }
	while (programsz < 100 && fgets(buffer, sizeof(buffer)-1, fp)) {
		e = instruction_parse(program+programsz, buffer);
		if (e<0/*err*/) { return 1; }
		programsz++;
	}
	fclose(fp);

	computer.a = 0;
	computer.b = 0;
	e = computer_run(&computer, program, programsz);
	if (e<0/*err*/) { return 1; }
	result1 = computer.b;

	computer.a = 1;
	computer.b = 0;
	e = computer_run(&computer, program, programsz);
	if (e<0/*err*/) { return 1; }
	result2 = computer.b;

	printf("%i %i\n", result1, result2);
	return 0;
}

static int
instruction_parse(instruction_t *_i, char const _l[])
{
	memset(_i, 0, sizeof(instruction_t));
	if (sscanf(_l, "hlf %c", &_i->reg)==1) {
		_i->name = I_HLF;
	} else if (sscanf(_l, "tpl %c", &_i->reg)==1) {
		_i->name = I_TPL;
	} else if (sscanf(_l, "inc %c", &_i->reg)==1) {
		_i->name = I_INC;
	} else if (sscanf(_l, "jmp %i", &_i->val)==1) {
		_i->name = I_JMP;
	} else if (sscanf(_l, "jie %c, %i", &_i->reg, &_i->val)==2) {
		_i->name = I_JIE;
	} else if (sscanf(_l, "jio %c, %i", &_i->reg, &_i->val)==2) {
		_i->name = I_JIO;
	} else {
		fprintf(stderr, "error: Invalid instruction: %s\n", _l);
		return -1;
	}
	if (_i->reg && !strchr("ab", _i->reg)) {
		fprintf(stderr, "error: Invalid register.\n");
		return -1;
	}
	return 0;
}

static int
computer_run(computer_t *_comp, instruction_t const _program[], size_t _programsz)
{
	instruction_t const *i;
	unsigned int *r = NULL;
	int pc = 0;
	while (pc >=0 && pc < _programsz) {
		i = &_program[pc];
		switch (i->reg) {
		case 'a': r = &_comp->a; break;
		case 'b': r = &_comp->b; break;
		}
		switch (i->name) {
		case I_HLF: *r = (*r)/2; pc++; break;
		case I_TPL: *r = (*r)*3; pc++; break;
		case I_INC: *r = (*r)+1; pc++; break;
		case I_JMP: pc += i->val; break;
		case I_JIE: if ((*r)%2==0) pc += i->val; else pc++; break;
		case I_JIO: if ((*r)==1)   pc += i->val; else pc++; break;
		}
	}
	if (pc<0 || pc > _programsz) {
		fprintf(stderr, "error: Can't jump out of memory.\n");
		return -1;
	}
	return 0;
}
