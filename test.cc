/*
 * ==================================================================================
 *
 *       Filename:  test.cc
 *
 *    Description:  testing fitsio and tiledb
 *
 *        Version:  1.0
 *        Created:  03/29/2017 21:18:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * ==================================================================================
 */
#include <limits>
#include <stdlib.h>
#include <iostream>
#include "fitsio.h"
#include "tiledb.h"

int main(){

    // === Write TileDB Array ===
    // Initialize context with the default configuration parameters
    TileDB_CTX* tiledb_ctx;
    tiledb_ctx_init(&tiledb_ctx, NULL);

    // Prepare parameters for array schema
    const char* array_name = "my_workspace/sparse_arrays/my_array_B";
    const char* attributes[] = { "energy" };
    const char* dimensions[] = { "ra", "dec", "met" };
    double domain[] =
    {
        0.0, 360.0,                                 //RA  domain
        -90.0, 90.0,                                //DEC domain
        0.0, std::numeric_limits<double>::max()       //MET domain
    };
    const int cell_val_num[] = {1};
    const int compression[] =
    {
        TILEDB_NO_COMPRESSION,
        TILEDB_NO_COMPRESSION
    };
    double tile_extents[] =
    {
        8.0,    //8 degrees of arc
        4.0,    //8 degrees of arc
        4096.0  //just over 60 minutes
    };
    const int types[] =
    {
        TILEDB_CHAR,
        TILEDB_FLOAT64
    };
  // Set array schema
  TileDB_ArraySchema array_schema;
  tiledb_array_set_schema(
      &array_schema,                        // Array schema struct
      array_name,                           // Array name
      attributes,                           // Attributes
      1,                                    // Number of attributes
      2,                                 // Capacity
      TILEDB_ROW_MAJOR,                     // Cell order
      cell_val_num,                         // Number of cell values per attribute
      compression,                          // Compression
      0,                                    // Sparse array
      dimensions,                           // Dimensions
      3,                                    // Number of dimensions
      domain,                               // Domain
      6*sizeof(double),        // Domain length in bytes
      tile_extents,                         // Tile extents
      3*sizeof(double),              // Tile extents length in bytes
      TILEDB_ROW_MAJOR,                     // Tile order
      types                                 // Types
  );

  // Create array
  tiledb_array_create(tiledb_ctx, &array_schema);

  // Free array schema
  tiledb_array_free_schema(&array_schema);

  TileDB_Array* tiledb_array;
  tiledb_array_init(
      tiledb_ctx,                                // Context
      &tiledb_array,                             // Array object
      "my_workspace/sparse_arrays/my_array_B",   // Array name
      TILEDB_ARRAY_WRITE_UNSORTED,                        // Mode
      NULL,                                      // Entire domain
      NULL,                                      // All attributes
      0);                                        // Number of attributes

    // === Read Fits Columns ===
    fitsfile *fptr;
    int status = 0, anynull = 0;
    long nrows;
    char filename[] = "lat_photon_weekly_w460_p302_v001.fits";
    //OPEN TABLE
    fits_open_table(
            &fptr,
            filename,
            READONLY,
            &status);
    if (status) {
        fits_report_error(stdout, status);
        return 1;
    }

    //GET NUMBER OF ROWS
    fits_get_num_rows(fptr, &nrows, &status);
    if (status) {
        fits_report_error(stdout, status);
        return 2;
    }

    /* int bufruns = nrows*sizeof(float) / 0x40000000; */
    /* int bufmod  = nrows*sizeof(float) % 0x40000000; */
    /* std::cout << nrows << " " << bufruns << std::endl; */
    int nelems  = nrows; // bufruns;
    double doublenull, *met, *buffer_coords;
    float floatnull, *ra, *decl;
    char *nonce;

    nonce= (char *) malloc(nelems * sizeof(char));
    met  = (double *) malloc(nelems * sizeof(double));
    ra   = (float *) malloc(nelems * sizeof(float));
    decl = (float *) malloc(nelems * sizeof(float));
    buffer_coords = (double *) malloc(3 * nelems * sizeof(double));

    //@TODO Read last bufmod elements.
    /* for(int i=0; i<=bufruns; ++i){ */
    int i=0;
    std::cout << i <<" "<< nelems << std::endl;
    std::cout << "RA " << std::endl;
    fits_read_col(fptr, TFLOAT, 2,              //RA
            1+(i*nelems), 1, nelems, &doublenull,
            ra, &anynull, &status);

    std::cout << "DEC " << std::endl;
    fits_read_col(fptr, TFLOAT, 3,              //DEC
            1+(i*nelems), 1, nelems, &doublenull,
            decl, &anynull, &status);

    std::cout << "MET " << std::endl;
    fits_read_col(fptr, TDOUBLE, 10,             //MET
            1+(i*nelems), 1, nelems, &doublenull,
            met, &anynull, &status);

    std::cout << "Buffer Merge" << std::endl;
    for(int j=0; j<nelems; ++j){
        nonce[j] = 'a';
        buffer_coords[(j*3)] = static_cast<double>(ra[j]);
        buffer_coords[(j*3)+1] = static_cast<double>(decl[j]);
        buffer_coords[(j*3)+2] = met[j];
    }

    const void* buffers[] = { nonce, buffer_coords };
    size_t buffer_sizes[] = { nelems*sizeof(char), 3*nelems*sizeof(double) };
    std::cout << "Write Array Buffer " << std::endl;
    tiledb_array_write(tiledb_array, buffers, buffer_sizes);

    /* } */
    tiledb_array_finalize(tiledb_array);
    free(nonce);
    free(met);
    free(ra);
    free(decl);
    free(buffer_coords);


    fits_close_file(fptr, &status);

  /* Finalize context. */
  tiledb_ctx_finalize(tiledb_ctx);

    /* free( ra ); */
    /* free( decl ); */


    return 0;
}
