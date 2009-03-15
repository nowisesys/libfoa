#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(libfoa, 0.6.5, lespaul@algonet.se)
AM_INIT_AUTOMAKE([libfoa], [0.6.4])
AC_CONFIG_SRCDIR([src/libfoa.c])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_RANLIB
AM_PROG_CC_C_O

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
CFLAGS_SAVED="$CFLAGS"
CFLAGS="$CFLAGS -fno-builtin"
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset strchr strdup strerror strpbrk vsnprintf])
CFLAGS="${CFLAGS_SAVED}"

# Check whether to build examples:
AC_MSG_CHECKING([whether to build examples])
AC_ARG_ENABLE([examples], 
  [AS_HELP_STRING([--enable-examples], 
    [Build libfoa example code [default=no]])],
  [build_examples=$enableval],
  [build_examples=no]
)
AC_MSG_RESULT($build_examples)
AM_CONDITIONAL(BUILD_EXAMPLES, test x${build_examples} == xyes)

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 example/Makefile
		 doc/Makefile
		 src/libfoa.pc])
AC_OUTPUT
