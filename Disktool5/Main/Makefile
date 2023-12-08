PROJECT=disktool5
CC=gcc
DEFS=
INCDIR=
INCS=
LIBDIR=
LIBS=
OBJS=\
	../Common/allocau.o \
	../Common/checkwrite.o \
	../Common/create.o \
	../Common/delchain.o \
	../Common/dideread.o \
	../Common/didewrite.o \
	../Common/extendfile.o \
	../Common/finddirent.o \
	../Common/dirempty.o \
	../Common/freedirent.o \
	../Common/lmptosec.o \
	../Common/lmptosecofs.o \
	../Common/loaddirent.o \
	../Common/nextsec.o \
	../Common/o_chdir.o \
	../Common/o_close.o \
	../Common/o_delete.o \
	../Common/o_mkdir.o \
	../Common/o_open.o \
	../Common/o_read.o \
	../Common/o_rename.o \
	../Common/o_rmdir.o \
	../Common/o_write.o \
	../Common/opencd.o \
	../Common/openmd.o \
	../Common/readlump.o \
	../Common/readsyssec.o \
	../Common/savesyssec.o \
	../Common/searchdir.o \
	../Common/sectolmp.o \
	../Common/seekend.o \
	../Common/setdatetime.o \
	../Common/setfileflags.o \
	../Common/splitpath.o \
	../Common/validfn.o \
	../Common/writelump.o \
	../Common/writesyssec.o \
	../Common/zerolump.o \
	main.o

$(PROJECT): $(OBJS)
	$(CC) $(DEFS) $(LIBDIR) $(OBJS) $(LIBS) -o $(PROJECT)

.c.o:
	$(CC) $(DEFS) $(INCDIR) $(INCS) -c $<

clean:
	-rm *.o
	-rm $(PROJECT)

main.o:      header.h main.c

