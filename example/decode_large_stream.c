#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include "libfoa.h"

/* 
 * Example: decode data from file stream.
 * 
 * This example demonstrate reading encoded data from a stream
 * and decoding it. The encoded data is generated and written
 * to a temporary file before, then the file stream is rewinded
 * and the encoded data is decoded.
 */

int main(int argc, char **argv) 
{
	struct libfoa foa;
	const struct foa_entity *ent;
	FILE *fs;
	int num, i;
	
	if(foa_init(&foa) < 0) {
		fprintf(stderr, "failed initilize libfoa: %s\n", foa_last_error(&foa));
		return 1;
	}
	
	if(argc != 1) 
		num = atoi(argv[1]);
	else
		num = 1000000;
	
	if((fs = tmpfile()) == NULL) {
		fprintf(stderr, "failed open temporary file\n");
		return 1;
	}
	
	printf("filling temp file with %d encoded objects... ", num);
	fflush(stdout);
	fprintf(fs, "[\n");
	for(i = 0; i < num; ++i) {
		fprintf(fs, "(\nname=adam\nage=35\n)\n");
	}
	fprintf(fs, "]\n");
	rewind(fs);
	printf("done\n");
	
	foa_set_stream(&foa, fs);
	printf("decoding object file stream... ");
	fflush(stdout);
	while((ent = foa_next(&foa))) {
	}
	printf("done\n");

	printf("cleaning up... ");
	fflush(stdout);	
	fclose(fs);
	foa_cleanup(&foa);	
	printf("done\n");
	
	return 0;
}
