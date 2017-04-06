/*
 * =====================================================================================
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
 * =====================================================================================
 */
#include "fits2tile.h"
#include <stdlib.h>


/*
 *--------------------------------------------------------------------------------------
 *       Class:  Fits2tile
 *      Method:  Fits2tile
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
Fits2tile::Fits2tile ()
{
}  /* -----  end of method Fits2tile::Fits2tile  (constructor)  ----- */

Fits2tile::Fits2tile (
    fitsfile& infile,
    std::string name,
    std::vector<const std::string> dims,
    std::vector<const std::string> attribs
    ) :
  infile{infile},
  array_name{name},
  dimensions{dims},
  attributes{attribs},
  schema_attributes_size{attrib.size()},
  schema_dimensions_size{dims.size()}
{
  schema_attributes = new (char*)[schema_attributes_size];
  schema_dimensions = new (char*)[schema_dimensions_size];
  schema_types = new int[schema_attributes_size+1];

  fits_get_num_rows(infile,
      &nrows,
      &status
      );
  if (status) {
    fits_report_error(stdout, status);
    return 2; /* TODO fix return of constructor failure */
  }

  int colnum;
  int typecode;
  long repeat;
  long width;
  for(int i=0; i<schema_attributes_size; ++i){
    schema_attributes[i] = c_str(attributes[i]);
    fits_get_colnum(infile,
        0,
        schema_attributes[i],
        &colnum,
        &status
        );
    fits_get_col_type(infile,
        &colnum,
        &typecode,
        &width,
        &status
        );
    schema_types[i] = f2ttype(typecode);
  }

  std::vector<int> dimension_types;
  for(int i=0; i<schema_dimensions_size; ++i){
    schema_dimensions[i] = c_str(dimensions[i]);
    fits_get_colnum(infile,
        0,
        schema_dimensions[i],
        &colnum,
        &status
        );
    fits_get_col_type(infile,
        &colnum,
        &typecode,
        &width,
        &status
        );
    dimension_types.push_back(f2ttype(typecode));
  }

  //For now, all dimensions must be the same type.
  //Will improve once I figure out how to handle the
  //domain type for disparate dimensions.
  bool flag = false;
  if(dimension_types.size() > 1){
    for(int i=1; i<dimension_types.size(); ++i){
      if(dimension_types[i] != dimension_types[i-1]) flag = true;
    }
  }
  if(flag) schema_types[schema_attribute_size] = dimension_types[0];

}   /* -----  end of method Fits2tile::Fits2tile  ----- */

TileDB_ArraySchema
Fits2tile::fill_schema ( TileDB_ArraySchema user_schema )
{
  user_schema.attributes_ = schema_attributes;
  user_schema.attribute_num_ = schema_attributes_size;
  user_schema.dimensions_ = schema_dimensions;
  user_schema.dim_num_ = schema_dimensions_size;
  user_schema.domain_ = schema_domain;
  user_schema.types_ = schema_types;
  return ;
}   /* -----  end of method Fits2tile::fill_schema  ----- */

