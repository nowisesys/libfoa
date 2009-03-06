#include <stdio.h>
#include "libfoa.h"

/* 
 * Example: decode data from file stream.
 * 
 * This example demonstrate reading encoded data from a stream
 * and decoding it.
 */

int main(int argc, char **argv) 
{
	struct libfoa foa;
	const struct foa_entity *ent;
	const char *file;
	FILE *fs;
	
	if(foa_init(&foa) < 0) {
		fprintf(stderr, "failed initilize libfoa: %s\n", foa_last_error(&foa));
		return 1;
	}
	
	if(argc > 1) {
		file = argv[1];
	} else {
		file = "data.txt";
	}
	
	fs = fopen(file, "r");
	if(!fs) {
		fprintf(stderr, "failed open %s for reading\n", file);
		return 1;
	}

	foa_set_stream(&foa, fs);
	while((ent = foa_next(&foa))) {
		switch(ent->type) {
		case FOA_TYPE_DATA_ENTITY:
			if(ent->name)
				printf("%s: %s\n", ent->name, ent->data);
			else
				printf("data: %s\n", ent->data);
			break;
		default:
			printf("spec: %s\n", ent->data);
			break;
		}		
	}
	
	fclose(fs);
	foa_cleanup(&foa);
	
	return 0;
}
