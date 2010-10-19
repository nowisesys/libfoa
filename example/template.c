#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include "libfoa.h"

/* 
 * Example template:
 * Description...
 */

int main(void) 
{
	struct libfoa foa;
	
	if(foa_init(&foa) < 0) {
		fprintf(stderr, "failed initilize libfoa: %s\n", foa_last_error(&foa));
		return 1;
	}
	
	foa_cleanup(&foa);
	return 0;
}
