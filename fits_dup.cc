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

int main() {

  typedef std::pair<float,float> coord;
  std::string fname = "L170410143649D9656A7F40_PH00.fits";
  std::vector<coord> coords;

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

  for (int i=0; i<nelems; ++i)
    coords.push_back(std::make_pair(ra[i],decl[i]));

  free(ra);
  free(decl);

  bool dups_found = false;
  std::sort(coords.begin(), coords.end());
  for (int i=0; i<coords.size() -1 ; ++i){
    if (coords[i].first == coords[i+1].first
        &&
        coords[i].second == coords[i+1].second){
      dups_found = true;
      std::cout << coords[i].first<< ","<< coords[i].second << std::endl;
    }
  }
  if (dups_found) std::cout << "Duplicates Found!" << std::endl;
  return 0;
}
