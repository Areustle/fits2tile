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
  char* buffer_a1 = new char[150561];
  /* float* buffer_coords = new float[2*150561]; */
  void* buffers[] = { buffer_a1 };
  size_t buffer_sizes[] = {
    150561
    /* 16*150561 */
  };

  /* char buffer_var_a2[30]; */
  /* std::cout << sizeof(buffer_var_a2) << std::endl; */
  std::cout << "Read From Array" << std::endl;
  // Read from array
  tiledb_array_read(tiledb_array, buffers, buffer_sizes);

  std::cout << "Output From Array" << std::endl;
  // Print cell values
  int64_t result_num = 150561;
  printf("coords\t a1\n");
  printf("--------------------------------------------------\n");
  for(int i=0; i<result_num; ++i) {
    std::cout
      /* <<  buffer_coords[2*i] */
      /* << ", " << buffer_coords[(2*i)+1] */
      /* << ", " << buffer_coords[2*i+2] */
      << "\t\t" << buffer_a1[i] << std::endl;
  }

  delete[] buffer_a1;
  /* delete[] buffer_coords; */
  // Finalize the array
  tiledb_array_finalize(tiledb_array);

  // Finalize context
  tiledb_ctx_finalize(tiledb_ctx);

  return 0;
}
