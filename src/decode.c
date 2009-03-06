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
#include <ctype.h>

#include "libfoa.h"
#include "internal.h"

/* 
 * Find first block of whitespace characters and eat them.
 */
static inline char * eat_white(char *str) 
{
	while((*str && isspace(*str)))  str--; str++;
	while((*str && isspace(*str))) *str++ = '\0';
	return str;
}

/* 
 * Decode the entity in foa->buff and update foa->entity. 
 * The buffer contains either:
 * 
 * 1. One of the special chars: ([])
 * 2. An single row of data (possibly named, see 3).
 * 3. Named data on form "name = data", where data itself can be 
 *    one of the special chars, i.e. "name = (". Note that strings 
 *    like "name = ]" is illegal.
 */
void decode_entity(struct libfoa *foa) 
{
	char *pp;

	foa->entity.line = foa->line;
	foa->entity.name = NULL;

	if(*foa->buff == '\n') {
		foa->entity.data = "unexpected empty line";
		foa->entity.type = FOA_TYPE_ERROR_MESSAGE;
		return;
	}
	
	if((pp = strchr(foa->buff, FOA_TYPE_NAME_DATA))) {
		/* 
		 * We got "name = data":
		 */
		if(foa->hashes != 1) {
			foa->entity.data = "named data is not allowed";
			foa->entity.type = FOA_TYPE_ERROR_MESSAGE;
			return;
		}
		
		*pp = ' ';
		
		foa->entity.name = foa->buff;
		foa->entity.data = eat_white(pp);
		
		if(!strchr(FOA_TYPE_SPEC_CHARS, *foa->entity.data))
			foa->entity.type = FOA_TYPE_DATA_ENTITY;
		else 
			foa->entity.type = *foa->entity.data;
	} else {
		/* 
		 * We got a single "data":
		 */
		foa->entity.data = foa->buff;
		foa->entity.type = FOA_TYPE_DATA_ENTITY;
	}
	
	if(strchr(FOA_TYPE_SPEC_CHARS, *foa->entity.data)) {
		/* 
		 * The buffer begins with a special char:
		 */
		if(strlen(foa->entity.data) != 1) {
			if(strchr(FOA_TYPE_SPEC_CHARS, foa->entity.data[1])) {
				foa->entity.data = "multiple special chars without newline separator";
				foa->entity.type = FOA_TYPE_ERROR_MESSAGE;
				return;
			} else if(foa->entity.data[1] != '\n') {
				foa->entity.data = "garbage character after special char";
				foa->entity.type = FOA_TYPE_ERROR_MESSAGE;
				return;
			}
		} else {
			foa->entity.data = "special chars without trailing newline";
			foa->entity.type = FOA_TYPE_ERROR_MESSAGE;
			return;
		}
		foa->entity.type = *foa->buff;
	}

	if((pp = strchr(foa->entity.data, '\n'))) *pp = '\0';
}
