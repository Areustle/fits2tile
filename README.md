fits2tile

a small utility for writing FITS data into TileDB.

Perhaps in the future I will generalize it, but for now
fits2tile is *very* user unfriendly.

Requirements: cfitsio, TileDB

Usage + Installation:
    - Download a Fermi Fits weekly file from: ftp://heasarc.nasa.gov/fermi/data/lat/weekly/photon/
    - Edit the 'filename' variable in test.cc to reference the fitsfile.
    - Alter the ArraySchema as best you wish.
    - Run the tileDB workspace creation example file to create a workspace and group.
    - Update the "CPPFLAGS" variable in Makefile to point to local libs and includes.

    make

    ./test
