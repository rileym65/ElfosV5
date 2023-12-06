PROJECT=fsgen2
CC=gcc
DEFS=
INCDIR=
INCS=
LIBDIR=
LIBS=
OBJS=\
	main.o


$(PROJECT): $(OBJS)
	$(CC) $(DEFS) $(LIBDIR) $(OBJS) $(LIBS) -o $(PROJECT)

main.o: data.h main.c
	$(CC) $(DEFS) $(INCDIR) $(INCS) -c main.c

clean:
	-rm *.o
	-rm $(PROJECT)

main.o:      header.h main.c

