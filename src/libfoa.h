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

#ifndef __LIBFOA_H__
#define __LIBFOA_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#if defined(__GNUC__) && ! defined(__NO_INLINE__)
# define FOA_API_INLINE static inline
#else
# define FOA_API_INLINE __inline
#endif

#if defined(WIN32) || defined(_WINDOWS) || defined(__CYGWIN__)
	/* Define LIBFOA_EXPORTS when building library on windows. */
# if defined(LIBFOA_EXPORTS)
#  if defined(__GNUC__)
#   define FOA_API_PUBLIC __attribute__((dllexport))
#  else
	/* Note: actually gcc seems to also supports this syntax. */
#   define FOA_API_PUBLIC __declspec(dllexport)
#  endif
# else
#  if defined(__GNUC__)
#   define FOA_API_PUBLIC __attribute__((dllimport))
#  else
	/* Note: actually gcc seems to also supports this syntax. */
#   define FOA_API_PUBLIC __declspec(dllimport) 
#  endif
# endif
# define FOA_API_HIDDEN
#else
# if __GNUC__ >= 4
#  define FOA_API_PUBLIC __attribute__ ((visibility("default")))
#  define FOA_API_HIDDEN __attribute__ ((visibility("hidden")))
# else
#  define FOA_API_PUBLIC
#  define FOA_API_HIDDEN
# endif
#endif

#define FOA_VERSION 0x000602
#define FOA_MAJOR ((FOA_VERSION) & 0xff0000 >> 16)
#define FOA_MINOR ((FOA_VERSION) & 0x00ff00 >> 8)
#define FOA_REVIS ((FOA_VERSION) & 0x0000ff)

/* 
 * Default memory allocation strategy.
 */
#define FOA_MEMORY_ALLOC_INIT  128              /* initial buffer size */
#define FOA_MEMORY_ALLOC_STEP  256              /* realloc buffer size */
#define FOA_MEMORY_ALLOC_MAX   8 * 1024 * 1024  /* maximum buffer size */
#define FOA_MEMORY_ALLOC_UNLIM 0                /* set maximun to unlimited */

/* 
 * Classification of the foa entity structure type:
 */
#define FOA_TYPE_DATA_ENTITY   0
#define FOA_TYPE_ERROR_MESSAGE 1
#define FOA_TYPE_START_OBJECT '('
#define FOA_TYPE_START_ARRAY  '['
#define FOA_TYPE_END_OBJECT   ')'
#define FOA_TYPE_END_ARRAY    ']'
#define FOA_TYPE_NAME_DATA    '='       /* name data separator */
#define FOA_TYPE_SPEC_CHARS   "([])"    /* ambigous special chars */

/* 
 * Default mode of operation:
 */
#define FOA_ENABLE_ESCAPE  1   /* escape special chars */
#define FOA_ENABLE_HASHES  1   /* encode/decode named data */

/* 
 * Mode parameter for foa_get_mode() or foa_set_mode().
 */
#define FOA_MODE_ESCAPE  1
#define FOA_MODE_HASHES  2

/* 
 * Current scanned entity.
 */
struct foa_entity
{
	const char *name;      /* named data or NULL */
	const char *data;      /* decoded data */
	int type;              /* see FOA_TYPE_XXX */
	unsigned long line;    /* input line */
};
	
/* 
 * Entity or error handler. If this function returns 0 to foa_scan(), 
 * then the parsing will stop and foa_scan() will return immidiatelly.
 */
typedef int (*foa_handler)(const struct foa_entity *ent, void *arg);

struct libfoa
{
	struct foa_entity entity;  /* current entity */
	foa_handler func;          /* callback function */
	void *arg;                 /* callback argument */
	unsigned long line;        /* parse line */
	char *buff;                /* parse buffer */
	size_t used;               /* current buffer size (used) */
	size_t size;               /* current buffer size (allocated) */
	size_t step;               /* memory alloc strategy */
	size_t max;                /* maximum buffer size */
	FILE *file;                /* input stream (possibly null) */
	const char *curr;          /* input buffer (possibly null) */
	const char *next;          /* next in input buffer */
	char *errmsg;              /* last error message */
	int escape;                /* enable escape of special chars */
	int hashes;                /* enable hashed (named) data */
};

/*
 * Initilize library usage. Returns 0 if successful and -1
 * on error.
 */
FOA_API_PUBLIC extern int foa_init(struct libfoa *foa);

/* 
 * Cleanup after finished.
 */
FOA_API_PUBLIC extern void foa_cleanup(struct libfoa *foa);

/* 
 * Define memory allocation strategy. The step variable defines how
 * aggressive memory is allocated. The max variable sets a maximum
 * buffer size in bytes (use 0 for unlimited). Returns 0 if successful
 * and -1 on error.
 */
FOA_API_PUBLIC extern int foa_alloc_strategy(struct libfoa *foa, size_t step, size_t max);

/* 
 * Use callback function to handle data seen. The optional arg get
 * passed along together with each call to func.
 */
FOA_API_PUBLIC extern void foa_set_callback(struct libfoa *foa, foa_handler func, void *arg);

/* 
 * Set file stream for reading and writing.
 */
FOA_API_PUBLIC extern void foa_set_stream(struct libfoa *foa, FILE *file);

/* 
 * Set input buffer for memory reading.
 */
FOA_API_PUBLIC extern void foa_set_buffer(struct libfoa *foa, const char *input);

/* 
 * Set current mode.
 */
FOA_API_PUBLIC extern void foa_set_mode(struct libfoa *foa, int mode, int enable);
	
/* 
 * Get current mode.
 */
FOA_API_PUBLIC extern void foa_get_mode(struct libfoa *foa, int mode, int *enable);
	
/* 
 * Decode the input using the registered callback function for 
 * each type (data or special character) seen. Returns 0 when
 * input data has been successful processed and -1 on error.
 */
FOA_API_PUBLIC extern int foa_scan(struct libfoa *foa);

/* 
 * Decode the input iterative. This is an alternative to
 * using the foa_scan() function. Returns the next entity
 * or NULL on error.
 */
FOA_API_PUBLIC extern const struct foa_entity * foa_next(struct libfoa *foa);

/* 
 * Write data to memory buffer or file stream. If append is true,
 * then data is appended (only for memory buffer), otherwise the
 * memory buffer is overwritten. The type is one of FOA_TYPE_XXX.
 * Use NULL as name to write anonymous data. Returns the encoded 
 * string on success and NULL on error.
 */
FOA_API_PUBLIC extern const char * foa_write(struct libfoa *foa, int append, int type, 
			      const char *name, const char *data);

/* 
 * Get last error message.
 */
FOA_API_INLINE const char * foa_last_error(struct libfoa *foa)
{
	return foa->errmsg;
}

/* 
 * Returns true if error is set.
 */
FOA_API_INLINE int foa_error_set(struct libfoa *foa)
{
	return foa->errmsg ? 1 : 0;
}

/* 
 * Clear stored error message.
 */
FOA_API_PUBLIC extern void foa_reset_error(struct libfoa *foa);
	
#ifdef __cplusplus
}      /* extern "C" */
#endif

#endif /* __LIBFOA_H__ */
