#include "aoc/input.h"
#include <stdlib.h>
#include <limits.h>

typedef struct Item   Item;
typedef struct Knight Knight;

struct Item {
	char const *name;
	int cost, damage, armor;
};

struct Knight {
	int hit_points;
	int damage, armor, cost;
};

Item weapons[] = {
	{"Dagger"    ,  8, 4, 0},
	{"Shortsword", 10, 5, 0},
	{"Warhammer" , 25, 6, 0},
	{"Longsword" , 40, 7, 0},
	{"Greataxe"  , 74, 8, 0},
	{NULL},
};
Item armors[] = {
	{"Leather"   , 13, 0, 1},
	{"Chainmail" , 31, 0, 2},
	{"Splintmail", 53, 0, 3},
	{"Bandedmail", 75, 0, 4},
	{"Platemail" ,102, 0, 5},
	{"@None"     ,  0, 0, 0},
	{NULL},
};
Item rings[] = {
	{"Damage +1" , 25, 1, 0},
	{"Damage +2" , 50, 2, 0},
	{"Damage +3" ,100, 3, 0},
	{"Defense +1", 20, 0, 1},
	{"Defense +2", 40, 0, 2},
	{"Defense +3", 80, 0, 3},
	{"@None",       0, 0, 0},
	{NULL},
};

static int  knight_equipment_it(int _next, Knight *_k);
static void knight_init(Knight *_k);
static int  knight_scanf(Knight *_k, FILE *_fp);
static Knight const *rpg20xx_get_winner(Knight const *_p1, Knight const *_p2);

int
main(int _argc, char *_argv[])
{
	int e;
	FILE *fp;
	Knight boss, player;
	Knight const *winner;
	int result1 = INT_MAX, result2 = -1;

	fp = aoc_input(_argv[1], "2015", 21, 1);
	if (!fp/*err*/) { return 1; }
	e = knight_scanf(&boss, fp);
	fclose(fp);
	if (e == -1/*err*/) { return 1; }
	knight_init(&player);

	for (int next=0; knight_equipment_it(next, &player); next=1) {
		if (player.cost < result1 || player.cost > result2) {
			winner = rpg20xx_get_winner(&player, &boss);
			if (player.cost < result1 && winner == &player)
				result1 = player.cost;
			if (player.cost > result2 && winner == &boss)
				result2 = player.cost;
		}
	}

	printf("%i %i\n", result1, result2);
	return 0;
}

static void
knight_init(Knight *_k)
{
	memset(_k, 0, sizeof(Knight));
	_k->hit_points = 100;
}

static int
knight_scanf(Knight *_k, FILE *_fp)
{
	int e;
	knight_init(_k);
	e = fscanf(_fp,
	    "Hit Points: %i" "\n"
	    "Damage: %i"     "\n"
	    "Armor: %i"      "\n",
	    &_k->hit_points,
	    &_k->damage,
	    &_k->armor
	);
	if (e != 3/*err*/) {
		fprintf(stderr, "error: Invalid input.\n");
		return -1;
	}
	return 0;
}

static Knight const *
rpg20xx_get_winner(Knight const *_p1, Knight const *_p2)
{
	int i = 0, damage;
	Knight p1, p2, *attacker, *defender;
	memcpy(&p1, _p1, sizeof(Knight));
	memcpy(&p2, _p2, sizeof(Knight));
	while (1) {
		attacker = (i==0)?&p1:&p2;
		defender = (i==0)?&p2:&p1;
		damage = attacker->damage - defender->armor;
		if (damage < 1)
			damage = 1;
		defender->hit_points -= damage;
		if (defender->hit_points <= 0) {
			break;
		}
		i=(i+1)%2;
	}
	return (i==0)?_p1:_p2;
}

static int
knight_equipment_it(int _next, Knight *_knight)
{
	static Item *weapon, *armor, *ring_l, *ring_r;
	if (_next) {
		goto reiterate;
	}
	for (weapon = weapons; weapon->name; weapon++) {
		for (armor = armors; armor->name; armor++) {
			for (ring_l = rings; ring_l->name; ring_l++) {
				for (ring_r = rings; ring_r->name; ring_r++) {
					if (ring_l->name[0] != '@' && ring_l == ring_r) {
						continue;
					}
					_knight->damage =
					    weapon->damage + ring_l->damage + ring_r->damage;
					_knight->armor =
					    armor->armor + ring_l->armor + ring_r->armor;
					_knight->cost =
					    weapon->cost + armor->cost + ring_l->cost + ring_r->cost;
					return 1;
					reiterate:
					(void)0;
				}
			}
		}
	}
	return 0;
}
