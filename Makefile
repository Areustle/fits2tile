CPPFLAGS=-g -std=c++11 -I. -I${HOME}/glu/usr/include -I${HOME}/asr/include -I/usr/include -L${HOME}/asr/lib -L${HOME}/glu/usr/lib -ltiledb -lcfitsio
all:
	g++ ${CPPFLAGS} -o arraylist array_list.cc
	g++ ${CPPFLAGS} -o test test.cc fits2tile.cc
	g++ ${CPPFLAGS} -o fread file_read.cc

.PHONY: all
