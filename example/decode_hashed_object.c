#include <stdio.h>
#include "libfoa.h"

/* 
 * This example demonstrate decoding an array of objects. Consider:
 * 
 * struct person 
 * {
 *     const char *name;
 *     unsigned int age;
 * }
 * 
 * Then data in the example is the encoded form of a single object:
 * 
 * struct person data = { "adam", 52 };
 * 
 * The decoder knows by ent->name how to map the decoded data to the
 * variables in the object.
 */

int main(void) 
{
	struct libfoa foa;
	const struct foa_entity *ent;
	
	const char *data = "(\nname = adam\nage = 52\n)\n";
	
	if(foa_init(&foa) < 0) {
		fprintf(stderr, "failed initilize libfoa\n");
		return 1;
	}
	
	foa_set_buffer(&foa, data);
	while((ent = foa_next(&foa))) {
		switch(ent->type) {
		case FOA_TYPE_DATA_ENTITY:
			printf("%s: %s\n", ent->name, ent->data);
			break;
		default:
			printf("spec: %s\n", ent->data);
			break;
		}
	}
	
	foa_cleanup(&foa);
	return 0;
}
