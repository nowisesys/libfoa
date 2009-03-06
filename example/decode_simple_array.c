#include <stdio.h>
#include "libfoa.h"

/*
 * Example: decode_simple_array.
 * 
 * This is an example of how to decode the FOA equivalent of:
 * 
 * const char *names[] = { "adam", "bertil", "ceasar", david" };
 * 
 */

int main(void) 
{
	struct libfoa foa;
	const struct foa_entity *ent;
	
	const char *data = "[\nadam\nbertil\nceasar\ndavid\n]\n";
	
	if(foa_init(&foa) < 0) {
		fprintf(stderr, "failed initilize libfoa\n");
		return 1;
	}
	
	foa_set_buffer(&foa, data);
	while((ent = foa_next(&foa))) {
		switch(ent->type) {
		case FOA_TYPE_DATA_ENTITY:
			printf("name: %s\n", ent->data);
			break;
		default:
			printf("spec: %s\n", ent->data);
			break;
		}
	}
	
	foa_cleanup(&foa);
	return 0;
}
