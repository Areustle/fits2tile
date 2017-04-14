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

  char* nonce= (char *) malloc(nelems * sizeof(char));
  float* fra   = (float *) malloc(nelems * sizeof(float));
  float* fdecl = (float *) malloc(nelems * sizeof(float));
  int64_t* buffer_coords = (int64_t *) malloc(2 * nelems * sizeof(int64_t));

  std::cout << nelems << std::endl;
  std::cout << "RA " << std::endl;
  fits_read_col_flt(fptr, 2, 1, 1, nelems, 7.0, fra, &anynull, &status);

  std::cout << "DEC " << std::endl;
  fits_read_col_flt(fptr, 3, 1, 1, nelems, 7.0, fdecl, &anynull, &status);

  std::cout << "Buffer Merge" << std::endl;
  for(int j=0; j<nelems; ++j){
      nonce[j] = 'a';
      buffer_coords[(j*2)+0] = int64_t(1000000*fra[j]);
      buffer_coords[(j*2)+1] = int64_t(1000000*fdecl[j])+90000000;
  }

  /* bool dups_found = false; */
  /* for (int i=0; i<nelems-1; ++i){ */
  /*   for (int j=i+1; j<nelems; ++j){ */
  /*     if ((buffer_coords[2*i] == buffer_coords[2*j]) */
  /*         && */
  /*         (buffer_coords[(2*i)+1] == fdecl[(2*j)+1])){ */
  /*       dups_found = true; */
  /*       std::cout << "Check with == found \n" */
  /*         << "("<< i     << "," << j       << ") \n" */
  /*         << "("<< fra[i] << "," << fdecl[i] << ") \n" */
  /*         << "("<< fdecl[j] << "," << fdecl[j] << ") \n" */
  /*         << std::endl; */
  /*     } */
  /*     if ((std::memcmp(&fra[i], &fra[j], sizeof(float)) == 0) && */
  /*         (std::memcmp(&fdecl[i], &fdecl[j], sizeof(float)) == 0)){ */
  /*       dups_found = true; */
  /*       std::cout << "Check with memcmp found \n" */
  /*         << "("<< i     << "," << j       << ") \n" */
  /*         << "("<< fra[i] << "," << fdecl[i] << ") \n" */
  /*         << "("<< fdecl[j] << "," << fdecl[j] << ") \n" */
  /*         << std::endl; */
  /*     } */
  /*   } */
  /* } */

  const void* buffers[] = { nonce, buffer_coords };
  size_t buffer_sizes[] = { nelems*sizeof(char), 2*nelems*sizeof(int64_t) };
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
  free(buffer_coords);

  tiledb_ctx_finalize(tiledb_ctx);
  fits_close_file(fptr, &status);
}
