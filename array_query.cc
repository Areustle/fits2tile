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

  // Initialize array
  TileDB_Array* tiledb_array;
  tiledb_array_init(
      tiledb_ctx,                                      // Context
      &tiledb_array,                                   // Array object
      "my_workspace/sparse_arrays/my_array_B",         // Array name
      TILEDB_ARRAY_READ,                               // Mode
      subarray,                                        // Constrain in subarray
      NULL,                                      // Subset on attributes
      0);                                              // Number of attributes

  // Prepare cell buffers
  size_t nrows = 64;
  float buffer_energy[nrows];
  float buffer_gallat[nrows];
  float buffer_gallong[nrows];
  float buffer_theta[nrows];
  float buffer_phi[nrows];
  float buffer_zenith_angle[nrows];
  float buffer_earth_az_ang[nrows];
  double buffer_met[nrows];
  int buffer_event_id[nrows];
  int buffer_run_id[nrows];
  int buffer_event_class[nrows];
  int buffer_event_type[nrows];
  double buffer_livetime[nrows];
  float  buffer_coords[2*nrows];

  void* buffers[] = {
    buffer_energy,
    buffer_gallat,
    buffer_gallong,
    buffer_theta,
    buffer_phi,
    buffer_zenith_angle,
    buffer_earth_az_ang,
    buffer_met,
    buffer_event_id,
    buffer_run_id,
    buffer_event_class,
    buffer_event_type,
    buffer_livetime,
    buffer_coords
  };
  size_t buffer_sizes[] = {
    sizeof(buffer_energy),
    sizeof(buffer_gallat),
    sizeof(buffer_gallong),
    sizeof(buffer_theta),
    sizeof(buffer_phi),
    sizeof(buffer_zenith_angle),
    sizeof(buffer_earth_az_ang),
    sizeof(buffer_met),
    sizeof(buffer_event_id),
    sizeof(buffer_run_id),
    sizeof(buffer_event_class),
    sizeof(buffer_event_type),
    sizeof(buffer_livetime),
    sizeof(buffer_coords)
  };

  // Loop until no overflow
  do {
    tiledb_array_read(tiledb_array, buffers, buffer_sizes);

    // Print cell values
    int64_t result_num = buffer_sizes[0] / sizeof(float);
    for(int i=0; i<result_num; ++i) {
      printf("%.1f,%.1f\t", buffer_coords[(2*i)+0],buffer_coords[(2*i)+1]);
      printf("%.1f ", buffer_energy[i]);
      printf("%.1f ", buffer_gallat[i]);
      printf("%.1f ", buffer_gallong[i]);
      printf("%.1f ", buffer_theta[i]);
      printf("%.1f ", buffer_phi[i]);
      printf("%.1f ", buffer_zenith_angle[i]);
      printf("%.1f ", buffer_earth_az_ang[i]);
      printf("%.1f ", buffer_met[i]);
      printf("%d ", buffer_event_id[i]);
      printf("%d ", buffer_run_id[i]);
      printf("%d ", buffer_event_class[i]);
      printf("%d ", buffer_event_type[i]);
      printf("%.1f\n", buffer_livetime[i]);
    }
  } while(tiledb_array_overflow(tiledb_array, 0) == 1);

  // Finalize the array
  tiledb_array_finalize(tiledb_array);

  /* Finalize context. */
  tiledb_ctx_finalize(tiledb_ctx);

  return 0;
}
