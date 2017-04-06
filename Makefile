CPPFLAGS=-g -I/Users/areustle/asr/include -L/Users/areustle/asr/lib
all:
	g++ ${CPPFLAGS} -lcfitsio -ltiledb -o test test.cc
	g++ ${CPPFLAGS} -lcfitsio -ltiledb -o fits2tile fits2tile.cc

.PHONY: all
