CPPFLAGS=-g -std=c++11 -I. -I${HOME}/asr/include -I/usr/include -L${HOME}/asr/lib -ltiledb -lcfitsio
all:
	g++ ${CPPFLAGS} -o arraylist array_list.cc
	g++ ${CPPFLAGS} -o test test.cc fits2tile.cc

.PHONY: all
