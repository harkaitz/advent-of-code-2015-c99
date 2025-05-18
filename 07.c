#include "aoc/input.h"

typedef struct Wire    Wire;
typedef unsigned short Signal;

struct Wire {
	char         name[10];
	Signal      (*gate) (Signal,Signal);
	char         a[10],b[10];
	Signal       v;
	int          v_set;
};

static err_t  wire_parse(char _s[]);
static Signal wire_get(char _n[]);


static Wire   wires[400];
static size_t wiresz = 0;

int
main(int _argc, char *_argv[])
{
	FILE        *fp;
	err_t        err;
	char         buffer[512];
	Signal       a;
	size_t       i;

	err = aoc_input(&fp, "2015", 7, 1);
	if (err/*err*/) { fprintf(stderr, "error: %s\n", err); return 1; }
	while (fgets(buffer, sizeof(buffer)-1, fp)) {
		err = wire_parse(buffer);
		if (err/*err*/) { fprintf(stderr, "error: %s: %s", err, buffer); return 1; }
	}
	fclose(fp);

	a = wire_get("a");
	for (i=0; i<wiresz; i++) {
		if (!strcmp(wires[i].name, "b")) {
			wires[i].v_set = 1;
			wires[i].v = a;
		} else {
			wires[i].v_set = 0;
		}
	}

	printf("%d %d\n", a, wire_get("a"));
	return 0;
}

static Signal wire_and   (Signal _a, Signal _b) { return _a & _b; }
static Signal wire_or    (Signal _a, Signal _b) { return _a | _b; }
static Signal wire_lshift(Signal _a, Signal _b) { return (_a << _b)&0xffff; }
static Signal wire_rshift(Signal _a, Signal _b) { return (_a >> _b)&0xffff; }
static Signal wire_not   (Signal _a, Signal _b) { return (~_a)&0xffff; }

static err_t
wire_parse(char _s[])
{
	Wire *w = wires+(wiresz++);
	memset(w, 0, sizeof(Wire));
	if (sscanf(_s, "%s -> %s", w->a, w->name)==2) {
		return NULL;
	}
	if (sscanf(_s, "%s AND %s -> %s", w->a, w->b, w->name)==3) {
		w->gate = wire_and;
		return NULL;
	}
	if (sscanf(_s, "%s OR %s -> %s", w->a, w->b, w->name)==3) {
		w->gate = wire_or;
		return NULL;
	}
	if (sscanf(_s, "%s LSHIFT %s -> %s", w->a, w->b, w->name)==3) {
		w->gate = wire_lshift;
		return NULL;
	}
	if (sscanf(_s, "%s RSHIFT %s -> %s", w->a, w->b, w->name)==3) {
		w->gate = wire_rshift;
		return NULL;
	}
	if (sscanf(_s, "NOT %s -> %s", w->a, w->name)==2) {
		w->gate = wire_not;
		return NULL;
	}
	return "Invalid line";
}

static Signal
wire_get(char _n[])
{
	Signal v;
	size_t i;
	Wire  *w;
	if (!_n[0])
		return 0;
	if (sscanf(_n, "%hd", &v)==1)
		return v;
	for (i=0; i<wiresz; i++) {
		w = wires + i;
		if (strcmp(w->name, _n))
			continue;
		if (!w->v_set) {
			w->v = w->gate?
			    w->gate(wire_get(w->a), wire_get(w->b)):
			    wire_get(w->a);
			w->v_set = 1;
		}
		return w->v;
	}
	return 0;
}
