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

/* 
 * Utility functions for libfoa.
 */

#ifndef __LIBFOA_UTILS_H__
#define __LIBFOA_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __LIBFOA_H__
# include <libfoa.h>
#endif

/* 
 * Enable or disable escape of special chars.
 */
FOA_API_INLINE void foa_enable_escape(struct libfoa *foa, int enable)
{
	foa_set_mode(foa, FOA_MODE_ESCAPE, enable);
}

/* 
 * Enable or disable looking for hashes (key = value) in input data.
 */
FOA_API_INLINE void foa_enable_hashes(struct libfoa *foa, int enable)
{
	foa_set_mode(foa, FOA_MODE_HASHES, enable);
}
	
/* 
 * Write data entity.
 */
FOA_API_INLINE const char * foa_write_data(struct libfoa *foa, const char *name, const char *data, int append)
{
	return foa_write(foa, append, FOA_TYPE_DATA_ENTITY, name, data);
}

/* 
 * Specialized for writing anonymous string data, that is, array of 
 * const char * where last element must be NULL. This function may 
 * cause the maximum alloc limit to be reached for the memory buffer 
 * if used in append mode. Returns NULL on error.
 */
FOA_API_PUBLIC extern const char * foa_write_array(struct libfoa *foa, const char **data, int append);

FOA_API_INLINE const char * foa_start_object(struct libfoa *foa, int append)
{
	return foa_write(foa, append, FOA_TYPE_START_OBJECT, NULL, NULL);
}

FOA_API_INLINE const char * foa_end_object(struct libfoa *foa, int append)
{
	return foa_write(foa, append, FOA_TYPE_END_OBJECT, NULL, NULL);
}

FOA_API_INLINE const char * foa_start_array(struct libfoa *foa, int append)
{
	return foa_write(foa, append, FOA_TYPE_START_ARRAY, NULL, NULL);
}

FOA_API_INLINE const char * foa_end_array(struct libfoa *foa, int append)
{
	return foa_write(foa, append, FOA_TYPE_END_ARRAY, NULL, NULL);
}

#ifdef __cplusplus
}      /* extern "C" */
#endif

#endif /* __LIBFOA_UTILS_H__ */
