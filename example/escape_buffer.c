#include <stdio.h>
#include "libfoa.h"
#include "libfoa-utils.h"

/* 
 * Example: escape_buffer
 * 
 * Test escaping of special chars in input buffer.
 */

int main(void) 
{
	struct libfoa foa;
	const struct foa_entity *ent;
	const char *res;
	const char *str1 = "a[b]c(d)";
	const char *str2 = "name = a[b]c(d)";
	const char *str3 = "a%5Bb%5Dc%28d%29e%3D";
	const char *str4 = "%10%5B%11%5D%12%28%13%29%14%3D";
	const char *str5 = "name = a%5Bb%5Dc%28d%29e%3D";
	const char *str6 = "name %3D a%5Bb%5Dc%28d%29e%3D";
	
	if(foa_init(&foa) < 0) {
		fprintf(stderr, "failed initilize libfoa: %s\n", foa_last_error(&foa));
		return 1;
	}
	
	printf("encoding (escaping) data:\n");
	printf("-------------------------\n");
	foa_enable_escape(&foa, 0);
	printf("%s -> %s (escape off)\n", str1, foa_write_data(&foa, NULL, str1, 0));
	foa_enable_escape(&foa, 1);
	printf("%s -> %s (escape on)\n", str1, foa_write_data(&foa, NULL, str1, 0));

	foa_enable_escape(&foa, 0);
	printf("%s -> %s (escape off)\n", str2, foa_write_data(&foa, NULL, str2, 0));
	foa_enable_escape(&foa, 1);
	printf("%s -> %s (escape on)\n", str2, foa_write_data(&foa, NULL, str2, 0));

	foa_enable_escape(&foa, 0);
	printf("%s -> %s (escape off)\n", str1, foa_write_data(&foa, "name", str1, 0));
	foa_enable_escape(&foa, 1);
	printf("%s -> %s (escape on)\n", str1, foa_write_data(&foa, "name", str1, 0));
	
	foa_enable_hashes(&foa, 0);
	res = foa_write_data(&foa, "name", str1, 0);
	if(res)
		printf("%s -> %s (hashes off)\n", str1, res);
	else
		printf("%s: error: %s\n (hashes off)\n", str1, foa_last_error(&foa));
	
	foa_enable_hashes(&foa, 1);
	res = foa_write_data(&foa, "name", str1, 0);
	if(res)
		printf("%s -> %s (hashes on)\n", str1, res);
	else
		printf("%s: error: %s\n (hashes on)\n", str1, foa_last_error(&foa));

	printf("\n");
	printf("decoding (escaped) data:\n");
	printf("-------------------------\n");
	foa_enable_escape(&foa, 1);
	foa_set_buffer(&foa, str3);
	while((ent = foa_next(&foa))) {
		if(ent->name)
			printf("%s -> %s = %s\n (escape on)\n", str3, ent->name, ent->data);
		else
			printf("%s -> %s\n (escape on)\n", str3, ent->data);
	}
	foa_set_buffer(&foa, str4);
	while((ent = foa_next(&foa))) {
		if(ent->name)
			printf("%s -> %s = %s\n (escape on)\n", str4, ent->name, ent->data);
		else
			printf("%s -> %s\n (escape on)\n", str4, ent->data);
	}
	foa_enable_escape(&foa, 0);
	foa_set_buffer(&foa, str3);
	while((ent = foa_next(&foa))) {
		if(ent->name)
			printf("%s -> %s = %s\n (escape off)\n", str3, ent->name, ent->data);
		else
			printf("%s -> %s\n (escape off)\n", str3, ent->data);
	}
	foa_enable_escape(&foa, 1);
	foa_set_buffer(&foa, str5);
	while((ent = foa_next(&foa))) {
		if(ent->name)
			printf("%s -> %s = %s\n (escape on)\n", str5, ent->name, ent->data);
		else
			printf("%s -> %s\n (escape on)\n", str5, ent->data);
	}
	foa_enable_hashes(&foa, 0);
	foa_set_buffer(&foa, str5);
	while((ent = foa_next(&foa))) {
		if(ent->type == FOA_TYPE_ERROR_MESSAGE) {
			printf("%s: error: %s\n (hashes off)\n", str5, ent->data);
		} else {
			if(ent->name)
				printf("%s -> %s = %s\n (hashes off)\n", str5, ent->name, ent->data);
			else
				printf("%s -> %s\n (hashes off)\n", str5, ent->data);
		}
	}
	foa_enable_hashes(&foa, 1);
	foa_set_buffer(&foa, str6);
	while((ent = foa_next(&foa))) {
		if(ent->name)
			printf("%s -> %s = %s\n (escape on)\n", str6, ent->name, ent->data);
		else
			printf("%s -> %s\n (escape on)\n", str6, ent->data);
	}
	
	foa_cleanup(&foa);
	return 0;
}
