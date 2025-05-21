#include "aoc/input.h"

typedef struct ingredient_s ingredient_t;

struct ingredient_s {
	char name[80];
	long capacity, durability, flavor, texture, calories;
	long quantity;
};

static int  ingredient_read(ingredient_t *_i, FILE *_fp);
static long recipe_score(ingredient_t _i[], size_t _isz);
static long recipe_calories(ingredient_t _i[], size_t _isz);

int
main(int _argc, char *_argv[])
{
	FILE        *fp;
	err_t        err;
	long         a,b,c,d;
	long         result1 = 0, result2 = 0, score;
	ingredient_t ingredients[10];
	size_t       ingredientsz = 0;

	err = aoc_input(&fp, "2015", 15, 1);
	if (err/*err*/) { fprintf(stderr, "error: %s\n", err); return 1; }
	while (ingredientsz < 10 && ingredient_read(ingredients+ingredientsz, fp)) { ingredientsz++; }
	fclose(fp);

	if (ingredientsz!=4/*err*/) {
		fprintf(stderr, "error: Only 4 ingredients are supported.\n");
		return 1;
	}

	for (a=0; a<=100; a++) {
		for (b=0; b<=(100-a); b++) {
			for (c=0; c<=(100-a-b); c++) {
				d = (100-a-b-c);
				ingredients[0].quantity = a;
				ingredients[1].quantity = b;
				ingredients[2].quantity = c;
				ingredients[3].quantity = d;
				score = recipe_score(ingredients, ingredientsz);
				if (score > result1) {
					result1 = score;
				}
				if (score > result2 && recipe_calories(ingredients, ingredientsz)==500) {
					result2 = score;
				}
			}
		}
	}

	printf("%li %li\n", result1, result2);
	return 0;
}

static int
ingredient_read(ingredient_t *_i, FILE *_fp)
{
	return fscanf(
	    _fp,
	    "%79[^:]: capacity %li, durability %li, flavor %li, texture %li, calories %li",
	    _i->name, &_i->capacity, &_i->durability, &_i->flavor, &_i->texture, &_i->calories
	) == 6;
}

static long
recipe_score(ingredient_t _i[], size_t _isz)
{
	long capacity = 0;
	long durability = 0;
	long flavor = 0;
	long texture = 0;
	for (size_t i=0; i<_isz; i++) {
		capacity   += _i[i].capacity    * _i[i].quantity;
		durability += _i[i].durability  * _i[i].quantity;
		flavor     += _i[i].flavor      * _i[i].quantity;
		texture    += _i[i].texture     * _i[i].quantity;
	}
	capacity   = (capacity>0)?   capacity:0;
	durability = (durability>0)? durability:0;
	flavor     = (flavor>0)?     flavor:0;
	texture    = (texture>0)?    texture:0;
	
	return capacity * durability * flavor * texture;
}

static long
recipe_calories(ingredient_t _i[], size_t _isz)
{
	long calories = 0;
	for (size_t i=0; i<_isz; i++) {
		calories += _i[i].calories*_i[i].quantity;
	}
	
	return calories;
}
