#include <limits>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include "fitsio.h"
#include "tiledb.h"
#include "fits2tile.h"

int main(){

  // === Write TileDB Array ===
  // Initialize context with the default configuration parameters
  TileDB_CTX* tiledb_ctx;
  tiledb_ctx_init(&tiledb_ctx, NULL);

  // Prepare parameters for array schema
  const char* array_name = "my_workspace/sparse_arrays/my_array_B";
  const char* attributes[] = { "a1" };
  const char* dimensions[] = {"ra", "dec"};
  float domain[] = { 0.0, 360.0, -90.0, 90.0 };
  const int cell_val_num[] = {1};
  const int compression[] = { TILEDB_NO_COMPRESSION, TILEDB_NO_COMPRESSION };
  float tile_extents[] = { 360.0, 180.0 };
  const int types[] = { TILEDB_CHAR, TILEDB_FLOAT32 };

  TileDB_ArraySchema array_schema;
  tiledb_array_set_schema(
      &array_schema,    // Array schema struct
      array_name,       // Array name
      attributes,       // Attributes
      1,                // Number of attributes
      4096,                // Capacity
      TILEDB_ROW_MAJOR, // Cell order
      cell_val_num,     // Number of cell values per attribute
      compression,      // Compression
      0,                // Sparse array
      dimensions,       // Dimensions
      2,                // Number of dimensions
      domain,           // Domain
      4*sizeof(float),  // Domain array length in bytes
      tile_extents,     // Tile extents
      2*sizeof(float),  // Tile extents length in bytes
      TILEDB_ROW_MAJOR, // Tile order
      types             // Types
  );

  // Create array
  tiledb_array_create(tiledb_ctx, &array_schema);

  tiledb_array_free_schema(&array_schema);

  tiledb_ctx_finalize(tiledb_ctx);

  return 0;
}
