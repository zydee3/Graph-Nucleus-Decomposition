#ifndef CSR_H_INCLUDED
#define CSR_H_INCLUDED

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utilities/array_util.h"

typedef struct CompressedSparseRow {
    int num_rows;
    int num_cols;
    int num_nnzs;

    int num_ptr_rows;

    int *ptr_rows;
    int *idx_cols;
    int *edge_weights;

    bool is_set;
} CompressedSparseRow;

// Create and Delete Functions
CompressedSparseRow *csr_new(int num_rows, int num_cols, int num_nnzs);
CompressedSparseRow *csr_new_from_file(FILE *file, int num_rows, int num_cols, int num_nnzs, bool is_directed);
CompressedSparseRow *csr_copy(CompressedSparseRow *csr);
void csr_delete(CompressedSparseRow **crs);

// Manipulation Functions
int *csr_get_coord_rows(CompressedSparseRow *csr);
void csr_compress_row_ptrs(CompressedSparseRow *csr, int *idx_decompressed_row_ptrs);
CompressedSparseRow *csr_reduce(CompressedSparseRow *csr, int num_rows, int num_cols, int num_nnz, bool *removed_vertices);

// Utility Functions
bool csr_is_equal(CompressedSparseRow *first, CompressedSparseRow *second);
void csr_print(CompressedSparseRow *csr, bool should_print_newline);

#endif