#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include "libfoa.h"
#include "libfoa-utils.h"

/* 
 * Example: encode_object
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
 * struct person myself = { "John", "Smith" };
 */

int main(void)
{
	struct libfoa foa;
	
	if(foa_init(&foa) < 0) {
		fprintf(stderr, "failed initilize libfoa: %s\n", foa_last_error(&foa));
		return 1;
	}
	
	printf("%s", foa_start_object(&foa, 0));
	printf("%s", foa_write_data(&foa, "fname", "John", 0));
	printf("%s", foa_write_data(&foa, "lname", "Smith", 0));
	printf("%s", foa_end_object(&foa, 0));
	
	if(foa_error_set(&foa)) {
		printf("%s\n", foa_last_error(&foa));
		return 1;
	}
	
	foa_cleanup(&foa);
	return 0;
}
