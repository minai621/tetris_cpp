# Set compiler to use
CC=g++
CFLAGS=-g -I. -fpermissive
LDFLAGS=
#DEPS=Tetris.h Matrix.h
OBJS=
DEBUG=0

all:: 19-01.exe 19-02.exe 19-03.exe 19-05.exe 19-10.exe 19-12.exe 19-14.exe 19-16.exe

19-01.exe: 19-01.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

19-02.exe: 19-02.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

19-03.exe: 19-03.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

19-05.exe: 19-05.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

19-10.exe: 19-10.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

19-12.exe: 19-12.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

19-14.exe: 19-14.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

19-16.exe: 19-16.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean: 
	rm -f *.exe *.o *~ *.stackdump
