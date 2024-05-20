CXX		= g++
EXTRA_CFLAGS =
INCLUDE	= -I.
CFLAGS	= -ggdb -O3 -march=native -Wall  $(INCLUDE) $(EXTRA_CFLAGS) `ppl7-config --cflags release`
LIB		= `ppl7-config --libs release`  -lstdc++
LIBDEP	:= $(shell ppl7-config --ppllib release)
PROGRAM	= ppltk


OBJECTS=

$(PROGRAM): $(OBJECTS) compile/main.o $(LIBDEP)
	$(CC) -o $(PROGRAM) $(OBJECTS) compile/main.o $(CFLAGS) $(LIB)
	-chmod 755 $(PROGRAM)

all: $(PROGRAM)

clean:
	-rm -f *.o $(PROGRAM) *.core compile


compile/main.o: demo/main.cpp Makefile include/ppltk.h
	- @mkdir -p compile
	$(CXX) -o compile/main.o -c demo/main.cpp $(CFLAGS)
