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
#include "tiledb.h"
#include <stdlib.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <tuple>

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
  attributes(input_attributes),
  status(0)
{
  fits_get_num_rows(infile, &nrows, &status);
  check_fits_status("Error Opening FITS File");

  // Throwables passed, begin 'new'ing.
  c_attributes  = new const char*[attributes.size()];
  c_dimensions  = new const char*[dimensions.size()];
  c_domain      = new double[2*dimensions.size()];
  c_types       = new int[1+attributes.size()];

  for( int i=0; i<attributes.size(); ++i ){
    /* status = 0; */
    c_attributes[i] = attributes[i].c_str();
    auto tfc = get_type_tuple(column_index(attributes[i]));
    attribute_types.push_back(tfc);
    c_types[i] = std::get<0>(tfc);
  }

  for( int i=0; i<dimensions.size(); ++i ){
    c_dimensions[i] = dimensions[i].c_str();
    int colnum = column_index(dimensions[i]);
    auto tfc = get_type_tuple(colnum);
    dimension_types.push_back(tfc);
    auto pair = get_domain(colnum,std::get<1>(tfc));
    c_domain[(2*i)] = pair.first;
    c_domain[(2*i)+1] = pair.second;
  }

  c_types[attributes.size()+1] = std::get<0>(dimension_types[0]);

}   /* -----  end of method Fits2tile::Fits2tile  ----- */



TileDB_ArraySchema
Fits2tile::fill_schema ( TileDB_ArraySchema user_schema )
{
  TileDB_ArraySchema f_array_schema;
  tiledb_array_set_schema(
      &f_array_schema,
      user_schema.array_name_,
      c_attributes,
      attributes.size(),
      user_schema.capacity_,
      user_schema.cell_order_,
      user_schema.cell_val_num_,
      user_schema.compression_,
      user_schema.dense_,
      c_dimensions,
      dimensions.size(),
      c_domain,
      dimensions.size()*2*sizeof(double),
      user_schema.tile_extents_,
      dimensions.size()*sizeof(double),
      user_schema.tile_order_,
      c_types
      );
  return f_array_schema;
}   /* -----  end of method Fits2tile::fill_schema  ----- */

std::pair<int,size_t>
Fits2tile::ftype( int colnum )
{
  int typecode;
  long width;
  fits_get_coltype( infile, colnum, &typecode, NULL, &width, &status);
  check_fits_status(&"Could not get type for column "[colnum]);
  return std::make_pair(typecode,static_cast<size_t>(width));
}

Fits2tile::type_tuple
Fits2tile::get_type_tuple(int colnum)
{
  auto pair = ftype(colnum);
  int tile_type;
  switch(pair.first){
    case TBYTE     : tile_type = TILEDB_CHAR;
                     break;
    case TINT      : tile_type = TILEDB_INT32;
                     break;
    case TLONGLONG : tile_type = TILEDB_INT64;
                     break;
    case TFLOAT    : tile_type = TILEDB_FLOAT32;
                     break;
    case TDOUBLE   : tile_type = TILEDB_FLOAT64;
                     break;
    default        : tile_type = TILEDB_FLOAT64;
                     break;
  }
  return std::make_tuple(tile_type,pair.first,pair.second);
}

int
Fits2tile::column_index( std::string column_name ) {
  int colnum;
  fits_get_colnum(
      infile,
      0,
      const_cast<char*>(column_name.c_str()),
      &colnum,
      &status);
  check_fits_status("Error locating column " +column_name +" in FITS file");
  return colnum;
}

std::pair<double,double>
Fits2tile::get_domain( int colnum , int typecode )
{
  double min_val;
  double max_val;
  std::string minkey = "TLMIN"+std::to_string(colnum);
  std::string maxkey = "TLMAX"+std::to_string(colnum);
  fits_read_key(
      infile,
      typecode,
      minkey.c_str(),
      &min_val,
      NULL,
      &status);
  check_fits_status("Error reading key" + minkey +" in FITS file");

  fits_read_key(
      infile,
      typecode,
      maxkey.c_str(),
      &max_val,
      NULL,
      &status);
  check_fits_status("Error reading key" + maxkey +" in FITS file");

  return std::make_pair(min_val,max_val);
}

void
Fits2tile::check_fits_status( std::string errmsg ){
  if(status){
    fits_report_error(stderr, status);
    status = 0;
    throw std::runtime_error(errmsg);
  }
}

//@TODO add safety checks for malloc success.
Fits2tile::buffer_pair
Fits2tile::allocate_column_buffer(type_tuple tt){
  size_t width = std::get<2>(tt);
  void* column_buffer = malloc(nrows*width);
  return std::make_pair(column_buffer, nrows*width);
}

void
Fits2tile::read_column(int colnum, type_tuple tt, buffer_pair bufp){
  fits_read_col(
      infile,
      std::get<1>(tt),
      1,1,
      colnum,
      bufp.second,
      NULL,
      bufp.first,
      NULL,
      &status);
  check_fits_status("Error Reading Column"+std::to_string(colnum));
}

void
Fits2tile::write_array( TileDB_Array& tiledb_array ){
  std::vector<buffer_pair> attrib_bufs;
  for(int i=0; i<attributes.size(); ++i){
    attrib_bufs.push_back(allocate_column_buffer(attribute_types[i]));
  }
  std::vector<buffer_pair> dimen_bufs;
  for(int i=0; i<dimensions.size(); ++i){
    dimen_bufs.push_back(allocate_column_buffer(dimension_types[i]));
  }
  for(int i=0; i<nrows; ++i){
    for(int j=0; j<dimensions.size(); ++j){
      buffer_coords[(i*dimensions.size())+j] =
    }
  }
  const void* buffers[] = { nonce, buffer_coords };
  size_t buffer_sizes[] = { nrows*sizeof(char), 3*nrows*sizeof(double) };
  std::cout << "Write Array Buffer " << std::endl;
  try{
    tiledb_array_write(tiledb_array, buffers, buffer_sizes);
  } catch (std::exception e){
    std::cout << e.what() << std::endl;
    return 1;
  }
}
