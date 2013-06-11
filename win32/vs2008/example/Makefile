# Makefile for building libfoa examples using Visual Studio 2008.
#
# Author: Anders Lövgren
# Date:   2010-10-19

TARGET  = debug

SRCLIB  = ..\..\..\src
SRCDIR  = ..\..\..\example
DESTDIR = .

!IF "$(TARGET)" == "debug"
VC9OPTS = /Od /Gm /EHsc /RTC1 /MDd /Fd"Debug\vc90.pdb" /W3 /nologo /ZI /TP /errorReport:prompt
DEFINES = /D "_DEBUG" 
!ELSE
VC9OPTS	= /O2 /Oi /GL /FD /EHsc /MD /Gy /Fd"Release\vc90.pdb" /W3 /nologo /Zi /TP /errorReport:prompt
DEFINES	= /D "NDEBUG"
!ENDIF

DEFINES		= $(DEFINES) /D "WIN32" /D "_CONSOLE" /D "HAVE_CONFIG_H=1" /D "_UNICODE" /D "UNICODE"
DEFINES		= $(DEFINES) /DLIBFOA_IMPORTS /D_WIN32_WINNT=0x400 /DSTDCALL=_stdcall

INCLUDES	= /I$(SRCDIR) /I$(SRCLIB) /I. /I..
CFLAGS		= $(DEFINES) $(VC9OPTS) $(INCLUDES)
LDFLAGS		= /link $(IMPORTLIB)

IMPORTLIB	= ..\$(TARGET)\libfoa.lib
IMPORTDLL	= ..\$(TARGET)\libfoa.dll

EXAMPLES	= decode_callback.exe decode_error.exe decode_hashed_array.exe \
	decode_hashed_object.exe decode_hashed_object_array.exe decode_large_object.exe \
	decode_large_stream.exe decode_simple_array.exe decode_simple_object.exe \
	decode_stream.exe decode_unordered_object.exe encode_buffer.exe \
	encode_object.exe encode_stream.exe escape_buffer.exe init_libfoa.exe \
	set_mode.exe write_array_buffer.exe write_array_stream.exe

all : $(EXAMPLES)
	copy $(IMPORTDLL) $(DESTDIR)

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
	if exist *.pdb del /q *.pdb
	if exist *.ilk del /q *.ilk
	if exist *.exe.manifest del /q *.exe.manifest
distclean : clean
	if exist *.exe del /q *.exe
	if exist *.dll del /q *.dll
rebuild : distclean all
