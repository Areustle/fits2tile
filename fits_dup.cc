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
#include <iostream>
#include <string>
#include <vector>
#include "fitsio.h"
#include <cstring>

int main() {

  typedef std::pair<float,float> coord;
  std::string fname = "L170410143649D9656A7F40_PH00.fits";

  fitsfile *fptr;
  long nelems;
  float *buffer_coords, *decl, *ra;
  int status = 0, anynull = 0;
  //OPEN TABLE
  fits_open_table( &fptr, fname.c_str(), READONLY, &status);
  if (status) { fits_report_error(stdout, status); return 7; }

  fits_get_num_rows(fptr, &nelems, &status);
  if(status){ fits_report_error(stderr, status); return 13; }

  ra   = (float *) malloc(nelems * sizeof(float));
  decl = (float *) malloc(nelems * sizeof(float));

  std::cout << "Write "<< fname << ", rows: " << nelems << std::endl;
  fits_read_col_flt(fptr, 2, 1, 1, nelems, 7.0, ra, &anynull, &status);
  fits_read_col_flt(fptr, 3, 1, 1, nelems, 7.0, decl, &anynull, &status);
  fits_close_file(fptr, &status);

  bool dups_found = false;
  for (int i=0; i<nelems-1; ++i){
    for (int j=i+1; j<nelems; ++j){
      if ((ra[i] == ra[j]) && (decl[i] == decl[j])){
        dups_found = true;
        std::cout << "Check with == found \n"
          << "("<< i     << "," << j       << ") \n"
          << "("<< ra[i] << "," << decl[i] << ") \n"
          << "("<< decl[j] << "," << decl[j] << ") \n"
          << std::endl;
      }
      if ((std::memcmp(&ra[i], &ra[j], sizeof(float)) == 0) &&
          (std::memcmp(&decl[i], &decl[j], sizeof(float)) == 0)){
        dups_found = true;
        std::cout << "Check with memcmp found \n"
          << "("<< i     << "," << j       << ") \n"
          << "("<< ra[i] << "," << decl[i] << ") \n"
          << "("<< decl[j] << "," << decl[j] << ") \n"
          << std::endl;
      }
    }
  }
  free(ra);
  free(decl);
  if (dups_found) {
    std::cout << "Duplicates Found!" << std::endl;
    return 1;
  }
  return 0;
}
