/* Library implementing FOA - Fast Object and Array encoding.
 * Copyright (C) 2009 Anders Lövgren
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "libfoa.h"
#include "internal.h"

/*
 * Initilize library.
 */
int foa_init(struct libfoa *foa)
{
	memset(foa, 0, sizeof(struct libfoa));
	
	foa->size = FOA_MEMORY_ALLOC_INIT;
	foa->step = FOA_MEMORY_ALLOC_STEP;
	foa->max  = FOA_MEMORY_ALLOC_MAX;
	foa->buff = malloc(foa->size);
	if(!foa->buff) {
		write_errlog(&foa->errmsg, errno, "failed alloc memory");
		return -1;
	}
	foa->escape = FOA_ENABLE_ESCAPE;
	foa->hashes = FOA_ENABLE_HASHES;
	
	return 0;
}

/* 
 * Cleanup after finished.
 */
void foa_cleanup(struct libfoa *foa)
{
	free(foa->buff);
	free(foa->errmsg);
}

/* 
 * Define memory allocation strategy. The step variable defines how
 * aggressive memory is allocated. The max variable sets a maximum
 * buffer size in bytes (use 0 for unlimited). Returns 0 if successful
 * and -1 on error.
 */
int foa_alloc_strategy(struct libfoa *foa, size_t step, size_t max)
{
	foa->step = step;
	foa->max  = max;
	if(foa->max < foa->used) {
		foa->buff = realloc(foa->buff, foa->max);
		if(!foa->buff) {
			write_errlog(&foa->errmsg, errno, "failed alloc memory");
			return -1;
		}
		foa->used = foa->max;
	}
	return 0;
}

/* 
 * Use callback function to handle data seen. The optional arg get
 * passed along together with each call to func.
 */
void foa_set_callback(struct libfoa *foa, foa_handler func, void *arg)
{
	foa->func = func;
	foa->arg  = arg;
}

/* 
 * Set file stream for reading (parsing) and writing (encoded result).
 */
void foa_set_stream(struct libfoa *foa, FILE *file)
{
	foa->line = 1;
	foa->used = 0;
	foa->curr = foa->next = NULL;
	foa->file = file;
}

/* 
 * Set input buffer for memory parsing.
 */
void foa_set_buffer(struct libfoa *foa, const char *input)
{
	foa->line = 1;
	foa->used = 0;
	foa->file = NULL;
	foa->curr = input;
	foa->next = strchr(foa->curr, '\n');	
}

/* 
 * Set current mode.
 */
void foa_set_mode(struct libfoa *foa, int mode, int enable)
{
	switch(mode) {
	case FOA_MODE_ESCAPE:
		foa->escape = enable == 0 ? 0 : 1;
		break;
	case FOA_MODE_HASHES:
		foa->hashes = enable == 0 ? 0 : 1;
		break;
	}
}
	
/* 
 * Get current mode.
 */
void foa_get_mode(struct libfoa *foa, int mode, int *enable)
{
	switch(mode) {
	case FOA_MODE_ESCAPE:
		*enable = foa->escape;
		break;
	case FOA_MODE_HASHES:
		*enable = foa->hashes;
		break;
	}
}

/* 
 * Process the input buffer or stream. This calls the registered callback 
 * function for each type (data or special character) seen.
 */
int foa_scan(struct libfoa *foa)
{
	if(!foa->func) {
		logerr(&foa->errmsg, 0, "no callback function defined");
		return -1;
	}
	if(!foa->curr && !foa->file) {
		logerr(&foa->errmsg, 0, "no input stream or memory buffer is set");
		return -1;
	}
	
	if(foa->file) {
		while(!feof(foa->file)) {
			if(read_stream(foa) < 0) {
				return feof(foa->file) ? 0 : -1;
			}
			if(!foa->func(&foa->entity, foa->arg)) {
				return -1;
			}
		}
	} else {
		while(foa->curr) {
			if(read_memory(foa) < 0) {
				return -1;
			}
			if(!foa->func(&foa->entity, foa->arg)) {
				return -1;
			}
		}
	}
	return 0;
}

/* 
 * Process the input buffer iterative. This is an alternative to
 * using the foa_scan() function.
 */
const struct foa_entity * foa_next(struct libfoa *foa)
{
	if(foa->file) {
		if(feof(foa->file)) {
			return NULL;
		}
		if(read_stream(foa) < 0) {
			return NULL;
		}
	} else if(foa->curr) {
		if(read_memory(foa) < 0) {
			return NULL;
		}
	} else {
		if(!foa->curr && foa->line != 1) {
			return NULL;  /* end of buffer */
		}
		logerr(&foa->errmsg, 0, "input stream or memory is unset");
		return NULL;
	}
	return &foa->entity;
}

/* 
 * Clear stored error message.
 */
void foa_reset_error(struct libfoa *foa)
{
	free(foa->errmsg);
	foa->errmsg = NULL;
}

/* 
 * Specialized for writing anonymous string data, that is, array of 
 * const char * where last element must be NULL. This function may 
 * cause the maximum alloc limit to be reached for the memory buffer 
 * if used in append mode.
 */
const char * foa_write_array(struct libfoa *foa, const char **data, int append)
{
	const char *res;
	
	res = foa_write(foa, append, FOA_TYPE_START_ARRAY, NULL, NULL);
	if(!res) 
		return NULL;
	
	while(*data) {
		res = foa_write(foa, append, FOA_TYPE_DATA_ENTITY, NULL, *(data++));
		if(!res)
			return NULL;
	}
	res = foa_write(foa, append, FOA_TYPE_END_ARRAY, NULL, NULL);
	if(!res)
		return NULL;
	
	return foa->buff;
}
