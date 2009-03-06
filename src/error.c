/* Library implementaion of FOA - Fast Object and Array encoding.
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
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include "libfoa.h"
#include "internal.h"

/* 
 * Write an error message to buffer pointed to buffer buff.
 */
void write_errlog(char **buff, int code, const char *fmt, ...)
{
	const char *errstr = NULL;
	va_list ap;
	int size;
	
	va_start(ap, fmt);
	size = vsnprintf(NULL, 0, fmt, ap);
	if(size > 0) {
		if(code) {
			errstr = strerror(code);
			if(errstr) 
				size += strlen(errstr) + 3;
		}
		*buff = realloc(*buff, size + 1);
		if(!*buff) {
			va_end(ap);
			return;
		}
		va_start(ap, fmt);
		size = vsnprintf(*buff, size + 1, fmt, ap);
		if(errstr)
			sprintf(*buff + size, " (%s)", errstr);
	}
	va_end(ap);
}
