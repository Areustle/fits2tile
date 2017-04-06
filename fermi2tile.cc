/*
 * ==================================================================================
 *
 *       Filename:  fits2tile.cc
 *
 *    Description:  Utility to write fits data into tiledb arrays
 *
 *        Version:  1.0
 *        Created:  03/29/2017 21:18:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alexander Reustle
 *   Organization:  NASA GSFC, University of Maryland College Park
 *
 * ==================================================================================
 */
#include <limits>
#include <stdlib.h>
#include <iostream>
#include "fitsio.h"
#include "tiledb.h"


/*
 * ===  FUNCTION  ======================================================================
 *         Name:  make_array
 *  Description:
 * =====================================================================================
 */
	void
make_array ( string , )
{
	/*-----------------------------------------------------------------------------
	 * Write TileDB Array
	 *
	 * Perform the necessary rituals.
	 *-----------------------------------------------------------------------------*/

	// Initialize context with the default configuration parameters
	TileDB_CTX* tiledb_ctx;
	tiledb_ctx_init(&tiledb_ctx, NULL);

	// Prepare parameters for array schema
	const char* array_name = argv[2];
	const char* attributes[] = { "met" };
	const char* dimensions[] = { "ra", "dec" };
	float domain[] = { 0.0, 360.0, -90.0, 90.0 };
	const int cell_val_num[] = {1};
	float tile_extents[] = { 8.0, 4.0 };
	const int types[] = { TILEDB_FLOAT64, TILEDB_FLOAT32 };

	// Set array schema
	TileDB_ArraySchema array_schema;
	tiledb_array_set_schema(
			&array_schema,                        // Array schema struct
			array_name,                           // Array name
			attributes,                           // Attributes
			1,                                    // Number of attributes
			32,                                   // Capacity
			TILEDB_ROW_MAJOR,                     // Cell order
			cell_val_num,                         // Number of cell values per attribute
			compression,                          // Compression
			0,                                    // Sparse array
			dimensions,                           // Dimensions
			2,                                    // Number of dimensions
			domain,                               // Domain
			4*sizeof(float),                      // Domain length in bytes
			tile_extents,                         // Tile extents
			2*sizeof(float),                      // Tile extents length in bytes
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
			TILEDB_ARRAY_WRITE_UNSORTED,               // Mode
			NULL,                                      // Entire domain
			NULL,                                      // All attributes
			0);                                        // Number of attributes
	return <+return value+>;
}		/* -----  end of function make_array  ----- */

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  Run the fits2tile main program.
 * =====================================================================================
 */
	int
main ( int argc, char *argv[] )
{

	/*-----------------------------------------------------------------------------
	 *
	 *  Parse Command line
	 *
	 *  Should contain argv[1] = fits filename, and argv[2] = tiledb array name
	 *
	 *-----------------------------------------------------------------------------*/

	/* if(argc != 3) */


	/*-----------------------------------------------------------------------------
	 *  === Read Fits Columns ===
	 *-----------------------------------------------------------------------------*/
	fitsfile *fptr;
	int status = 0, anynull = 0;
	long nrows;
	char filename[] = argv[1];
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

	int nelems  = nrows;
	double doublenull;
	float floatnull;

	char*   nonce         = new char[nelems];
	float*  ra            = new float[nelems];
	float*  decl          = new float[nelems];
	double* met           = new double[nelems];
	double* buffer_coords = new double[2*nelems];

	std::cout << "RA " << std::endl;
	fits_read_col(
			fptr,
			TFLOAT,
			2,
			1+(nelems),
			1,
			nelems,
			&doublenull,
			ra,
			&anynull,
			&status);

	std::cout << "DEC " << std::endl;
	fits_read_col(fptr,
			TFLOAT,
			3,
			1+(nelems),
			1,
			nelems,
			&doublenull,
			decl,
			&anynull,
			&status);

	std::cout << "MET " << std::endl;
	fits_read_col(fptr,
			TDOUBLE,
			10,
			1+(nelems),
			1,
			nelems,
			&doublenull,
			met,
			&anynull,
			&status);

	std::cout << "Buffer Merge" << std::endl;
	for(int j=0; j<nelems; ++j){
		nonce[j] = 'a';
		buffer_coords[(j*2)]   = ra[j];
		buffer_coords[(j*2)+1] = decl[j];
		/* buffer_coords[(j*3)+2] = met[j]; */
	}

	const void* buffers[] = { nonce, buffer_coords };
	size_t buffer_sizes[] = { nelems*sizeof(char), 2*nelems*sizeof(double) };
	std::cout << "Write Array Buffer " << std::endl;
	tiledb_array_write(tiledb_array, buffers, buffer_sizes);

	/* } */
	tiledb_array_finalize(tiledb_array);
	free(nonce);
	free(met);
	free(ra);
	free(decl);
	free(buffer_coords);


	/* Finalize context. */
	fits_close_file(fptr, &status);
	tiledb_ctx_finalize(tiledb_ctx);

	return EXIT_SUCCESS;
	}				/* ----------  end of function main  ---------- */

