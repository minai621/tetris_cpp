# Set compiler to use
CC=g++
CFLAGS=-g -I. -fpermissive -Wno-deprecated -std=c++11
LDFLAGS=
DEBUG=0

all:: Main.exe 

Main.exe: Main.o Matrix_e.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o: %.c $(DEPS_CHAT)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS_TET)
	$(CC) -c -o $@ $< $(CFLAGS)

clean: 
	rm -f *.exe 
	rm -f *.o *~ *.stackdump
