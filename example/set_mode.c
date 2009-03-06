#include <stdio.h>
#include "libfoa.h"

/* 
 * Example: set_mode
 * 
 * Test set mode (enable/disable escape and hashes) using foa_set_mode().
 */

static void report_mode(const char *label, const struct libfoa *foa)	
{
	printf("* %s:\n", label);
	printf("  escape: %d, hashes: %d\n", foa->escape, foa->hashes);
}

int main(void) 
{
	struct libfoa foa;
	int mode;
	
	if(foa_init(&foa) < 0) {
		fprintf(stderr, "failed initilize libfoa: %s\n", foa_last_error(&foa));
		return 1;
	}
	
	report_mode("default", &foa);
	
	printf("\n");
	printf("testing foa_set_mode():\n");
	printf("-----------------------\n");
	
	foa_set_mode(&foa, FOA_MODE_ESCAPE, 0);
	report_mode("disable escape", &foa);
	foa_set_mode(&foa, FOA_MODE_HASHES, 0);
	report_mode("disable hashes", &foa);
	foa_set_mode(&foa, FOA_MODE_ESCAPE, 1);
	report_mode("enable escape", &foa);
	foa_set_mode(&foa, FOA_MODE_HASHES, 1);
	report_mode("enable hashes", &foa);

	printf("\n");
	printf("testing foa_get_mode():\n");
	printf("-----------------------\n");
	
	mode = 0;
	foa_get_mode(&foa, FOA_MODE_ESCAPE, &mode);
	printf("* foa_get_mode(): escape: %d\n", mode);
	mode = 0;
	foa_get_mode(&foa, FOA_MODE_HASHES, &mode);
	printf("* foa_get_mode(): hashes: %d\n", mode);
	
	foa_cleanup(&foa);
	return 0;
}
