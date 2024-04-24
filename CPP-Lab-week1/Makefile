# Set compiler to use
CC=g++
CFLAGS=-g -I. -fpermissive -Wno-deprecated -std=c++14
LDFLAGS=
DEBUG=0
DEPS=Matrix.h

all:: Lab1.exe

Lab1.exe: Lab1.o Matrix.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean: 
	rm -f *.exe 
	rm -f *.o *~ *.stackdump
