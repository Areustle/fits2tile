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
    ./array_read > ar.out
    ./array_query > aq.out
```

`array_workspace` creates a workspace and group necessary for tiledb, titled my_workspace


`array_create` sets the schema for the array. It defines the (for now 2) dimensions and their domains, along with the attribute, a dummy char 'a' for now.
The data is intended to be astronomical point data, so the dimensions are Right Ascension and Declination in the celestial coordinate system.

`array_write` Reads data from the FITS file for mission week 460 and writes it to the array we just created.

`array_read` Reads the entire array back out. This is quite large, so dump it to a file.

`array_query` Reads a small subarray. Found a bug in TileDB with this one.
