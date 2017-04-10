/*
 * ============================================================================
 *
 *       Filename:  fits2tile.h
 *
 *    Description:  Interface for fits2tile library. Convert a FITS format
 *                  data file into a TileDB array.
 *
 *        Version:  1.0
 *        Created:  04/05/2017 21:04:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexander Reustle
 *   Organization:
 *
 * ============================================================================
 */
#include <string>
#include <vector>
#include <stdlib.h>
#include "fitsio.h"
#include "tiledb.h"
#include <tuple>

/*
 * ============================================================================
 *        Class:  Fits2tile
 *  Description:  Library for converting fits files to TileDB arrays on disk
 * ============================================================================
 */
class Fits2tile
{
  public:
    /*===============  TYPEDEFS      ======================================= */
    typedef std::tuple<int,int,int,size_t> meta_tuple;
    typedef std::pair<void*,size_t> buffer_pair;

    /* ==============  LIFECYCLE     ======================================= */
    Fits2tile ( fitsfile*,                       /* constructor */
                std::vector<std::string>,
                std::vector<std::string>
              );

    /* Fits2tile ( fitsfile& infile ,              /1* constructor *1/ */
    /*             TileDB_ArraySchema array_schema */
    /*           ); */

    /* ==============  ACCESSORS     ======================================= */
    long get_row_count(){return nrows;}
    TileDB_ArraySchema get_schema();
    std::pair<double, double> get_domain(int,int);

    /* ==============  MUTATORS      ======================================= */
    void set_schema(TileDB_ArraySchema);
    TileDB_ArraySchema fill_schema(TileDB_ArraySchema);
    void write_array(TileDB_Array&);

    /* ==============  OPERATORS     ======================================= */

  protected:
    /* ==============  DATA MEMBERS  ======================================= */

  private:
    /* ==============  Utilities     ======================================= */
    /* std::pair<int,size_t> ftype(int); */
    meta_tuple get_meta_tuple(int);
    int column_index(std::string);
    void check_fits_status( std::string );
    void read_dim_column(meta_tuple, double*);
    void read_attr_column(meta_tuple, void*);
    buffer_pair alloc_col_buf( meta_tuple );

    /* ==============  DATA MEMBERS  ======================================= */
    int status;
    long nrows;
    fitsfile* infile;
    std::string array_name;
    std::vector<meta_tuple> dimension_types;
    std::vector<meta_tuple> attribute_types;
    /* std::vector<buffer_pair> dbuf; */
    /* std::vector<buffer_pair> abuf; */
    const void** buffers;
    size_t* buffer_sizes;
    const char** c_attributes;
    const char** c_dimensions;
    double* c_domain;
    int* c_types;
}; /* -----  end of class Fits2tile  ----- */

