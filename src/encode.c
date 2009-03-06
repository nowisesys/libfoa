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
#include "config.h"
#endif

#include <stdio.h>
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
# include <string.h>
#elif HAVE_STRINGS_H
# include <strings.h>
#endif

#include "libfoa.h"
#include "internal.h"

/* 
 * Write data to memory buffer or file stream. If append is true,
 * then data is appended (only for memory buffer), otherwise the
 * memory buffer is overwritten. The type is one of FOA_TYPE_XXX.
 * Returns the encoded string on success and NULL on error.
 */
const char * foa_write(struct libfoa *foa, int append, int type, 
		       const char *name, const char *data)
{
	size_t need = 1;      /* required size */
	size_t puts = 0;      /* requested number of bytes */
	int doesc = 0;        /* is escaping required? */
	const char *pp;
	
	if(name) {
		if(!foa->hashes) {
			logerr(&foa->errmsg, 0, "named data is not allowed");
			return NULL;
		}
		if(strchr(name, FOA_TYPE_NAME_DATA)) {
			logerr(&foa->errmsg, 0, "'%c' is not allowed inside name", 
			       FOA_TYPE_NAME_DATA);
			return NULL;
		}
		need += strlen(name) + 3;    /* 'name = ' */
	}
	
	switch(type) {
	case FOA_TYPE_START_OBJECT:
	case FOA_TYPE_START_ARRAY:
	case FOA_TYPE_END_OBJECT:
	case FOA_TYPE_END_ARRAY:
		need += 1;
		break;
	case FOA_TYPE_DATA_ENTITY:
		if(!data) {
			logerr(&foa->errmsg, 0, "missing data argument");
			return NULL;
		}
		need += strlen(data);
		if(strpbrk(data, FOA_TYPE_SPEC_CHARS) != NULL ||
		   strchr(data,  FOA_TYPE_NAME_DATA)  != NULL) {
			doesc = 1;
		}
		if(doesc) {
			for(pp = data; *pp; ++pp) {
				if(foa->escape && 
				   (*pp == FOA_TYPE_START_OBJECT ||
				    *pp == FOA_TYPE_START_ARRAY ||
				    *pp == FOA_TYPE_END_OBJECT ||
				    *pp == FOA_TYPE_END_ARRAY ||
				    *pp == FOA_TYPE_NAME_DATA)) {
					need += 2;
				}
			}
		}
		break;
	default:
		logerr(&foa->errmsg, 0, "invalid encode type: %d", type);
		return NULL;
	}

	if(!append) {
		foa->buff[0] = '\0';
		foa->used = 0;
	}
	puts = foa->used + need;
	
	if(need + foa->used > foa->max && foa->max != 0) {
		logerr(&foa->errmsg, 0, "maximum buffer size reached");
		return NULL;
	}
	if(need + foa->used > foa->size) {
		if(need < foa->step) 
			foa->size += foa->step;
		else
			foa->size += need + 1;
		foa->buff = realloc(foa->buff, foa->size);
	}

	if(name) {
		foa->used += sprintf(foa->buff + foa->used,
				     "%s %c ", 
				     name, 
				     FOA_TYPE_NAME_DATA);
	}
	if(need == 2) {      /* one of ([]) */
		foa->used += sprintf(foa->buff + foa->used, "%c", type);
	} else {
		if(!doesc || (!foa->escape && !foa->hashes)) {
			foa->used += sprintf(foa->buff + foa->used, "%s", data);
		} else {
			for(pp = data; *pp; ++pp) {
				if(foa->escape && 
				   (*pp == FOA_TYPE_START_OBJECT ||
				    *pp == FOA_TYPE_START_ARRAY ||
				    *pp == FOA_TYPE_END_OBJECT ||
				    *pp == FOA_TYPE_END_ARRAY || 
				    *pp == FOA_TYPE_NAME_DATA)) {
					foa->used += sprintf(foa->buff + foa->used, 
							     "%%%X", 
							     *pp);
				} else {
					foa->used += sprintf(foa->buff + foa->used,
							     "%c", *pp);
				}
			}
		}
	}
	foa->used += sprintf(foa->buff + foa->used, "\n");
	
	if(puts != foa->used) {
		logerr(&foa->errmsg, 0, "failed write %lu bytes", need);
		return NULL;
	}

	if(foa->file) 
		fprintf(foa->file, "%s", foa->buff);
	
	return foa->buff;
}
