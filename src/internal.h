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

/*
 * Implementation private API.
 */

#ifndef __INTERNAL_H__
#define __INTERNAL_H__

/* 
 * String representation of HTTP encoded characters (special chars):
 */
#define FOA_ENCODE_OPEN_PARANTHESIS  "%28"
#define FOA_ENCODE_OPEN_BRACKET      "%5B"
#define FOA_ENCODE_CLOSE_PARANTHESIS "%29"
#define FOA_ENCODE_CLOSE_BRACKET     "%5D"
#define FOA_ENCODE_EQUAL_SIGN        "%3D"

#define FOA_ENCODE_BEGIN_OBJECT      FOA_ENCODE_OPEN_PARANTHESIS
#define FOA_ENCODE_BEGIN_ARRAY       FOA_ENCODE_OPEN_BRACKET
#define FOA_ENCODE_END_OBJECT        FOA_ENCODE_CLOSE_PARANTHESIS
#define FOA_ENCODE_END_ARRAY         FOA_ENCODE_CLOSE_BRACKET
#define FOA_ENCODE_NAME_DATA         FOA_ENCODE_EQUAL_SIGN

/* 
 * Write an formatted message to the error buffer.
 */
void write_errlog(char **buff, int code, const char *fmt, ...) FOA_API_HIDDEN;

/* 
 * Read next entity from the input memory buffer.
 */
int read_memory(struct libfoa *foa) FOA_API_HIDDEN;

/* 
 * Read next entity from the input stream.
 */
int read_stream(struct libfoa *foa) FOA_API_HIDDEN;

/* 
 * Decode the entity in foa->buff and update foa->entity.
 */
void decode_entity(struct libfoa *foa) FOA_API_HIDDEN;

/*
 * Replacement for missing library functions.
 */
#if ! defined(HAVE_STRCHR)
# undef strchr
extern char * strchr(const char *s, int c) FOA_API_HIDDEN;
#endif
#if ! defined(HAVE_MEMSET)
extern void * memset(void *s, int c, size_t n) FOA_API_HIDDEN;
#endif
#if ! defined(HAVE_STRDUP)
# undef strdup
extern char * strdup(const char *s) FOA_API_HIDDEN;
#endif
#if ! defined(HAVE_STRPBRK)
# undef strpbrk
extern char * strpbrk(const char *s, const char *accept) FOA_API_HIDDEN
#endif
#if ! defined(HAVE_VSNPRINTF)
extern int vsnprintf(char *str, size_t size, const char *fmt, va_list ap) FOA_API_HIDDEN;
#endif

/* 
 * Plan for future error logging thru use supplied error logger.
 */
#define logerr write_errlog

#endif /* __INTERNAL_H__ */
