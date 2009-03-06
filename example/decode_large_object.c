#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libfoa.h"

/* 
 * Example: decode_large_object
 * 
 * Dynamic generate an large object buffer and decode it to 
 * test performance.
 */

#define OBJECTS  20 * 1000 * 1000

static const char objstr[] = "(\nf1=v1\nf2=v2\nf3=v3\n)\n";

int main(int argc, char **argv) 
{
	struct libfoa foa;
	const struct foa_entity *ent;
	char *buff;
	unsigned long i, size, num = OBJECTS;
	size_t pos = 0;
	
	if(foa_init(&foa) < 0) {
		fprintf(stderr, "failed initilize libfoa: %s\n", foa_last_error(&foa));
		return 1;
	}
	
	if(argc > 1)
		num = atoi(argv[1]);

	size = strlen(objstr) * num + 13;
	buff = malloc(size);
	if(!buff) {
		fprintf(stderr, "failed alloc memory");
		return 1;
	}
	
	printf("initializing %lu objects (%lu bytes)... ", num, size);
	pos += sprintf(buff, "large = [\n");
	for(i = 0; i < num; ++i) {
		pos += sprintf(buff + pos, "%s", objstr);
	}
	pos += sprintf(buff + pos, "]\n");
	printf("done\n");
	
	printf("decoding memory object... ");
	fflush(stdout);
	foa_set_buffer(&foa, buff);
	while((ent = foa_next(&foa))) {
	}
	printf("done\n");
	
	free(buff);
	
	foa_cleanup(&foa);
	return 0;
}
