#include "fitsio.h"
#include "tiledb.h"
#include <stdlib.h>
#include <iostream>
#include <cstdio>

int main() {
  // Initialize context with the default configuration parameters
  TileDB_CTX* tiledb_ctx;
  tiledb_ctx_init(&tiledb_ctx, NULL);

  // Initialize array
  TileDB_Array* tiledb_array;
  tiledb_array_init(
      tiledb_ctx,                                       // Context
      &tiledb_array,                                    // Array object
      "my_workspace/sparse_arrays/my_array_B",          // Array name
      TILEDB_ARRAY_READ,                                // Mode
      NULL,                                             // Whole domain
      NULL,                                             // All attributes
      0);                                               // Number of attributes

  fitsfile *fptr;
  int status = 0;
  long nelems;
  char filename[] = "L170410143649D9656A7F40_PH00.fits";
  fits_open_table( &fptr, filename, READONLY, &status);
  fits_get_num_rows(fptr, &nelems, &status);
  if(status){
    fits_report_error(stderr, status);
    return -1;
  }
  size_t nrows = static_cast<size_t>(nelems);

  // Prepare cell buffers
  std::cout << "Prepare buffers" << std::endl;
  char* buffer_a1 = (char*) malloc(sizeof(char)*nrows);
  int* buffer_coords = (int*) malloc(2*sizeof(int)*nrows);
  void* buffers[] = { buffer_a1 , buffer_coords };
  size_t buffer_sizes[] = { nrows, 2*nrows*sizeof(int) };

  std::cout << "Read From Array" << std::endl;
  tiledb_array_read(tiledb_array, buffers, buffer_sizes);
  std::cout << "Output From Array" << std::endl;
  // Print cell values
  printf("coords\t a1\n");
  printf("--------------------------------------------------\n");
  for(int i=0; i<nrows; ++i) {
    std::cout
      <<  buffer_coords[2*i]
      << ", " << buffer_coords[(2*i)+1]
      << "\t\t" << buffer_a1[i] << std::endl;
  }
  free(buffer_a1);
  free(buffer_coords);
  tiledb_array_finalize(tiledb_array);
  tiledb_ctx_finalize(tiledb_ctx);

  return 0;
}
