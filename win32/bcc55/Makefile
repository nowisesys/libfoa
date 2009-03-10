# Makefile for building libfoa.dll using Borland C++ 5.5
#
# Author: Anders Lövgren
# Date:   2009-03-10

CC      = $(BCCDIR)\bin\bcc32
ILINK32 = $(BCCDIR)\bin\ilink32

BCC32OPTS = -ps -WD -WR -w-pia- -w-par- -5
CPPFLAGS  = -DWIN32 -DHAVE_CONFIG_H -D_BCC55 -DLIBFOA_EXPORTS -I$(SRCDIR) -I.
CFLAGS    = -O2 $(CPPFLAGS) $(BCC32OPTS)
LDFLAGS   = -x -c -aa -Tpd -Gi -Gl -OS -r -m $(LDGECKO)
LDGECKO   = -GC"Author: Anders Loevgren" -GC"Company: QNET" -GC"Released: 2009-03-10" 

IMPORTLIB = import32.lib cw32i.lib

LIBNAME = libfoa.dll
EXPNAME = libfoa.exp
OBJECTS = decode.obj encode.obj error.obj libfoa.obj memory.obj replace.obj stream.obj
SOURCES = $(OBJECTS:.obj=.c)

SRCDIR  = ..\..\src
DESTDIR = .
TEMPDIR = temp

.path.c = $(SRCDIR)

targets : $(LIBNAME)
$(LIBNAME) : $(OBJECTS)
	mkdir $(TEMPDIR)
	$(ILINK32) $(LDFLAGS) $(OBJECTS) $(IMPORTLIB) -I$(TEMPDIR) -L$(BCCDIR)\Lib, $@, $(EXPNAME)

clean :
	if exist *.obj del /q $(OBJECTS)
	if exist *.tds del /q *.tds
	if exist $(TEMPDIR) rmdir /s /q $(TEMPDIR)
distclean : clean
	if exist libfoa.* del /q libfoa.*

.c.obj :
	$(CC) $(CFLAGS) -c $<
