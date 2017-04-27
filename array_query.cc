#include "tiledb.h"
#include <cstdio>

int main() {
  // Initialize context with the default configuration parameters
  TileDB_CTX* tiledb_ctx;
  tiledb_ctx_init(&tiledb_ctx, NULL);

  // Subarray and attributes
  float subarray[] = {
    180,190    //RA lower,upper bound
    ,
    0,5        //Dec lower,upper bound
  };
  const char* attributes[] = { "met" };

  // Initialize array
  TileDB_Array* tiledb_array;
  tiledb_array_init(
      tiledb_ctx,                                      // Context
      &tiledb_array,                                   // Array object
      "my_workspace/sparse_arrays/my_array_B",         // Array name
      TILEDB_ARRAY_READ,                               // Mode
      subarray,                                        // Constrain in subarray
      attributes,                                      // Subset on attributes
      1);                                              // Number of attributes

  // Prepare cell buffers
  double buffer_a1[4096];
  float  buffer_coords[4096];
  void* buffers[] = { buffer_a1, buffer_coords };
  size_t buffer_sizes[] = { sizeof(buffer_a1), sizeof(buffer_coords) };


  // Loop until no overflow
  printf(" met\n----\n");
    printf("\nReading cells...\n");
  do {
    /* printf("\nReading cells...\n"); */

    // Read from array
    tiledb_array_read(tiledb_array, buffers, buffer_sizes);

    // Print cell values
    int64_t result_num = buffer_sizes[0] / sizeof(double);
    for(int i=0; i<result_num; ++i) {
      if(buffer_a1[i] != TILEDB_EMPTY_FLOAT64) // Check for deletion
        printf("%.1f\n", buffer_a1[i]);
    }
  } while(tiledb_array_overflow(tiledb_array, 0) == 1);

  // Finalize the array
  tiledb_array_finalize(tiledb_array);

  /* Finalize context. */
  tiledb_ctx_finalize(tiledb_ctx);

  return 0;
}
