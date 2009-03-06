#include <stdio.h>
#include <stdlib.h>
#include "libfoa.h"

/* 
 * The encoded example data is equivalent to:
 * 
 * const char *adam = "32";
 * const char *bertil = "52";
 * const char *ceasar = "15";
 * const char *david = "38";
 * 
 * const char *data[] = { adam, bertil, ceasar, david };
 * 
 * The C-language is missing the equivalent of map (C++), hashed 
 * array (PHP) or hashes (Perl). In FOA, the name in "name = data" 
 * is used to denote the variable name and the data is representing
 * the value of that variable.
 */

int main(void) 
{
	struct libfoa foa;
	const struct foa_entity *ent;
	
	const char *data = "[\nadam = 32\nbertil = 52\nceasar = 15\ndavid = 38\n]\n";
	
	if(foa_init(&foa) < 0) {
		fprintf(stderr, "failed initilize libfoa\n");
		return 1;
	}
	
	foa_set_buffer(&foa, data);
	while((ent = foa_next(&foa))) {
		switch(ent->type) {
		case FOA_TYPE_DATA_ENTITY:
			printf("name: %s, age: %d\n", ent->name, atoi(ent->data));
			break;
		default:
			printf("spec: %s\n", ent->data);
			break;
		}
	}
	
	foa_cleanup(&foa);
	return 0;
}
