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
1. Download week 460 Fermi Fits weekly file from: ftp://heasarc.nasa.gov/fermi/data/lat/weekly/photon/lat_photon_weekly_w460_p302_v001.fits
1. Update the "CPPFLAGS" variable in `Makefile` to point to local tiledb and cfitsio libraries and includes.

```
    make
    rm -rf my_workspace
    ./array_workspace
    ./array_create
    ./array_write
    ./array_read
    ./array_query
```
