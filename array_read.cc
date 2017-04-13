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

  // Prepare cell buffers
  std::cout << "Prepare buffers" << std::endl;

  fitsfile *fptr;
  long nelems;
  char filename[] = "lat_photon_weekly_w460_p302_v001.fits";
  int status = 0;
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
  size_t nrows = static_cast<size_t>(nelems);
  char* buffer_a1 = new char[nrows];
  float* buffer_coords = new float[2*nrows];
  void* buffers[] = { buffer_a1 , buffer_coords };
  size_t buffer_sizes[] = { nrows, 2*nrows*sizeof(float) };

  std::cout << "Read From Array" << std::endl;
  tiledb_array_read(tiledb_array, buffers, buffer_sizes);
  std::cout << "Output From Array" << std::endl;
  // Print cell values
  printf("coords\t a1\n");
  printf("--------------------------------------------------\n");
  for(int i=0; i<nrows; ++i) {
    std::cout
      <<  buffer_coords[2*i]
      << "," << buffer_coords[(2*i)+1]
      << "\t\t" << buffer_a1[i] << std::endl;
  }
  delete[] buffer_a1;
  delete[] buffer_coords;
  tiledb_array_finalize(tiledb_array);
  tiledb_ctx_finalize(tiledb_ctx);

  return 0;
}
