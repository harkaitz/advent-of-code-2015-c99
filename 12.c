#include "aoc/input.h"
#include "aoc/json.h"
#include <stdbool.h>

char   buffer[50000];
size_t buffersz;

static long json_count_numbers(struct json_value_s *_json, bool _ignore_red);

int
main(int _argc, char *_argv[])
{
	FILE        *fp;
	err_t        err;
	struct json_value_s *json;
	long                 res1, res2;
	
	err = aoc_input(&fp, "2015", 12, 1);
	if (err/*err*/) { fprintf(stderr, "error: %s\n", err); return 1; }
	buffersz = fread(buffer, 1, sizeof(buffer)-1, fp);
	fclose(fp);

	json = json_parse(buffer, buffersz);
	if (!json/*err*/) { fprintf(stderr, "error: Invalid JSON.\n"); return 1; }
	res1 = json_count_numbers(json, false);
	res2 = json_count_numbers(json, true);

	printf("%li %li\n", res1, res2);
	return 0;
}

static long
json_count_numbers(struct json_value_s *_json, bool _ignore_red)
{
	long count = 0;
	json_number_t *number;
	json_object_t *object;
	json_object_element_t *eo;
	json_array_t *array;
	json_array_element_t *ea;
	json_string_t *string;
	
	if ((number = json_value_as_number(_json))) {
		return atoi(number->number);
	} else if ((object = json_value_as_object(_json))) {
		for (eo = object->start; eo; eo = eo->next) {
			if ((_ignore_red) &&
			    (string = json_value_as_string(eo->value)) &&
			    (!strcmp(string->string, "red")))
				return 0;
			count += json_count_numbers(eo->value, _ignore_red);
		}
		return count;
	} else if ((array = json_value_as_array(_json))) {
		for (ea = array->start; ea; ea = ea->next) {
			count += json_count_numbers(ea->value, _ignore_red);
		}
		return count;
	}
	return count;
}
