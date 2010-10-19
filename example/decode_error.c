#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include "libfoa.h"

/* 
 * Example: decode_error
 * 
 * This example should generate decode errors. This example can be
 * used to test that decoding is returning error when feed with 
 * invalid data.
 */

struct unit_test
{
	const char *buff;
	int fail;
};

static const char *failed = "failed";
static const char *passed = "passed";

int main(void) 
{
	struct libfoa foa;
	struct unit_test unit1[] = { 
		/* empty lines are not allowed: */
		{ "\n", 1 },
		{ "\n\n\n\n", 1 },
		/* test invalid formatted special chars: */
		{ "[]", 1 },
		{ "()", 1 },
		{ "[[", 1 },
		{ "[(", 1 },
		{ "([", 1 },
		{ "((", 1 },
		{ "]]", 1 },
		{ "])", 1 },
		{ ")]", 1 },
		{ "))", 1 },
		{ "[()]", 1 },
		{ "([])", 1 },
		/* missing trailing newline: */
		{ ")", 1 },
		{ "]", 1 },
		{ ")\n)", 1 },
		{ "]\n]", 1 },
		{ ")\n]", 1 },
		{ "]\n)", 1 },
		/* these test should pass: */
		{ "[\n", 0 }, 
		{ "(\n", 0 },
		{ "]\n", 0 },
		{ ")\n", 0 },
		{ "[\n(\n)\n]\n", 0 },
		{ "(\n[\n]\n)\n", 0 },
		{ "(\n[\n{}\n]\n)\n", 0 },   /* {} is simple data ...   */
		{ "(\n[\n[]\n]\n)\n", 1 },   /* ... but [] is an array! */
		/* valid named object, array or field: */
		{ "name = [\n", 0 },
		{ "name = [\n]\n", 0 },
		{ "name = (\n", 0 },
		{ "name = (\n(\n[\n]\n)\n)\n", 0 },
		{ "name = value\n", 0 },
		/* invalid named object or array: */
		{ "name = ]\n", 1 },
		{ "name = )\n", 1 },
		{ "name = []\n", 1 },
		{ "name = ()\n", 1 },
		/* allow indent of named data: */
		{ "name = [\n  name = val\n]\n", 0 },
		{ "name = (\n  name = val\n  name = val\n)\n", 0 },
		/* some more complex structures: */
		{ "name = (\nf1=v1\nf2=v2\nf3=v3\n)\n", 0 },
		{ "name = (\nf1 = v1\nf2 = v2\nf3 = v3\n)\n", 0 },
		{ "name = (\nf1=v1\nf2=v2\nf3=[\na1=v3\na1=v4\n]\n)\n", 0 },
		{ "name = (\nf1=v1\nf2=v2\nf3=[\na1=((\na1=v4\n]\n)\n", 1 },
		{ "name = [\nname = [\nname = (\n)\n]\n]\n", 0 },
		{ NULL, 0 }
	};
	struct unit_test *un1;
	int fail = 0;
	int test = 0;
	
	if(foa_init(&foa) < 0) {
		fprintf(stderr, "failed initilize libfoa: %s\n", foa_last_error(&foa));
		return 1;
	}
	
	for(un1 = unit1; un1->buff; ++un1) {
		const char *result;
		const struct foa_entity *ent;

		foa_set_buffer(&foa, un1->buff);
		
		result = passed;
		while((ent = foa_next(&foa))) {
			/* printf("name: '%s', data: '%s', type: %d\n",  */
			/*        ent->name, ent->data, ent->type); */
			if(ent->type == FOA_TYPE_ERROR_MESSAGE) {
				if(un1->fail != 1) {
					result = failed;
					fail++;
				}
				break;
			}
		}
		
		printf("test %d: \t%s\n", ++test, result);
		if(result == failed) {
			if(foa_error_set(&foa)) {
				printf("  error: %s\n", foa_last_error(&foa));
				foa_reset_error(&foa);
			} else if(ent) {
				printf("  error: %s\n", ent->data);
			}
		}
	}
	
	if(fail) {
		printf("\nsummary: %d test failed\n", fail);
	} else {
		printf("\nsummary: all test passed\n");
	}
	
	foa_cleanup(&foa);
	return fail;
}
