#include "tiledb.h"
#include <vector>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <chrono>

int main() {
  // Initialize context with the default configuration parameters
  TileDB_CTX* tiledb_ctx;
  tiledb_ctx_init(&tiledb_ctx, NULL);

  // Subarray and attributes
  double subarray[] = {355.0,360.0, 175.0,180.0};
  double subarray1[] = {45.0,55.0, 5.0,180.0};
  double subarray2[] = {5.0,355.0, 175.0,180.0};
  double subarray3[] = {200.0,270.0, 75.0,105.0};
  double subarray4[] = {355.0,360.0, 90.0,180.0};
  double subarray5[] = {110.0,150.0, 55.0,140.0};
  std::vector<double*> queries {
      subarray, subarray1, subarray2,
      subarray3, subarray4, subarray5
  };
  const char* attrib[] = { "energy" };

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
  int test_iterations = 1;
  // Prepare cell buffers
  size_t nrows = 1024;
  float *buffer_energy = new float[nrows];
  void* buffers[] = { buffer_energy };
  size_t buffer_sizes[] = { nrows*sizeof(float) };
  std::chrono::time_point<std::chrono::steady_clock> start, end;
  /* std::vector<std::chrono::duration<double>> results(queries.size()); */
  /* for (auto r : results) r= std::chrono::duration<double>::zero(); */

  for(int i=0; i<test_iterations; ++i){
    for(int j=0; j<queries.size(); ++j){
      tiledb_array_reset_subarray(tiledb_array, queries[j]);
      start = std::chrono::steady_clock::now();
      do {
        tiledb_array_read(tiledb_array, buffers, buffer_sizes);
      } while(tiledb_array_overflow(tiledb_array, 0) == 1);
      end = std::chrono::steady_clock::now();
      std::chrono::duration<double> r = end-start;
      std::cout << (std::chrono::duration<double, std::milli>(r).count() / test_iterations) <<std::endl;
      std::memset(buffer_energy, 0.0, nrows*sizeof(float));
      /* std::cout << elapsed_seconds.count() << std::endl; */
      /* results[j] += elapsed_seconds; */
    }
  }

  // Finalize the array
  tiledb_array_finalize(tiledb_array);

  /* Finalize context. */
  tiledb_ctx_finalize(tiledb_ctx);

  /* for (auto r : results) { */
  /*   std::cout << (std::chrono::duration<double, std::milli>(r).count() / test_iterations) <<std::endl; */
  /* } */

  return 0;
}
