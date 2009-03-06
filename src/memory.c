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
 * Read next entity from the input buffer. Returns -1 on error or
 * if end of buffer is found. Use foa->curr to detect if end of
 * buffer was reached.
 */
int read_memory(struct libfoa *foa)
{
	size_t need = 0;

	if(!foa->curr || *foa->curr == '\0')
		return -1;
	if(foa->next) 
		if(*foa->next == '\n')
			foa->next++;
	
	if(foa->next)
		need = foa->next - foa->curr;
	else 
		need = strlen(foa->curr);
	
	if(need > foa->size) {
		if(need > foa->max && foa->max != 0) {
			logerr(&foa->errmsg, 0, "maximum buffer size reached");
			return -1;
		}
		if(need < foa->step)
			foa->size += foa->step;
		else
			foa->size = need + 1;
		foa->buff = realloc(foa->buff, foa->size);
		if(!foa->buff) {
			logerr(&foa->errmsg, errno, "failed alloc memory");
			return -1;
		}
	}
	memcpy(foa->buff, foa->curr, need);
	foa->buff[need] = '\0';
	
	if((foa->curr = foa->next)) 
		foa->next = strchr(foa->curr + 1, '\n');
	
	decode_entity(foa);
	foa->line++;
	
	return 0;
}
