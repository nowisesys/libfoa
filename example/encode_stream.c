#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include "libfoa.h"
#include "libfoa-utils.h"

/* 
 * Example: encode_buffer
 * 
 * Simple example showing how to encode in memory buffer. Each call to
 * foa_xxx() inside printf() formats the data, write it to the internal
 * buffer and flushes it to the file stream. This mode should be fairly
 * efficient and require very little allocated memory.
 */

int main(void)
{
	struct libfoa foa;
	
	if(foa_init(&foa) < 0) {
		fprintf(stderr, "failed initilize libfoa: %s\n", foa_last_error(&foa));
		return 1;
	}
	foa_set_stream(&foa, stdout);   /* set output stream */
	
	foa_start_object(&foa, 0);
	foa_write_data(&foa, NULL, "John", 0);
	foa_write_data(&foa, NULL, "Smith", 0);
	foa_end_object(&foa, 0);
	
	if(foa_error_set(&foa)) {
		printf("%s\n", foa_last_error(&foa));
		return 1;
	}
	
	foa_cleanup(&foa);
	return 0;
}
