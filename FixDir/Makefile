PROJECT=fixdir
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

.c.o:
	$(CC) $(DEFS) $(INCDIR) $(INCS) -c $<

clean:
	-rm *.o
	-rm $(PROJECT)

main.o:      header.h main.c

