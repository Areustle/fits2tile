/*
 * =====================================================================================
 *
 *       Filename:  fits2tile.h
 *
 *    Description:  Interface for fits2tile library. Convert a FITS format data file
 *                  into a TileDB array.
 *
 *        Version:  1.0
 *        Created:  04/05/2017 21:04:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexander Reustle
 *   Organization:
 *
 * =====================================================================================
 */
#include <string>
#include <vector>
#include <stdlib.h>
#include "fitsio.h"
#include "tiledb.h"
/*
 * =====================================================================================
 *        Class:  Fits2tile
 *  Description:  Library for converting fits files to TileDB arrays on disk
 * =====================================================================================
 */
class Fits2tile
{
  public:
    /* ====================  LIFECYCLE     ======================================= */
    Fits2tile ( fitsfile& infile,               /* constructor */
                std::string array_name,
                std::vector<std::string> dimensions,
                std::vector<std::string> attributes
              );

    /* Fits2tile ( fitsfile& infile,               /1* constructor *1/ */
    /*            std::string array_name, */
    /*            std::vector<unsigned int> dimensions, */
    /*            std::vector<unsigned int> attributes */
    /*          ); */

    Fits2tile ( fitsfile& infile ,              /* constructor */
                TileDB_ArraySchema array_schema
              );

    /* Fits2tile ( std::vector<fitsfile&> infiles,               /1* constructor *1/ */
    /*            std::string array_name, */
    /*            std::vector<std::string> dimensions, */
    /*            std::vector<std::string> attributes */
    /*          ); */

    /* Fits2tile ( std::vector<fitsfile&> infiles ,              /1* constructor *1/ */
    /*            TileDB_ArraySchema array_schema */
    /*          ); */

    /* ====================  ACCESSORS     ======================================= */
    long get_row_count();
    std::vector<std::string> get_column_names();
    TileDB_ArraySchema get_schema();

    /* ====================  MUTATORS      ======================================= */
    void set_schema(TileDB_ArraySchema);
    TileDB_ArraySchema fill_schema(TileDB_ArraySchema);
    void array_write(TileDB_Array&);

    /* ====================  OPERATORS     ======================================= */

  protected:
    /* ====================  DATA MEMBERS  ======================================= */

  private:
    /* ====================  Utilities     ======================================= */
    int f2ttype(int fits_typecode);

    /* ====================  DATA MEMBERS  ======================================= */
    int status = 0;
    long nrows;
    const fitsfile& infile;
    std::string array_name;
    std::vector<const std::string> dimensions;
    std::vector<const std::string> attributes;
    char* schema_attributes[];
    const size_t schema_attributes_size;
    char* schema_dimensions[];
    const size_t schema_dimensions_size;
    template<typename T> T schema_domain[];
    int schema_types[];

}; /* -----  end of class Fits2tile  ----- */

