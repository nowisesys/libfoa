#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include "libfoa.h"

/* 
 * Example: decode_simple_object.
 * 
 * The encoded data in the example is equivalent to:
 * 
 * struct item
 * {
 *     int artnr;
 *     float price;
 *     const char *name;
 *     const char *desc;
 * };
 * 
 * struct item data = { 7464635, 8.50, "guistr_jd_10", "Guitar strings, Jim Dunlop, 0.10 inch" };
 * 
 * Note:
 * 
 * The data is anonymous, so it's expected that the decoder 
 * knows the the order between the members of the object to map
 * them to member variables.
 * 
 */

int main(void) 
{
	struct libfoa foa;
	const struct foa_entity *ent;
	
	const char *data = "(\n7464635\n8.50\nguistr_jd_10\nGuitar strings, Jim Dunlop, 0.10 inch\n)\n";
	
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
