#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include "libfoa.h"

/* 
 * Example: decode_hashed_object_array.
 * 
 * This example demonstrate decoding an array of objects. Consider:
 * 
 * struct person 
 * {
 *     const char *name;
 *     unsigned int age;
 * }
 * 
 * Then data in the example is the encoded form of:
 * 
 * struct person data[] = {
 *     { "adam",   52 },
 *     { "bertil", 38 }
 * };
 * 
 * Note:
 * 
 * Because the variable names (ent->name) is encoded, it is possible 
 * to recover the data (map entity data to object members) even if the
 * data is encoded in mixed order (switching order between name and age).
 */

int main(void) 
{
	struct libfoa foa;
	const struct foa_entity *ent;
	
	const char *array = "[\n(\nname = adam\nage = 52\n)\n(\nname = bertil\nage = 38\n)\n]\n";
	
	if(foa_init(&foa) < 0) {
		fprintf(stderr, "failed initilize libfoa\n");
		return 1;
	}
	
	foa_set_buffer(&foa, array);
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
