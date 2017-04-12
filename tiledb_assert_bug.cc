/**
 * @file   tiledb_array_create_sparse.cc
 *
 * @section LICENSE
 *
 * The MIT License
 *
 * @copyright Copyright (c) 2016 MIT and Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @section DESCRIPTION
 *
 * It shows how to create a sparse array.
 */

#include "tiledb.h"
#include <cstdio>

int main() {
  // Initialize context with the default configuration parameters
  TileDB_CTX* tiledb_ctx;
  tiledb_ctx_init(&tiledb_ctx, NULL);

  // Create a workspace
  tiledb_workspace_create(tiledb_ctx, "my_workspace");
  tiledb_group_create(tiledb_ctx, "my_workspace/sparse_arrays");


  // Prepare parameters for array schema
  const char* array_name = "my_workspace/sparse_arrays/my_array_B";
  const char* attributes[] = { "a1" };  // Three attributes
  const char* dimensions[] = { "d1", "d2" };        // Two dimensions
  float domain[] =
  {
      1, 4,                       // d1
      1, 4                        // d2
  };
  const int cell_val_num[] =
  {
      1,                          // a1
  };
  const int compression[] =
  {
        TILEDB_NO_COMPRESSION,    // a1
        TILEDB_NO_COMPRESSION     // coordinates
  };
  float tile_extents[] =
  {
      1,                          // d1
      1                           // d2
  };
  const int types[] =
  {
      TILEDB_INT32,               // a1
      TILEDB_INT64                // coordinates
  };

  // Set array schema
  TileDB_ArraySchema array_schema;
  tiledb_array_set_schema(
      &array_schema,              // Array schema struct
      array_name,                 // Array name
      attributes,                 // Attributes
      1,                          // Number of attributes
      2,                          // Capacity
      TILEDB_ROW_MAJOR,           // Cell order
      cell_val_num,               // Number of cell values per attribute
      compression,                // Compression
      0,                          // Sparse array
      dimensions,                 // Dimensions
      2,                          // Number of dimensions
      domain,                     // Domain
      4*sizeof(float),          // Domain length in bytes
      tile_extents,               // Tile extents
      2*sizeof(float),          // Tile extents length in bytes
      TILEDB_ROW_MAJOR,           // Tile order
      types                       // Types
  );

  // Create array
  tiledb_array_create(tiledb_ctx, &array_schema);
  tiledb_array_free_schema(&array_schema);

  // Write to the array
  TileDB_Array* tiledb_write_array;
  tiledb_array_init(
      tiledb_ctx,                                // Context
      &tiledb_write_array,                             // Array object
      "my_workspace/sparse_arrays/my_array_B",   // Array name
      TILEDB_ARRAY_WRITE,                        // Mode
      NULL,                                      // Entire domain
      NULL,                                      // All attributes
      0);                                       // Number of attributes
  int buffer_write_a1[] = { 0, 1, 2 };
  float buffer_write_coords[] = {
    1, 1,
    3, 1,
    3, 3,
  };
  const void* buffers_write[] = {
    buffer_write_a1,
    buffer_write_coords
  };
  size_t buffer_sizes_write[] = {
      sizeof(buffer_write_a1),
      sizeof(buffer_write_coords)
  };
  tiledb_array_write(tiledb_write_array, buffers_write, buffer_sizes_write);
  tiledb_array_finalize(tiledb_write_array);

  // Read From the array
  ///////////////////////////////////////////////////
  ///////// There are no cells in this region ///////
  ///////////////////////////////////////////////////
  float subarray[] = { 1,3 , 2,4 };
  ///////////////////////////////////////////////////
  ///////////////////////////////////////////////////

  TileDB_Array* tiledb_read_array;
  tiledb_array_init(
      tiledb_ctx,                               // Context
      &tiledb_read_array,                       // Array object
      "my_workspace/sparse_arrays/my_array_B",  // Array name
      TILEDB_ARRAY_READ,                        // Mode
      subarray,                                 // Entire domain
      attributes,                               // All attributes
      0);                                       // Number of attributes

  int buffer_a1_read[3];
  void* buffers_read[] = { buffer_a1_read };
  size_t buffer_sizes_read[] = { sizeof(buffer_a1_read) };

  // Loop until no overflow
  printf(" a1\n----\n");
  do {
    printf("Reading cells...\n");

    // Read from array
    tiledb_array_read(tiledb_read_array, buffers_read, buffer_sizes_read);

    // Print cell values
    float result_num = buffer_sizes_read[0] / sizeof(int);
    for(int i=0; i<result_num; ++i) {
      if(buffer_a1_read[i] != TILEDB_EMPTY_INT32) // Check for deletion
        printf("%3d\n", buffer_a1_read[i]);
    }
  } while(tiledb_array_overflow(tiledb_read_array, 0) == 1);

  tiledb_array_finalize(tiledb_read_array);
  tiledb_ctx_finalize(tiledb_ctx);

  return 0;
}
