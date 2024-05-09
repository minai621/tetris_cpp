# Set compiler to use
CC=g++
CFLAGS=-g -I. -fpermissive
LDFLAGS=
DEPS=Tetris.h Matrix.h
OBJS=Main.o Tetris.o Matrix.o ttymodes.o
DEBUG=0

all:: Main.exe

Main.exe: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean: 
	rm -f *.exe *.o *~ *.stackdump
