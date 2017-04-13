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

int main() {

  TileDB_CTX* tiledb_ctx;
  tiledb_ctx_init(&tiledb_ctx, NULL);

  // Initialize array
  TileDB_Array* tiledb_array;
  tiledb_array_init(
      tiledb_ctx,                                // Context
      &tiledb_array,                             // Array object
      "my_workspace/sparse_arrays/my_array_B",   // Array name
      TILEDB_ARRAY_WRITE_UNSORTED,                        // Mode
      NULL,                                      // Entire domain
      NULL,                                      // All attributes
      0);                                        // Number of attributes

  fitsfile *fptr;
  long nelems;
  char *nonce;
  int status = 0, anynull = 0;
  char filename[] = "L170410143649D9656A7F40_PH00.fits";
  //OPEN TABLE
  fits_open_table( &fptr, filename, READONLY, &status);
  if (status) {
      fits_report_error(stdout, status);
      return 7;
  }

  fits_get_num_rows(fptr, &nelems, &status);
  if(status){
    fits_report_error(stderr, status);
    return 13;
  }

  nonce= (char *) malloc(nelems * sizeof(char));
  float* fra   = (float *) malloc(nelems * sizeof(float));
  float* fdecl = (float *) malloc(nelems * sizeof(float));
  int* ira   = (int *) malloc(nelems * sizeof(int));
  int* idecl = (int *) malloc(nelems * sizeof(int));
  int* buffer_coords = (int *) malloc(2 * nelems * sizeof(int));

  std::cout << sizeof(int) << std::endl;
  std::cout << sizeof(float) << std::endl;
  std::cout << nelems << std::endl;
  std::cout << "RA " << std::endl;
  fits_read_col_flt(fptr, 2,              //RA
          1, 1, nelems, 7.0,
          fra, &anynull, &status);

  std::cout << "DEC " << std::endl;
  fits_read_col_flt(fptr, 3,              //DEC
          1, 1, nelems, 7.0,
          fdecl, &anynull, &status);

  std::cout << "reinterpreting floats as ints" << std::endl;
  std::memcpy(ira, fra, nelems*sizeof(int));
  std::memcpy(idecl, fdecl, nelems*sizeof(int));

  std::cout << "Buffer Merge" << std::endl;
  for(int j=0; j<nelems; ++j){
      nonce[j] = 'a';
      buffer_coords[(j*2)+0] = ira[j];
      buffer_coords[(j*2)+1] = idecl[j];
      std::cout << ira[j]<<","<<idecl[j]<<std::endl;
  }


  const void* buffers[] = { nonce, buffer_coords };
  size_t buffer_sizes[] = { nelems*sizeof(char), 2*nelems*sizeof(int) };
  std::cout << "Write Array Buffer " << std::endl;
  try{
    tiledb_array_write(tiledb_array, buffers, buffer_sizes);
  } catch (std::exception e){
    std::cout << e.what() << std::endl;
    return 1;
  }

  tiledb_array_finalize(tiledb_array);
  free(nonce);
  free(fra);
  free(fdecl);
  free(ira);
  free(idecl);
  free(buffer_coords);

  tiledb_ctx_finalize(tiledb_ctx);
  fits_close_file(fptr, &status);
}
