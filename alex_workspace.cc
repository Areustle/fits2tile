#include "tiledb.h"

int main() {
  TileDB_CTX* tiledb_ctx;
  tiledb_ctx_init(&tiledb_ctx, NULL);
  tiledb_workspace_create(tiledb_ctx, "my_workspace");
  tiledb_group_create(tiledb_ctx, "my_workspace/dense_arrays");
  tiledb_group_create(tiledb_ctx, "my_workspace/sparse_arrays");
  tiledb_ctx_finalize(tiledb_ctx);
  return 0;
}
