PROJECT=elfos
CC=gcc
DEFS=
INCDIR=
INCS=
LIBDIR=
LIBS=
OBJS=\
	allocau.o \
	checkwrite.o \
	create.o \
	delchain.o \
	dideread.o \
	didewrite.o \
	dirempty.o \
	extendfile.o \
	finddirent.o \
	freedirent.o \
	lmptosec.o \
	lmptosecofs.o \
	loaddirent.o \
	nextsec.o \
	o_chdir.o \
	o_close.o \
	o_delete.o \
	o_mkdir.o \
	o_open.o \
	o_read.o \
	o_rename.o \
	o_rmdir.o \
	o_write.o \
	opencd.o \
	openmd.o \
	readlump.o \
	readsyssec.o \
	savesyssec.o \
	searchdir.o \
	seekend.o \
	setdatetime.o \
	setfileflags.o \
	sectolmp.o \
	splitpath.o \
	validfn.o \
	writelump.o  \
	writesyssec.o \
	zerolump.o


all: $(OBJS)

.c.o:
	$(CC) $(DEFS) $(INCDIR) $(INCS) -c $<

clean:
	-rm *.o

