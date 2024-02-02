PROJECT=fsgen2
CC=gcc
DEFS=
INCDIR=
INCS=
LIBDIR=
LIBS=
BASE=../../../Image/bin
KERNEL=../../../Kernel
BIN=\
	../Common/chdir.bin \
	../Common/chmod.bin \
	../Common/dir.bin \
	../Common/lbr.bin \
	../Common/xr.bin  \
	../Common/kernel.prg
OBJS=\
	main.o


$(PROJECT): $(OBJS)
	$(CC) $(DEFS) $(LIBDIR) $(OBJS) $(LIBS) -o $(PROJECT)

data.h: ../Common/*.prg makedata $(BIN)
	./makedata >data.h

main.o: data.h main.c
	$(CC) $(DEFS) $(INCDIR) $(INCS) -c main.c

makedata: makedata.c
	$(CC) $(DEFS) $(INCDIR) $(INCS) -c makedata.c
	$(CC) $(DEFS) $(LIBDIR) makedata.o $(LIBS) -o makedata

../Common/chdir.bin: $(BASE)/chdir.bin
	cd ../Common; cp $(BASE)/chdir.bin .

../Common/chmod.bin: $(BASE)/chmod.bin
	cd ../Common; cp $(BASE)/chmod.bin .

../Common/dir.bin: $(BASE)/dir.bin
	cd ../Common; cp $(BASE)/dir.bin .

../Common/lbr.bin: $(BASE)/lbr.bin
	cd ../Common; cp $(BASE)/lbr.bin .

../Common/xr.bin: $(BASE)/xr.bin
	cd ../Common; cp $(BASE)/xr.bin .

../Common/kernel.prg: $(KERNEL)/kernel.prg
	cd ../Common; cp $(KERNEL)/kernel.prg .

clean:
	-rm data.h
	-rm *.o
	-rm $(PROJECT)

main.o:      header.h main.c

