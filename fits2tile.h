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
    Fits2tile ( fitsfile* input_file,               /* constructor */
                std::vector<std::string> input_dimensions,
                std::vector<std::string> input_attributes
              );

    /* Fits2tile ( fitsfile& infile ,              /1* constructor *1/ */
    /*             TileDB_ArraySchema array_schema */
    /*           ); */

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
    int f2ttype( std::string column_name );

    /* ====================  DATA MEMBERS  ======================================= */
    int status;
    long nrows;
    fitsfile* infile;
    std::string array_name;
    std::vector<std::string> dimensions;
    std::vector<std::string> attributes;
    std::vector<int> dimension_types;
    std::vector<int> attribute_types;

}; /* -----  end of class Fits2tile  ----- */

