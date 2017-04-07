CPPFLAGS=-g -I. -I/home/areustle/usr/include -I/usr/include -L/home/areustle/usr/lib
all:
	g++ ${CPPFLAGS} -lcfitsio -ltiledb -o test test.cc
	g++ ${CPPFLAGS} -lcfitsio -ltiledb -o fits2tile fits2tile.cc

.PHONY: all
