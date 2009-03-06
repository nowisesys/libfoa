#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "libfoa.h"
#include "libfoa-utils.h"

/* 
 * Example: decode_unordered_object
 * 
 * This is an example of decoding unordered data objects. See
 * note in decode_hashed_object_array. It encodes an array of
 * objects (with order of object members mixed) and then read
 * them back assigning decoded value to correct member.
 */

struct person
{
	const char *name;
	unsigned int age;
};

static void print_person(const char *id, const struct person *p) 
{
	printf("(%s): name: %s, age: %d\n", id, p->name, p->age);
}

int main(void) 
{
	struct libfoa foa;
	const struct foa_entity *ent;
	struct person p1 = { "adam", 35 };
	struct person p2 = { "bertil", 28 };
	struct person pd;  /* decoded person */
	char age[3], *buff;
	const char *pp;
	
	if(foa_init(&foa) < 0) {
		fprintf(stderr, "failed initilize libfoa: %s\n", foa_last_error(&foa));
		return 1;
	}
	
	print_person("p1", &p1);
	print_person("p2", &p2);

	/* 
	 * Encode the two persons:
	 */
	foa_start_array(&foa, 1);
	
	sprintf(age, "%d", p1.age);
	foa_start_object(&foa, 1);
	foa_write_data(&foa, "age", age, 1);
	foa_write_data(&foa, "name", p1.name, 1);
	foa_end_object(&foa, 1);
	
	sprintf(age, "%d", p2.age);
	foa_start_object(&foa, 1);
	foa_write_data(&foa, "name", p2.name, 1);
	foa_write_data(&foa, "age", age, 1);
	foa_end_object(&foa, 1);

	if(!(pp = foa_end_array(&foa, 1))) {
		fprintf(stderr, "error: %s\n", foa_last_error(&foa));
		return 1;
	}
	printf("encoded:\n%s\n", pp);
	buff = strdup(pp);
	
	/* 
	 * Decode the two persons:
	 */
	foa_set_buffer(&foa, buff);
	while((ent = foa_next(&foa))) {
		switch(ent->type) {
		case FOA_TYPE_START_OBJECT:
			pd.name = NULL;
			pd.age  = 0;
			break;
		case FOA_TYPE_END_OBJECT:
			print_person("pd", &pd);
			break;
		case FOA_TYPE_DATA_ENTITY:
			if(strcmp(ent->name, "name") == 0) {
				pd.name = strdup(ent->data);
			} else {
				pd.age = atoi(ent->data);
			}
		}
	}
	
	free(buff);
	free((char *)pd.name);
	
	foa_cleanup(&foa);
	return 0;
}
