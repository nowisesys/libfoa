#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include "libfoa.h"
#include "libfoa-utils.h"

/* 
 * Example: encode_named_object
 * 
 * Simple example showing how to encode an named object with member
 * variables in the memory buffer.
 */

int main(void)
{
	struct libfoa foa;
	
	if(foa_init(&foa) < 0) {
		fprintf(stderr, "failed initilize libfoa: %s\n", foa_last_error(&foa));
		return 1;
	}
	
	printf("%s", foa_write(&foa, 0, FOA_TYPE_START_OBJECT, "person", NULL));
	printf("%s", foa_write(&foa, 0, FOA_TYPE_DATA_ENTITY, NULL, "Anders"));
	printf("%s", foa_write(&foa, 0, FOA_TYPE_DATA_ENTITY, NULL, "Lövgren"));
	printf("%s", foa_write(&foa, 0, FOA_TYPE_END_OBJECT, NULL, NULL));
	
	if(foa_error_set(&foa)) {
		printf("%s\n", foa_last_error(&foa));
		return 1;
	}
	
	foa_cleanup(&foa);
	return 0;
}
