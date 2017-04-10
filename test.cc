/*
 * ============================================================================
 *
 *       Filename:  test.cc
 *
 *    Description:  testing fitsio and tiledb
 *
 *        Version:  1.0
 *        Created:  03/29/2017 21:18:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * ============================================================================
 */
#include <limits>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include "fitsio.h"
#include "tiledb.h"
#include "fits2tile.h"

int main(){

  // === Write TileDB Array ===
  // Initialize context with the default configuration parameters
  TileDB_CTX* tiledb_ctx;
  tiledb_ctx_init(&tiledb_ctx, NULL);

  // Prepare parameters for array schema
  const char* array_name = "my_workspace/sparse_arrays/my_array_B";
  const char* attributes[] = { "energy" };
  const char* dimensions[] = {"ra", "dec"};
  float domain[] = {
    /* 0.0, std::numeric_limits<double>::max(), */
    0.0, 360.0,
    -90.0, 90.0
  };
  const int cell_val_num[] = {1};
  const int compression[] =
  {
    TILEDB_NO_COMPRESSION,
    TILEDB_NO_COMPRESSION
  };
  float tile_extents[] =
  {
      /* 5400.0,   //90 Minutes in seconds */
      8.0,      //8 degrees of arc
      4.0       //4 degrees of arc
  };
  const int types[] =
  {
    TILEDB_CHAR,
    TILEDB_FLOAT32
  };

  // Set array schema
  TileDB_ArraySchema array_schema;
  tiledb_array_set_schema(
      &array_schema,    // Array schema struct
      array_name,       // Array name
      attributes,       // Attributes
      1,                // Number of attributes
      32,                // Capacity
      TILEDB_ROW_MAJOR, // Cell order
      cell_val_num,     // Number of cell values per attribute
      compression,      // Compression
      0,                // Sparse array
      dimensions,       // Dimensions
      2,                // Number of dimensions
      domain,           // Domain
      4*sizeof(float), // Domain array length in bytes
      tile_extents,     // Tile extents
      2*sizeof(float), // Tile extents length in bytes
      TILEDB_ROW_MAJOR, // Tile order
      types             // Types
  );

  fitsfile *fptr;
  int status = 0, anynull = 0;
  char filename[] = "L170410105950F748C67F80_PH00.fits";
  //OPEN TABLE
  fits_open_table( &fptr, filename, READONLY, &status);
  if (status) {
      fits_report_error(stdout, status);
      return 7;
  }

  /* std::vector<std::string> dims = {"TIME", "RA", "DEC"}; */
  /* std::vector<std::string> attr = {"ENERGY"}; */
  /* Fits2tile ff(fptr, dims, attr); */
  /* array_schema = ff.fill_schema(array_schema); */

  // Create array
  tiledb_array_create(tiledb_ctx, &array_schema);

  // === Read Fits Columns ===
  // GET NUMBER OF ROWS
  /* long nelems = ff.get_row_count(); */
  long nelems = 150561;

  TileDB_Array* tiledb_array;
  tiledb_array_init(
      tiledb_ctx,                                // Context
      &tiledb_array,                             // Array object
      "my_workspace/sparse_arrays/my_array_B",   // Array name
      TILEDB_ARRAY_WRITE_UNSORTED,                        // Mode
      NULL,                                      // Entire domain
      NULL,                                      // All attributes
      0);                                        // Number of attributes

  float doublenull, *met, *buffer_coords, *decl, *ra;
  float floatnull;
  char *nonce;

  nonce= (char *) malloc(nelems * sizeof(char));
  /* met  = (double *) malloc(nelems * sizeof(double)); */
  ra   = (float *) malloc(nelems * sizeof(float));
  decl = (float *) malloc(nelems * sizeof(float));
  buffer_coords = (float *) malloc(2 * nelems * sizeof(float));

  //@TODO Read last bufmod elements.
  std::cout << nelems << std::endl;
  std::cout << "RA " << std::endl;
  fits_read_col_flt(fptr, 2,              //RA
          1, 1, nelems, 7.0,
          ra, &anynull, &status);

  std::cout << "DEC " << std::endl;
  fits_read_col_flt(fptr, 3,              //DEC
          1, 1, nelems, 7.0,
          decl, &anynull, &status);

  /* std::cout << "MET " << std::endl; */
  /* fits_read_col(fptr, TDOUBLE, 10,             //MET */
  /*         1, 1, nelems, &doublenull, */
  /*         met, &anynull, &status); */

  std::cout << "Buffer Merge" << std::endl;
  for(int j=0; j<nelems; ++j){
      nonce[j] = 'a';
      /* buffer_coords[(j*3)] = met[j]; */
      buffer_coords[(j*2)+0] = ra[j];
      buffer_coords[(j*2)+1] = decl[j];
  }

  for(int i=0; i<32; ++i){
    /* std::cout << ra[i] << "\t"; */
    /* std::cout << decl[i] << "\t"; */
    /* std::cout << met[i] << " \t\t"; */
    std::cout << nonce[i] << " ";
    for( int j=0; j<2; ++j){
      std::cout << buffer_coords[2*i+j] << " ";
    }
    std::cout << std::endl;
  }

  const void* buffers[] = { nonce, buffer_coords };
  size_t buffer_sizes[] = { nelems*sizeof(char), 2*nelems*sizeof(float) };
  std::cout << "Write Array Buffer " << std::endl;
  try{
    tiledb_array_write(tiledb_array, buffers, buffer_sizes);
  } catch (std::exception e){
    std::cout << e.what() << std::endl;
    return 1;
  }


  tiledb_array_free_schema(&array_schema);
  tiledb_array_finalize(tiledb_array);
  free(nonce);
  /* free(met); */
  free(ra);
  free(decl);
  free(buffer_coords);


  fits_close_file(fptr, &status);

  /* Finalize context. */
  tiledb_ctx_finalize(tiledb_ctx);

    return 0;
}
