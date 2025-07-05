#include "aoc/input.h"
#include <limits.h>
#define SPELL_COUNT 5

typedef struct Game Game;
typedef struct Knight Knight;
typedef struct Wizard Wizard;
typedef struct Spell Spell;

struct Knight {
	int hit_points;
	int damage;
};

struct Wizard {
	int hit_points;
	int armor;
	int mana;
};

struct Game {
	int    clock;
	Knight boss;
	Wizard player;
	int    spell_active[5];
	int    spent;
};

struct Spell {
	char const *name;
	int mana, turns;
	void (*fun) (Game *);
};

static void missile(Game *_g)  { _g->boss.hit_points -= 4; }
static void drain(Game *_g)    { _g->boss.hit_points -= 2; _g->player.hit_points += 2; }
static void shield(Game *_g)   { _g->player.armor = 7; }
static void poison(Game *_g)   { _g->boss.hit_points -= 3; }
static void recharge(Game *_g) { _g->player.mana += 101; }

static Spell spells[SPELL_COUNT] = {
	{"Recharge"     , 229, 5, recharge },
	{"Magic Missile", 53 , 0, missile },
	{"Drain"        , 73 , 0, drain },
	{"Shield"       , 113, 6, shield },
	{"Poison"       , 173, 6, poison }
};

static int  knight_scanf(Knight *_k, FILE *_fp);
static void wizard_init(Wizard *_w, int _hit_points, int _armor, int _mana);
static void game_value(Game *_game, int *_best, int _depth, int _max, int hard_mode);

int
main(int _argc, char *_argv[])
{
	FILE         *fp;
	int           e;
	Game          game;
	int           result1 = INT_MAX, result2 = INT_MAX;

	fp = aoc_input(_argv[1], "2015", 22, 1);
	if (!fp/*err*/) { return 1; }
	e = knight_scanf(&game.boss, fp);
	fclose(fp);
	if (e<0/*err*/) { return 1; }
	wizard_init(&game.player, 50, 0, 500);

	game_value(&game, &result1, 0, 20, 0);
	game_value(&game, &result2, 0, 20, 1);

	printf("%i %i\n", result1, result2);
	return 0;
}

static int
knight_scanf(Knight *_knight, FILE *_fp)
{
	int e;
	memset(_knight, 0, sizeof(Knight));
	e = fscanf(_fp,
	    "Hit Points: %i" "\n"
	    "Damage: %i"     "\n",
	    &_knight->hit_points,
	    &_knight->damage
	);
	if (e != 2/*err*/) {
		fprintf(stderr, "error: Invalid input.\n");
		return -1;
	}
	return 0;
}

static void
wizard_init(Wizard *_wizard, int _hit_points, int _armor, int _mana)
{
	memset(_wizard, 0, sizeof(Wizard));
	_wizard->hit_points = _hit_points;
	_wizard->armor = _armor;
	_wizard->mana = _mana;
}

static void
game_apply_spells(Game *_game)
{
	size_t spell;
	for (spell=0; spell<5; spell++) {
		if (_game->spell_active[spell]) {
			spells[spell].fun(_game);
			_game->spell_active[spell]--;
		}
	}
}

static void
game_player_play(Game *_after, Game *_before, size_t _spell, int _hard_mode)
{
	memcpy(_after, _before, sizeof(Game));
	if (_hard_mode) {
		_after->player.hit_points--;
		if (_after->player.hit_points<=0)
			return;
	}
	game_apply_spells(_after);
	if (_after->boss.hit_points > 0) {
		_after->player.mana -= spells[_spell].mana;
		_after->spent       += spells[_spell].mana;
		if (!spells[_spell].turns)
			spells[_spell].fun(_after);
		else
			_after->spell_active[_spell] = spells[_spell].turns;
		
	}
	_after->player.armor = 0;
	_after->clock++;
}

static void
game_boss_play(Game *_after, Game *_before)
{
	int damage;
	memcpy(_after, _before, sizeof(Game));
	game_apply_spells(_after);
	damage = (_after->boss.damage - _after->player.armor);
	_after->player.hit_points -= (damage<=0)?1:damage;
	_after->player.armor = 0;
	_after->clock++;
}

static size_t
game_next(Game *_before, Game _after[SPELL_COUNT], int _hard_mode)
{
	size_t r = 0, spell;
	if (_before->clock % 2 == 0) {
		for (spell=0; spell < SPELL_COUNT; spell++) {
			if (_before->spell_active[spell] > 1)
				continue;
			if (_before->player.mana < spells[spell].mana)
				continue;
			game_player_play(_after+r, _before, spell, _hard_mode);
			r++;
		}
	} else {
		game_boss_play(_after+r, _before);
		r++;
	}
	return r;
}

static int
game_winner(Game *_game)
{
	if (_game->player.hit_points > 0 && _game->boss.hit_points <= 0)
		return 0;
	if (_game->boss.hit_points > 0 && _game->player.hit_points <= 0)
		return 1;
	return -1;
}

static void
game_value(Game *_game, int *_best, int _depth, int _max, int _hard_mode)
{
	Game    after[SPELL_COUNT];
	int     winner;
	size_t  afterc;
	winner = game_winner(_game);
	if (winner == 0) {
		if (_game->spent < *_best)
			*_best = _game->spent;
		return;
	}
	if (winner == 1)
		return;
	if (_depth == _max)
		return;
	afterc = game_next(_game, after, _hard_mode);
	for (size_t g=0; g<afterc; g++) {
		if (after[g].spent < *_best) {
			game_value(after+g, _best, _depth+1, _max, _hard_mode);
		}
	}
}
