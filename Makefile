CPPFLAGS=-g -I. -I${HOME}/asr/include -I/usr/include -L${HOME}/asr/lib -lcfitsio -ltiledb
all:
	g++ ${CPPFLAGS} -o test test.cc fits2tile.cc

.PHONY: all
