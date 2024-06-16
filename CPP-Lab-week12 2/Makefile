# Set compiler to use
CC=g++
CFLAGS=-g -I. -fpermissive
LDFLAGS=
#DEPS=Tetris.h Matrix.h
OBJS=
DEBUG=0

all:: 12-01.exe 12-02.exe 12-03.exe 12-04.exe 12-07.exe 12-08.exe 12-09.exe

12-01.exe: 12-01.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

12-02.exe: 12-02.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

12-03.exe: 12-03.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

12-04.exe: 12-04.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

12-07.exe: 12-07.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

12-08.exe: 12-08.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

12-09.exe: 12-09.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean: 
	rm -f *.exe *.o *~ *.stackdump
