CPPFLAGS=-g -std=c++11 -I. -I${HOME}/asr/include -I/usr/include -L${HOME}/asr/lib -ltiledb -lcfitsio
all:
	g++ ${CPPFLAGS} -o array_workspace array_workspace.cc
	g++ ${CPPFLAGS} -o array_create array_create.cc
	g++ ${CPPFLAGS} -o array_create_int array_create_int.cc
	g++ ${CPPFLAGS} -o array_write array_write.cc
	g++ ${CPPFLAGS} -o array_write_int array_write_int.cc
	g++ ${CPPFLAGS} -o array_read array_read.cc
	g++ ${CPPFLAGS} -o array_read_int array_read_int.cc

.PHONY: all
