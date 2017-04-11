# CPPFLAGS=-g -std=c++11 -I. -I${HOME}/glu/usr/include -I${HOME}/asr/include -I/usr/include -L${HOME}/asr/lib -L${HOME}/glu/usr/lib -ltiledb -lcfitsio
CPPFLAGS=-g -std=c++11 -I. -I${HOME}/asr/include -I/usr/include -L${HOME}/asr/lib -ltiledb -lcfitsio
all:
	g++ ${CPPFLAGS} -o array_workspace array_workspace.cc
	g++ ${CPPFLAGS} -o array_create array_create.cc
	g++ ${CPPFLAGS} -o array_write array_write.cc
	g++ ${CPPFLAGS} -o array_read array_read.cc
	g++ ${CPPFLAGS} -o test test.cc fits2tile.cc
	g++ ${CPPFLAGS} -o fread file_read.cc

.PHONY: all
