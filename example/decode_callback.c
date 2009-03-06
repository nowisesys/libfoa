#include <stdio.h>
#include "libfoa.h"

/* 
 * Example: decode_callback
 * 
 * Demonstrates how to use a callback function (foa_handler) to 
 * handle decoded data.
 */

int callback(const struct foa_entity *ent, void *data)
{
	const char *args = (const char *)data;
	
	if(ent->type == FOA_TYPE_ERROR_MESSAGE) {
		fprintf(stderr, "error: %s\n", ent->data);
		return 0;
	}
	if(ent->name)
		printf("%s %s = %-15s\t(type: %d)\n", args, ent->name, ent->data, ent->type);
	else
		printf("%s %-20s\t(type: %d)\n", args, ent->data, ent->type);
	
	return 1;   /* continue */
}

int main(int argc, char **argv) 
{
	struct libfoa foa;
	const char *file;
	const char *data = "<args>: ";    /* callback function parameter */
	FILE *fs;
	
	if(argc != 1) 
		file = argv[1];
	else 
		file = "data.txt";
	
	fs = fopen(file, "r");
	if(!fs) {
		fprintf(stderr, "failed open %s for reading\n", file);
		return 1;
	}
	
	if(foa_init(&foa) < 0) {
		fprintf(stderr, "failed initilize libfoa: %s\n", foa_last_error(&foa));
		return 1;
	}
	
	foa_set_stream(&foa, fs);
	foa_set_callback(&foa, callback, (void *)data);
	if(foa_scan(&foa) < 0) {
		fprintf(stderr, "failed scan data: %s\n", foa_last_error(&foa));
	}
	
	foa_cleanup(&foa);
	fclose(fs);
	
	return 0;
}
