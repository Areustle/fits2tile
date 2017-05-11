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
    /* "L170410143649D9656A7F40_PH00.fits" */
    "lat_photon_weekly_w448_p302_v001.fits",
    "lat_photon_weekly_w449_p302_v001.fits",
    "lat_photon_weekly_w450_p302_v001.fits",
    "lat_photon_weekly_w451_p302_v001.fits",
    "lat_photon_weekly_w452_p302_v001.fits",
    "lat_photon_weekly_w453_p302_v001.fits",
    "lat_photon_weekly_w454_p302_v001.fits",
    "lat_photon_weekly_w455_p302_v001.fits",
    "lat_photon_weekly_w456_p302_v001.fits",
    "lat_photon_weekly_w457_p302_v001.fits",
    "lat_photon_weekly_w458_p302_v001.fits",
    "lat_photon_weekly_w459_p302_v001.fits",
    "lat_photon_weekly_w460_p302_v001.fits",
    "lat_photon_weekly_w461_p302_v001.fits",
    "lat_photon_weekly_w462_p302_v001.fits",
    "lat_photon_weekly_w463_p302_v001.fits",
    "lat_photon_weekly_w464_p302_v001.fits",
    "lat_photon_weekly_w465_p302_v001.fits",
    "lat_photon_weekly_w466_p302_v001.fits"
  };

  for( std::string fname : fnames ){
    fitsfile *fptr;
    long nelems;
    int status = 0, _nul = 0;

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

	  /* char*   nonce         = new char[nelems]; */

	  float*  energy        = new float[nelems];
	  double*  ra            = new double[nelems];
	  double*  decl          = new double[nelems];
	  /* float*  gallat        = new float[nelems]; */
	  /* float*  gallong       = new float[nelems]; */
	  /* float*  theta         = new float[nelems]; */
	  /* float*  phi           = new float[nelems]; */
	  /* float*  zenith_angle  = new float[nelems]; */
	  /* float*  earth_az_ang  = new float[nelems]; */
    /* double* met           = new double[nelems]; */
    /* int*    event_id      = new int[nelems]; */
    /* int*    run_id        = new int[nelems]; */
    /* /1* short*  recon_v       = new short[nelems]; *1/ */
    /* /1* short*  calib_v       = new short[3*nelems]; *1/ */
    /* int*    event_class   = new int[nelems]; */
    /* int*    event_type    = new int[nelems]; */
    /* /1* short*  conv_type     = new short[nelems]; *1/ */
    /* double* livetime      = new double[nelems]; */
	  double*  buffer_coords = new double[2*nelems];



    //@TODO Read last bufmod elements.
    std::cout << "Write "<< fname << ", rows: " << nelems << std::endl;

    fits_read_col_flt(fptr, 1,              //Energy
            1, 1, nelems, 7.0, energy, &_nul, &status);
    fits_read_col_dbl(fptr, 2,              //RA
            1, 1, nelems, 7.0, ra, &_nul, &status);
    fits_read_col_dbl(fptr, 3,              //DEC
            1, 1, nelems, 7.0, decl, &_nul, &status);
    /* fits_read_col_flt(fptr, 4, */
    /*         1, 1, nelems, 7.0, gallat, &_nul, &status); */
    /* fits_read_col_flt(fptr, 5, */
    /*         1, 1, nelems, 7.0, gallong, &_nul, &status); */
    /* fits_read_col_flt(fptr, 6, */
    /*         1, 1, nelems, 7.0, theta, &_nul, &status); */
    /* fits_read_col_flt(fptr, 7, */
    /*         1, 1, nelems, 7.0, phi, &_nul, &status); */
    /* fits_read_col_flt(fptr, 8, */
    /*         1, 1, nelems, 7.0, zenith_angle, &_nul, &status); */
    /* fits_read_col_flt(fptr, 9, */
    /*         1, 1, nelems, 7.0, earth_az_ang, &_nul, &status); */
    /* fits_read_col_dbl(fptr, 10, */
    /*         1, 1, nelems, 7.0, met, &_nul, &status); */
    /* fits_read_col_int(fptr, 11, */
    /*         1, 1, nelems, 7, event_id, &_nul, &status); */
    /* fits_read_col_int(fptr, 12, */
    /*         1, 1, nelems, 7, run_id, &_nul, &status); */
    /* /1* fits_read_col_sht(fptr, 13, *1/ */
    /* /1*         1, 1, nelems, 7, recon_v, &_nul, &status); *1/ */
    /* fits_read_col_int(fptr, 15, */
    /*         1, 1, nelems, 7, event_class, &_nul, &status); */
    /* fits_read_col_int(fptr, 16, */
    /*         1, 1, nelems, 7, event_type, &_nul, &status); */
    /* /1* fits_read_col_sht(fptr, 17, *1/ */
    /* /1*         1, 1, nelems, 7, conv_type, &_nul, &status); *1/ */
    /* fits_read_col_dbl(fptr, 18, */
    /*         1, 1, nelems, 7, livetime, &_nul, &status); */

    /* std::cout << "Buffer Merge" << std::endl; */
    for(int j=0; j<nelems; ++j){
        buffer_coords[(j*2)+0] = ra[j];
        buffer_coords[(j*2)+1] = decl[j]+90;
    }

    /* for(int i=0; i<32; ++i){ */
    /*   /1* std::cout << nonce[i] << " "; *1/ */
    /*   for( int j=0; j<2; ++j){ */
    /*     /1* std::cout << buffer_coords[2*i+j] << " "; *1/ */
    /*   } */
    /*   std::cout << std::endl; */
    /* } */

    const void* buffers[] = {
      energy,
      /* gallat, */
      /* gallong, */
      /* theta, */
      /* phi, */
      /* zenith_angle, */
      /* earth_az_ang, */
      /* met, */
      /* event_id, */
      /* run_id, */
      /* /1* recon_v, *1/ */
      /* event_class, */
      /* event_type, */
      /* /1* conv_type, *1/ */
      /* livetime, */
      buffer_coords
    };
    size_t buffer_sizes[] = {
      nelems*sizeof(float),
      /* nelems*sizeof(float), */
      /* nelems*sizeof(float), */
      /* nelems*sizeof(float), */
      /* nelems*sizeof(float), */
      /* nelems*sizeof(float), */
      /* nelems*sizeof(float), */
      /* nelems*sizeof(double), */
      /* nelems*sizeof(int), */
      /* nelems*sizeof(int), */
      /* /1* nelems*sizeof(short), *1/ */
      /* nelems*sizeof(int), */
      /* nelems*sizeof(int), */
      /* /1* nelems*sizeof(short), *1/ */
      /* nelems*sizeof(double), */
      2*nelems*sizeof(double)
    };
    /* std::cout << "Write Array Buffer " << std::endl; */
    try{
      tiledb_array_write(tiledb_array, buffers, buffer_sizes);
    } catch (std::exception e){
      std::cout << e.what() << std::endl;
      return 1;
    }

    fits_close_file(fptr, &status);

    delete[] energy;
    delete[] ra;
    delete[] decl;
    /* delete[] gallat; */
    /* delete[] gallong; */
    /* delete[] theta; */
    /* delete[] phi; */
    /* delete[] zenith_angle; */
    /* delete[] earth_az_ang; */
    /* delete[] met; */
    /* delete[] event_id; */
    /* delete[] run_id; */
    /* /1* delete[] recon_v; *1/ */
    /* /1* delete[] calib_v; *1/ */
    /* delete[] event_class; */
    /* delete[] event_type; */
    /* /1* delete[] conv_type; *1/ */
    /* delete[] livetime; */
    delete[](buffer_coords);

  }
  std::cout << "Finalize Array"<<std::endl;
  tiledb_array_finalize(tiledb_array);
  // Consolidate the sparse array
  std::cout << "Consolidate Fragments"<<std::endl;
  tiledb_array_consolidate(tiledb_ctx, "my_workspace/sparse_arrays/my_array_B");
  tiledb_ctx_finalize(tiledb_ctx);
  std::cout << "Finished"<<std::endl;
  return 0;
}
