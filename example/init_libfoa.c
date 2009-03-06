#include <stdio.h>
#include "libfoa.h"

/* 
 * Example: init_libfoa
 * 
 * Simple example of how to initilize libfoa.
 */

int main(void) 
{
	struct libfoa foa;
	
	if(foa_init(&foa) < 0) {
		fprintf(stderr, "failed initilize libfoa: %s\n", foa_last_error(&foa));
		return 1;
	}
	printf("libfoa initilized\n");
	
	foa_cleanup(&foa);
	printf("libfoa cleaned up\n");
	
	return 0;
}
