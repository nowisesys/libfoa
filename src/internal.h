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
#define FOA_ENCODE_GREATER_THAN      "%3E"
#define FOA_ENCODE_NAME_DATA         FOA_ENCODE_EQUAL_SIGN "" FOA_ENCODE_GREATER_THAN

#define FOA_TYPE_ASSIGNMENT   '='
#define FOA_TYPE_GREATER_THAN '>'

/* 
 * Write an formatted message to the error buffer.
 */
void write_errlog(char **buff, int code, const char *fmt, ...)
	__attribute__((visibility("hidden")));

/* 
 * Read next entity from the input memory buffer.
 */
int read_memory(struct libfoa *foa) 
	__attribute__((visibility("hidden")));

/* 
 * Read next entity from the input stream.
 */
int read_stream(struct libfoa *foa) 
	__attribute__((visibility("hidden")));

/* 
 * Decode the entity in foa->buff and update foa->entity.
 */
void decode_entity(struct libfoa *foa) 
	__attribute__((visibility("hidden")));

/* 
 * Plan for future error logging thru use supplied error logger.
 */
#define logerr write_errlog

#endif /* __INTERNAL_H__ */
