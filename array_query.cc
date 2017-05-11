#include "tiledb.h"
#include <vector>
#include <iostream>
#include <cstdio>
#include <chrono>

int main() {
  // Initialize context with the default configuration parameters
  TileDB_CTX* tiledb_ctx;
  tiledb_ctx_init(&tiledb_ctx, NULL);

  // Subarray and attributes
  double subarray[] = {355.0,360.0, 175.0,180.0};
  const char* attrib[] = { "energy" };

  /* std::vector<std::tuple<double,double,double,double>> queries; */
  /* queries.push_back(355,360,175,180]); */

  // Initialize array
  TileDB_Array* tiledb_array;
  tiledb_array_init(
      tiledb_ctx,                                      // Context
      &tiledb_array,                                   // Array object
      "my_workspace/sparse_arrays/my_array_B",         // Array name
      TILEDB_ARRAY_READ,                               // Mode
      subarray,                                        // Constrain in subarray
      attrib,                                      // Subset on attributes
      1);                                              // Number of attributes

  // Prepare cell buffers
  size_t nrows = 64;
  float *buffer_energy = new float[nrows];
  void* buffers[] = { buffer_energy };
  size_t buffer_sizes[] = { nrows*sizeof(float) };
  std::chrono::time_point<std::chrono::system_clock> start, end;

  for(int j=0; j<10; ++j){
    /* tiledb_array_reset_subarray(); */
    start = std::chrono::system_clock::now();
    do {
      tiledb_array_read(tiledb_array, buffers, buffer_sizes);

    } while(tiledb_array_overflow(tiledb_array, 0) == 1);

    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << elapsed_seconds.count() << std::endl;
  }

  // Finalize the array
  tiledb_array_finalize(tiledb_array);

  /* Finalize context. */
  tiledb_ctx_finalize(tiledb_ctx);

  return 0;
}
