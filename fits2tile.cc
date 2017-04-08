/*
 * ============================================================================
 *
 *       Filename:  fits2tile.cc
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  04/05/2017 21:19:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * ============================================================================
 */
#include "fits2tile.h"
#include <stdlib.h>
#include <string>
#include <vector>
#include <stdexcept>


/*
 *-----------------------------------------------------------------------------
 *       Class:  Fits2tile
 *      Method:  Fits2tile
 * Description:  constructor
 *-----------------------------------------------------------------------------
 */
Fits2tile::Fits2tile (
    fitsfile* input_file,
    std::vector<std::string> input_dimensions,
    std::vector<std::string> input_attributes
    ):infile(input_file),
  dimensions(input_dimensions),
  attributes(input_attributes)
{
  fits_get_num_rows(infile, &nrows, &status);
  if (status) {
    fits_report_error(stderr, status);
    throw std::runtime_error("Error Opening FITS File");
  }

  for( auto attribute : attributes )
    attribute_types.push_back(f2ttype(attribute));

  for( auto dimension : dimensions )
    dimension_types.push_back(f2ttype(dimension));
}   /* -----  end of method Fits2tile::Fits2tile  ----- */

int
Fits2tile::f2ttype( std::string column_name )
{
  int colnum, typecode;
  long repeat, width;

  fits_get_colnum(
      infile,
      0,
      const_cast<char*>(column_name.c_str()),
      &colnum,
      &status);

  if (status) {
    fits_report_error(stderr, status);
    throw
      std::runtime_error("Error locating column "
                          +column_name
                          +" in FITS file");
  }

  fits_get_coltype(
      infile,
      colnum,
      &typecode,
      &repeat,
      &width,
      &status);

  return typecode;
}

TileDB_ArraySchema
Fits2tile::fill_schema ( TileDB_ArraySchema user_schema )
{
  user_schema.attributes_ = schema_attributes;
  user_schema.attribute_num_ = schema_attributes_size;
  user_schema.dimensions_ = schema_dimensions;
  user_schema.dim_num_ = schema_dimensions_size;
  user_schema.domain_ = schema_domain;
  user_schema.types_ = schema_types;
  return user_schema;
}   /* -----  end of method Fits2tile::fill_schema  ----- */

