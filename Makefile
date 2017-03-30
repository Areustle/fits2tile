CPPFLAGS=-g -I/Users/areustle/asr/include -L/Users/areustle/asr/lib
all:
	g++ ${CPPFLAGS} -lcfitsio -ltiledb -o test test.cc
