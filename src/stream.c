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
 * Read next entity from the input stream. Returns -1 on error and
 * 0 if successful.
 */
int read_stream(struct libfoa *foa)
{
	register char *put, *res;
		
	for(put = foa->buff;; put += foa->step) {
		res = fgets(put, foa->size - 1, foa->file);
		if(!res) {
			if(!feof(foa->file))
				logerr(&foa->errmsg, errno, "failed read stream");
			return -1;
		}
		if(strchr(res, '\n')) 
			break;
		if(foa->size + foa->step > foa->max) {
			logerr(&foa->errmsg, 0, "maximum buffer size reached");
			return -1;
		}
		foa->size += foa->step;
		foa->buff = realloc(foa->buff, foa->size);
		if(!foa->buff) {
			logerr(&foa->errmsg, errno, "failed alloc memory");
			foa->size = 0;
			return -1;
		}
		printf("realloc: %lu bytes\n", foa->size);
	}
	
	decode_entity(foa);
	foa->line++;
	
	return 0;
}
