# Makefile for building libfoa examples using Visual Studio 2003.
#
# Author: Anders Lövgren
# Date:   2009-03-13

TARGET  = release

SRCLIB  = ..\..\..\src
SRCDIR  = ..\..\..\example
DESTDIR = .

VC7OPTS   = /O2 /G5
DEFINES   = /DWIN32 /DHAVE_CONFIG_H /DLIBFOA_IMPORTS /D_WIN32_WINNT=0x400 /DSTDCALL=_stdcall
INCLUDES  = /I$(SRCDIR) /I$(SRCLIB) /I. /I..
CFLAGS    = $(VC7OPTS) $(DEFINES) $(INCLUDES)
LDFLAGS   = /link $(IMPORTLIB)

IMPORTLIB = ..\$(TARGET)\libfoa.lib

EXAMPLES  = decode_callback.exe decode_error.exe decode_hashed_array.exe \
    decode_hashed_object.exe decode_hashed_object_array.exe decode_large_object.exe \
    decode_large_stream.exe decode_simple_array.exe decode_simple_object.exe \
    decode_stream.exe decode_unordered_object.exe encode_buffer.exe \
    encode_object.exe encode_stream.exe escape_buffer.exe init_libfoa.exe \
    set_mode.exe write_array_buffer.exe write_array_stream.exe

all : $(EXAMPLES)

decode_callback.exe : {$(SRCDIR)}decode_callback.c
decode_error.exe : {$(SRCDIR)}decode_error.c
decode_hashed_array.exe : {$(SRCDIR)}decode_hashed_array.c
decode_hashed_object.exe : {$(SRCDIR)}decode_hashed_object.c
decode_hashed_object_array.exe : {$(SRCDIR)}decode_hashed_object_array.c
decode_large_object.exe : {$(SRCDIR)}decode_large_object.c
decode_large_stream.exe : {$(SRCDIR)}decode_large_stream.c
decode_simple_array.exe : {$(SRCDIR)}decode_simple_array.c
decode_simple_object.exe : {$(SRCDIR)}decode_simple_object.c
decode_stream.exe : {$(SRCDIR)}decode_stream.c
decode_unordered_object.exe : {$(SRCDIR)}decode_unordered_object.c
encode_buffer.exe : {$(SRCDIR)}encode_buffer.c
encode_object.exe : {$(SRCDIR)}encode_object.c
encode_stream.exe : {$(SRCDIR)}encode_stream.c 
escape_buffer.exe : {$(SRCDIR)}escape_buffer.c
init_libfoa.exe : {$(SRCDIR)}init_libfoa.c
set_mode.exe : {$(SRCDIR)}set_mode.c
write_array_buffer.exe : {$(SRCDIR)}write_array_buffer.c
write_array_stream.exe : {$(SRCDIR)}write_array_stream.c

# Rule for building source files (we can't use builtin):
{$(SRCDIR)}.c{}.exe :
	$(CC) $(CFLAGS) $< $(LDFLAGS)

clean :
	if exist *.obj del /q *.obj
	if exist *.tds del /q *.tds
distclean : clean
	if exist *.exe del /q *.exe
rebuild : distclean all
