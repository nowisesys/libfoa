EXTRA_DIST = libfoa.pc libfoa.pc.in

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libfoa.pc

lib_LTLIBRARIES = libfoa.la
libfoa_la_SOURCES = libfoa.c libfoa.h internal.h stream.c memory.c error.c \
		    decode.c encode.c libfoa-utils.h replace.c
libfoa_la_LDFLAGS = -version-info 1:0:1 -Wl,-x

noinst_HEADERS = internal.h
include_HEADERS = libfoa.h libfoa-utils.h
