#include <stdio.h>
#include "libfoa.h"
#include "libfoa-utils.h"

/* 
 * Example: encode_buffer
 * 
 * Simple example showing how to encode an object with named member
 * variables in the memory buffer. This example is equivalent to
 * encoding:
 * 
 * struct person 
 * {
 *     const char *fname;    // first name
 *     const char *lname;    // last name
 * };
 * 
 * struct person myself = { "Anders", "Lövgren" };
 */

int main(void)
{
	struct libfoa foa;
	
	if(foa_init(&foa) < 0) {
		fprintf(stderr, "failed initilize libfoa: %s\n", foa_last_error(&foa));
		return 1;
	}
	
	printf("%s", foa_start_object(&foa, 0));
	printf("%s", foa_write_data(&foa, "fname", "Anders", 0));
	printf("%s", foa_write_data(&foa, "lname", "Lövgren", 0));
	printf("%s", foa_end_object(&foa, 0));
	
	if(foa_error_set(&foa)) {
		printf("%s\n", foa_last_error(&foa));
		return 1;
	}
	
	foa_cleanup(&foa);
	return 0;
}
