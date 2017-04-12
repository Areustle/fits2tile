/*
 * ============================================================================
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
 * ============================================================================
 */
#include <limits>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include "fitsio.h"
#include "tiledb.h"
#include "fits2tile.h"

int main() {

  TileDB_CTX* tiledb_ctx;
  tiledb_ctx_init(&tiledb_ctx, NULL);

  // Initialize array
  TileDB_Array* tiledb_array;
  tiledb_array_init(
      tiledb_ctx,                                // Context
      &tiledb_array,                             // Array object
      "my_workspace/sparse_arrays/my_array_B",   // Array name
      /* TILEDB_ARRAY_WRITE,                        // Mode */
      TILEDB_ARRAY_WRITE_UNSORTED,                        // Mode
      NULL,                                      // Entire domain
      NULL,                                      // All attributes
      0);                                        // Number of attributes

  std::vector<std::string> fnames = {
    "lat_photon_weekly_w455_p302_v001.fits",
    "lat_photon_weekly_w456_p302_v001.fits",
    "lat_photon_weekly_w457_p302_v001.fits",
    "lat_photon_weekly_w458_p302_v001.fits",
    "lat_photon_weekly_w459_p302_v001.fits",
    "lat_photon_weekly_w460_p302_v001.fits",
    "lat_photon_weekly_w461_p302_v001.fits"
  };

  for( std::string fname : fnames ){
    fitsfile *fptr;
    long nelems;
    float *buffer_coords, *decl, *ra;
    char *nonce;
    int status = 0, anynull = 0;
    //OPEN TABLE
    fits_open_table( &fptr, fname.c_str(), READONLY, &status);
    if (status) {
        fits_report_error(stdout, status);
        return 7;
    }

    fits_get_num_rows(fptr, &nelems, &status);
    if(status){
      fits_report_error(stderr, status);
      return 13;
    }
    /* nelems = 8645; */

    nonce= (char *) malloc(nelems * sizeof(char));
    ra   = (float *) malloc(nelems * sizeof(float));
    decl = (float *) malloc(nelems * sizeof(float));
    buffer_coords = (float *) malloc(2 * nelems * sizeof(float));

    //@TODO Read last bufmod elements.
    std::cout << "Write "<< fname << ", rows: " << nelems << std::endl;
    /* std::cout << "RA " << std::endl; */
    fits_read_col_flt(fptr, 2,              //RA
            1, 1, nelems, 7.0,
            ra, &anynull, &status);

    /* std::cout << "DEC " << std::endl; */
    fits_read_col_flt(fptr, 3,              //DEC
            1, 1, nelems, 7.0,
            decl, &anynull, &status);

    /* std::cout << "Buffer Merge" << std::endl; */
    for(int j=0; j<nelems; ++j){
        nonce[j] = 'a';
        buffer_coords[(j*2)+0] = ra[j];
        buffer_coords[(j*2)+1] = decl[j];
    }

    /* for(int i=0; i<32; ++i){ */
    /*   /1* std::cout << nonce[i] << " "; *1/ */
    /*   for( int j=0; j<2; ++j){ */
    /*     /1* std::cout << buffer_coords[2*i+j] << " "; *1/ */
    /*   } */
    /*   std::cout << std::endl; */
    /* } */

    const void* buffers[] = { nonce, buffer_coords };
    size_t buffer_sizes[] = { nelems*sizeof(char), 2*nelems*sizeof(float) };
    /* std::cout << "Write Array Buffer " << std::endl; */
    try{
      tiledb_array_write(tiledb_array, buffers, buffer_sizes);
    } catch (std::exception e){
      std::cout << e.what() << std::endl;
      return 1;
    }

    fits_close_file(fptr, &status);
    free(nonce);
    free(ra);
    free(decl);
    free(buffer_coords);

  }
  std::cout << "Finalize Array"<<std::endl;
  tiledb_array_finalize(tiledb_array);
  // Consolidate the sparse array
  std::cout << "Consolidate Fragments"<<std::endl;
  tiledb_array_consolidate(tiledb_ctx, "my_workspace/sparse_arrays/my_array_B");
  tiledb_ctx_finalize(tiledb_ctx);
  return 0;
}
