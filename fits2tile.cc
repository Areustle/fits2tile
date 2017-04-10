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
    std::vector<std::string> dimensions,
    std::vector<std::string> attributes
    ):infile(input_file), status(0)
{
  fits_get_num_rows(infile, &nrows, &status);
  check_fits_status("Error Opening FITS File");

  //First throwables passed, begin 'new'ing.
  c_attributes  = new const char*[attributes.size()];
  c_dimensions  = new const char*[dimensions.size()];
  c_domain      = new double[2*dimensions.size()];
  c_types       = new int[1+attributes.size()];

  for( int i=0; i<attributes.size(); ++i ){
    /* status = 0; */
    c_attributes[i] = attributes[i].c_str();
    auto ctfs = get_meta_tuple(column_index(attributes[i]));
    attribute_types.push_back(ctfs);
    c_types[i] = std::get<1>(ctfs);
  }

  for( int i=0; i<dimensions.size(); ++i ){
    c_dimensions[i] = dimensions[i].c_str();
    auto ctfs = get_meta_tuple(column_index(dimensions[i]));
    dimension_types.push_back(ctfs);
    auto pair = get_domain(std::get<0>(ctfs),std::get<2>(ctfs));
    c_domain[(2*i)] = pair.first;
    c_domain[(2*i)+1] = pair.second;
  }

  c_types[attributes.size()+1] = std::get<1>(dimension_types[0]);
  /* buffers = new const void*[1 + attribute_types.size()]; */
  /* buffer_sizes = new size_t[1 + attribute_types.size()]; */

}   /* -----  end of method Fits2tile::Fits2tile  ----- */

TileDB_ArraySchema
Fits2tile::fill_schema ( TileDB_ArraySchema user_schema )
{
  TileDB_ArraySchema f_array_schema;
  tiledb_array_set_schema(
      &f_array_schema,
      user_schema.array_name_,
      c_attributes,
      attribute_types.size(),
      user_schema.capacity_,
      user_schema.cell_order_,
      user_schema.cell_val_num_,
      user_schema.compression_,
      user_schema.dense_,
      c_dimensions,
      dimension_types.size(),
      c_domain,
      dimension_types.size()*2*sizeof(double),
      user_schema.tile_extents_,
      dimension_types.size()*sizeof(double),
      user_schema.tile_order_,
      c_types
      );
  return f_array_schema;
}   /* -----  end of method Fits2tile::fill_schema  ----- */

Fits2tile::meta_tuple
Fits2tile::get_meta_tuple(int colnum)
{
  int typecode;
  long width;
  fits_get_coltype( infile, colnum, &typecode, NULL, &width, &status);
  check_fits_status(&"Could not get type for column "[colnum]);
  int tile_type;
  switch(typecode){
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
  return std::make_tuple(colnum,tile_type,typecode,static_cast<size_t>(width));
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
Fits2tile::alloc_col_buf(meta_tuple tt){
  size_t width = std::get<3>(tt);
  void* column_buffer = malloc(nrows*width);
  return std::make_pair(column_buffer, nrows*width);
}

void
Fits2tile::read_dim_column(meta_tuple tt, double* dimbuf){
  fits_read_col_dbl(
      infile,
      std::get<0>(tt),
      1,
      1,
      nrows,
      0.0,
      dimbuf,
      NULL,
      &status);
  check_fits_status("Error Reading Column"+std::to_string(std::get<0>(tt)));
}

void
Fits2tile::read_attr_column(meta_tuple tt, void* bufp){
  fits_read_col(
      infile,
      std::get<2>(tt),
      std::get<0>(tt),
      1,
      1,
      nrows,
      NULL,
      bufp,
      NULL,
      &status);
  check_fits_status("Error Reading Column"+std::to_string(std::get<0>(tt)));
}

/* void */
/* Fits2tile::write_array( TileDB_Array& tiledb_array ){ */
/*   for( auto att : attribute_types ){ */
/*     auto bufp = alloc_col_buf(att); */
/*     read_column(att, bufp); */
/*     abuf.push_back(bufp); */
/*   } */
/*   for ( auto dtt : dimension_types){ */
/*     auto bufp = alloc_col_buf(dtt); */
/*     read_column(dtt, bufp); */
/*     dbuf.push_back(bufp); */
/*   } */
/*   double* buffer_coords = (double*)malloc(dbuf.size()*nrows*sizeof(double)); */
/*   for( int j=0; j<nrows; ++j){ */
/*     for (int k=0; k<dbuf.size(); ++k){ */
/*       buffer_coords[(j*dbuf.size())+k] = ((dbuf[k].second*)dbuf[k].first)[j]; */
/*     } */
/*   } */

/*   for(int i=0; i<nrows; ++i){ */
/*     for(int j=0; j<dimensions.size(); ++j){ */
/*       buffer_coords[(i*dimensions.size())+j] = */
/*     } */
/*   } */
/*   const void* buffers[] = { nonce, buffer_coords }; */
/*   size_t buffer_sizes[] = { nrows*sizeof(char), 3*nrows*sizeof(double) }; */
/*   std::cout << "Write Array Buffer " << std::endl; */
/*   try{ */
/*     tiledb_array_write(tiledb_array, buffers, buffer_sizes); */
/*   } catch (std::exception e){ */
/*     std::cout << e.what() << std::endl; */
/*     return 1; */
/*   } */
/* } */
