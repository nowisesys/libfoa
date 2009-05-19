#include <stdio.h>
#include "libfoa.h"

/* 
 * Example: decode_named_object
 * 
 * This example demonstrate decoding an named object.
 */

int main(void) 
{
	struct libfoa foa;
	const struct foa_entity *ent;
	
	const char *data = "object = (\npersons = [\nname = Adam\nname = Bertil\n]\n)\n";
	
	if(foa_init(&foa) < 0) {
		fprintf(stderr, "failed initilize libfoa\n");
		return 1;
	}
	
	foa_set_buffer(&foa, data);
	while((ent = foa_next(&foa))) {
		switch(ent->type) {
		case FOA_TYPE_DATA_ENTITY:
			printf("[type=%d] %s: %s\n", ent->type, ent->name, ent->data);
			break;
		default:
			if(ent->name) {
				printf("[type=%d] spec: %s: %s\n", ent->type, ent->name, ent->data);
			} else {
				printf("[type=%d] spec: %s\n", ent->type, ent->data);
			}
			break;
		}
	}
	
	foa_cleanup(&foa);
	return 0;
}
