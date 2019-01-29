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

#ifdef DEBUG_REPLACE_C
# undef HAVE_MEMSET
# undef HAVE_STRCHR
# undef HAVE_STRDUP
# undef HAVE_STRPBRK
# undef HAVE_MALLOC
# undef HAVE_REALLLOC
#endif

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
 * Replacement for missing memset().
 */
#if ! defined(HAVE_MEMSET)
void * memset(void *s, int c, size_t n)
{
	void *p = s;
	if(s) {
		while(n-- > 0)
			*(char *)s++ = c;
	}
	return p;
}
#endif

/*
 * Replacement for missing strchr().
 */
#if ! defined(HAVE_STRCHR)
char * strchr(const char *s, int c)
{
	if(s) { 
		while(*s && *s != c) ++s;
		return *s ? (char *)s : NULL;
	}
	return NULL;
}
#endif

/*
 * Replacement for missing strdup().
 */
#if ! defined(HAVE_STRDUP)
char * strdup(const char *s)
{
	if(s) { 
		char *p = malloc(strlen(s) + 1);
		if(!p)
			return NULL;
		return strcpy(p, s);
	}
	return NULL;
}
#endif

/*
 * Replacement for missing strpbrk().
 */
#if ! defined(HAVE_STRPBRK)
char * strpbrk(const char *s, const char *accept)
{
	while(s && *s && !strchr(accept, *s++));
	return (char *)s;
}
#endif

/*
 * The malloc and realloc replacement code was taken from 
 * the autoconf info.
 */
#if ! defined(HAVE_MALLOC)

# undef malloc
# include <sys/types.h>

void * malloc();

/*
 * Allocate an N-byte block of memory from the heap.
 * If N is zero, allocate a 1-byte block.  
 */
void * rpl_malloc (size_t n)
{
	if (n == 0)
		n = 1;
	return malloc (n);
}
#endif /* ! HAVE_MALLOC */

#if ! defined(HAVE_REALLOC)

# undef realloc
# include <sys/types.h>

void * realloc();

/*
 * Allocate an N-byte block of memory from the heap.
 * If N is zero, allocate a 1-byte block.  
 */
void * rpl_realloc (void *p, size_t n)
{
	if (n == 0)
		n = 1;
	return realloc (p, n);
}
#endif /* ! HAVE_REALLOC */

#ifndef HAVE_VSNPRINTF
int vsnprintf(char *str, size_t size, const char *fmt, va_list ap) 
{
	return vsprintf(str, fmt, ap);
}
#endif
