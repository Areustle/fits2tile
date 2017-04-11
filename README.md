FAILURE EXAMPLE.

  REQUIRES CFITSIO

Requirements: 
-------------

* [cfitsio](https://heasarc.gsfc.nasa.gov/fitsio/fitsio.html) - Available in most package managers, and from [here](https://github.com/HEASARC/cfitsio)
* [TileDB](http://istc-bigdata.org/tiledb/index.html)

Usage + Installation:
---------------------

1. Update the "CPPFLAGS" variable in Makefile to point to local libs and includes.
1. fail_assert.sh will cause the failure

    make

    sh fail_assert.sh
