fits2tile

a small utility for writing FITS data into TileDB.

Perhaps in the future I will generalize it, but for now
fits2tile is *very* user unfriendly.

Requirements: 
-------------

* [cfitsio](https://heasarc.gsfc.nasa.gov/fitsio/fitsio.html) - Available in most package managers, and from [here](https://github.com/HEASARC/cfitsio)
* [TileDB](http://istc-bigdata.org/tiledb/index.html)

Usage + Installation:
---------------------

1. Download a Fermi Fits weekly file from: ftp://heasarc.nasa.gov/fermi/data/lat/weekly/photon/
1. Edit the 'filename' variable in test.cc to reference the fitsfile.
1. Alter the ArraySchema as best you wish.
1. Run the tileDB workspace creation example file to create a workspace and group.
1. Update the "CPPFLAGS" variable in Makefile to point to local libs and includes.

    make

    ./test
