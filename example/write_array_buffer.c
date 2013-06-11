#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include "libfoa.h"
#include "libfoa-utils.h"

/* 
 * Example: write_array_buffer
 * 
 * Test write an array of const char * to a memory buffer.
 */

int main(void) 
{
	struct libfoa foa;
	const char *data[] = {
		"adam", "bertil", "ceasar", "david", "erik", "fredrik", 
		"gunnar", "henrik", "isak", "johan", "karl", "lars",
		"martin", "niklas", "oskar", "peter", "quintus", "robert",
		"sixten", "torbjörn", "urban", "viktor", "xerxes", "yngve",
		NULL 
	};
	const char *res;
	
	if(foa_init(&foa) < 0) {
		fprintf(stderr, "failed initilize libfoa: %s\n", foa_last_error(&foa));
		return 1;
	}
	
	res = foa_write_array(&foa, data, 1);  /* write buffer in append mode */
	if(!res) {
		fprintf(stderr, "failed encode array: %s\n", foa_last_error(&foa));
		return 1;
	}
	printf("result:\n");
	printf("-------\n");
	printf("%s", res);
	
	foa_cleanup(&foa);
	return 0;
}
