# Set compiler to use
CC=g++
CFLAGS=-g -I. -fpermissive
LDFLAGS_TET=
DEPS_TET=CTetris.h Tetris.h Matrix.h
OBJS_TET=Main.o CTetris.o Tetris.o Matrix.o ttymodes.o
DEBUG=0

all:: Main.exe

Main.exe: $(OBJS_TET)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS_TET)

%.o: %.c $(DEPS_TET)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS_TET)
	$(CC) -c -o $@ $< $(CFLAGS)

clean: 
	rm -f *.exe *.o *~ *.stackdump
