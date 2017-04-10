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
  char* buffer_a1 = new char[32];
  double* buffer_coords = new double[3*32];
  void* buffers[] = { buffer_a1, buffer_coords };
  size_t buffer_sizes[] = { sizeof(buffer_a1), sizeof(buffer_coords) };

  std::cout << "Read From Array" << std::endl;
  // Read from array
  tiledb_array_read(tiledb_array, buffers, buffer_sizes);

  std::cout << "Output From Array" << std::endl;
  // Print cell values
  int64_t result_num = 32;
  printf("coords\t a1\n");
  printf("--------------------------------------------------\n");
  for(int i=0; i<result_num; ++i) {
    printf("(%f, %f, %f)", buffer_coords[3*i], buffer_coords[3*i+1], buffer_coords[3*i+2]);
    printf("\t %c\n", buffer_a1[i]);
  }

  delete[] buffer_a1;
  delete[] buffer_coords;
  // Finalize the array
  tiledb_array_finalize(tiledb_array);

  // Finalize context
  tiledb_ctx_finalize(tiledb_ctx);

  return 0;
}
