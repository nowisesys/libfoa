/* Library implementing FOA - Fast Object and Array encoding.
 * Copyright (C) 2009-2019 Anders Lövgren
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

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
# include <string.h>
#elif HAVE_STRINGS_H
# include <strings.h>
#endif

#include <errno.h>
#include <ctype.h>

#include "libfoa.h"
#include "internal.h"

#if defined(WIN32) && defined(_BCC55)
# ifdef FOA_API_INLINE
#  undef FOA_API_INLINE
# endif
# define FOA_API_INLINE
#endif

struct http_encode
{
	const char *str;
	char rep;
};

/* 
 * Find first block of whitespace characters and eat them.
 */
FOA_API_INLINE char * eat_white(char *str) 
{
	while((*str && isspace(*str)))  str--; str++;
	while((*str && isspace(*str))) *str++ = '\0';
	return str;
}

/* 
 * Get unescaped character from HTTP encoded character. The str
 * is pointing to at start of an '%NN' substring. Returns the
 * unescaped character or 0 if no matching unescaped character 
 * exists.
 */
static char get_unescaped_char(const char *str)
{
	static struct http_encode encode[] = {
		{ FOA_ENCODE_BEGIN_OBJECT, FOA_TYPE_START_OBJECT },
		{ FOA_ENCODE_BEGIN_ARRAY,  FOA_TYPE_START_ARRAY },
		{ FOA_ENCODE_END_OBJECT,   FOA_TYPE_END_OBJECT },
		{ FOA_ENCODE_END_ARRAY,    FOA_TYPE_END_ARRAY },
		{ FOA_ENCODE_NAME_DATA,    FOA_TYPE_NAME_DATA },
		{ NULL, 0 }
	};
	struct http_encode *ptr;
	
	for(ptr = encode; ptr->str; ++ptr) {
		if(strncmp(ptr->str, str, 3) == 0) {
			break;
		}
	}
	return ptr->rep;
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
 * 
 * The data might be escaped, that is, all '([])=' might have been
 * encoded as HTTP encoded characters.
 */
void decode_entity(struct libfoa *foa) 
{
	char *pp;

	foa->entity.line = foa->line;
	foa->entity.name = NULL;
	foa->entity.type = 0;

	if(*foa->buff == '\n') {
		foa->entity.data = "unexpected empty line";
		foa->entity.type = FOA_TYPE_ERROR_MESSAGE;
		return;
	}
	
	if((pp = strchr(foa->buff, FOA_TYPE_NAME_DATA))) {
		/* 
		 * We got "name = data":
		 */
		if(!foa->hashes) {
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
		if(!foa->entity.type) {
			foa->entity.type = *foa->buff;
		}
	}
	
	if(foa->escape && foa->entity.type == FOA_TYPE_DATA_ENTITY) {
		if(strchr(foa->entity.data, '%')) {
			/* 
			 * We might have at least one escaped character. 
			 */
			char enc;
			for(pp = (char *)foa->entity.data; pp; pp = strchr(pp + 1, '%')) {
				if((enc = get_unescaped_char(pp)) != 0) {
					*pp = enc;
					memmove(pp + 1, pp + 3, strlen(pp));
				}
			}
		}
	}

	if((pp = strchr(foa->entity.data, '\n'))) *pp = '\0';
}
