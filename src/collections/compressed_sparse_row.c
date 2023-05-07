#include "compressed_sparse_row.h"

// Begin Create and Delete Functions

/**
 * @brief Create a new CompressedSparseRow object with the given
 * number of rows, columns, and non-zero elements.
 *
 * The function asserts that the number of rows, columns, and
 * non-zero elements are all non-negative. The function then
 * allocates memory for the CompressedSparseRow object and
 * its member variables. The function then returns the
 * CompressedSparseRow object.
 *
 * @attention This implementation uses only the lower triangle
 * of the adjacency matrix. In other words, for edges (u,v), the
 * expected edge list consists of edges sorted by u, then v such that
 * u <= v.
 *
 * @param num_rows The number of rows in the matrix.
 * @param num_cols The number of columns in the matrix.
 * @param num_nnzs The number of non-zero elements in the matrix.
 * @return CompressedSparseRow*
 */
CompressedSparseRow* csr_new(int num_rows, int num_cols, int num_nnzs) {
    assert(num_rows >= 0);
    assert(num_cols >= 0);
    assert(num_nnzs >= 0);

    CompressedSparseRow* csr = malloc(sizeof(CompressedSparseRow));

    csr->num_rows = num_rows;
    csr->num_cols = num_cols;
    csr->num_nnzs = num_nnzs;

    csr->num_ptr_rows = num_rows + 1;
    csr->ptr_rows = calloc(csr->num_ptr_rows, sizeof(int));
    csr->idx_cols = calloc(csr->num_nnzs, sizeof(int));
    csr->edge_weights = calloc(csr->num_nnzs, sizeof(int));

    csr->is_set = false;

    return csr;
}

/**
 * @brief Create a new CompressedSparseRow object from the given
 * file.
 *
 * The expected format of the file has the directedness of the graph
 * and the number of vertices and edges on the first two lines. This
 * function assumes the position of the file pointer is at the start
 * of the third line where the first edge is listed.
 *
 * The function uses a nested for loop to parse each line of the file
 * and add the edge to the CompressedSparseRow object. While reading
 * edges (u,v) from the file, if the current u is greater than the
 * previous u (call this u'), then the function sets all row pointers
 * from u' to u to the current index of the non-zero elements
 * indicating those rows are empty. After the nested loop, the row
 * pointers from the last u to the number of rows are set to the
 * number most recent value for the non-zero elements, also indicating
 * those rows to be empty.
 *
 * @param file The file to read from.
 * @param num_rows The number of rows in the matrix.
 * @param num_cols The number of columns in the matrix.
 * @param num_nnzs The number of non-zero elements in the matrix.
 * @return CompressedSparseRow* The CompressedSparseRow object parsed
 * from the file.
 */
CompressedSparseRow* csr_new_from_file(FILE* file, int num_rows, int num_cols, int num_nnzs) {
    assert(file != NULL);

    CompressedSparseRow* vertices = csr_new(num_rows, num_cols, num_nnzs);
    assert(vertices != NULL);

    const int len_buffer = 50;
    char* buffer = calloc(len_buffer, sizeof(char));

    int idx_prev_row = -1;
    int idx_row, idx_col, val_edge_weight;

    for (int idx_nnz = 0; idx_nnz <= vertices->num_nnzs; idx_nnz++) {
        if (fgets(buffer, len_buffer, file) == NULL) {
            break;
        }

        if (sscanf(buffer, "%d %d %d\n", &idx_row, &idx_col, &val_edge_weight) != 3) {
            break;
        }

        assert(idx_row >= 0);
        assert(idx_col >= 0);
        assert(idx_row < vertices->num_rows);
        assert(idx_col < vertices->num_cols);
        assert(val_edge_weight >= 0);

        // prevent vertices with self-loops
        assert(idx_row != idx_col);

        // the edge list should be ordered by u then v for an edge u,v
        // assert(idx_prev_row <= idx_row);

        if (idx_prev_row < idx_row) {
            // Set the row pointer from u' to u to the current index
            // of the non-zero elements indicating those rows are
            // empty.
            for (int i = idx_prev_row + 1; i < idx_row; i++) {
                vertices->ptr_rows[i] = idx_nnz;
            }

            vertices->ptr_rows[idx_row] = idx_nnz;
            idx_prev_row = idx_row;
        }

        vertices->idx_cols[idx_nnz] = idx_col;
        vertices->edge_weights[idx_nnz] = val_edge_weight;
    }

    for (int i = idx_prev_row + 1; i <= vertices->num_rows; i++) {
        vertices->ptr_rows[i] = vertices->num_nnzs;
    }

    free(buffer);

    vertices->is_set = true;
    return vertices;
}

/**
 * @brief Creates a deep copy of the given CompressedSparseRow object.
 *
 * @param csr The CompressedSparseRow object to deep copy.
 * @return CompressedSparseRow* The deep copy of the given
 * CompressedSparseRow object.
 */
CompressedSparseRow* csr_copy(CompressedSparseRow* csr) {
    CompressedSparseRow* copy = csr_new(csr->num_rows, csr->num_cols, csr->num_nnzs);
    memcpy(copy->ptr_rows, csr->ptr_rows, csr->num_ptr_rows * sizeof(int));
    memcpy(copy->idx_cols, csr->idx_cols, csr->num_nnzs * sizeof(int));
    memcpy(copy->edge_weights, csr->edge_weights, csr->num_nnzs * sizeof(int));
    copy->is_set = csr->is_set;
    return copy;
}

/**
 * @brief Deletes the given CompressedSparseRow object.
 *
 * All associated memory is freed and the pointer
 * CompressedSparseRow** is set to NULL.
 *
 * @param csr
 */
void csr_delete(CompressedSparseRow** csr) {
    assert(csr != NULL && *csr != NULL);

    free((*csr)->ptr_rows);
    free((*csr)->idx_cols);
    free((*csr)->edge_weights);
    free(*csr);
    *csr = NULL;
}

// End Create and Delete Functions
// Begin Utility Functions

/**
 * @brief Transforms a compressed sparse row row-pointers into an
 * coordinate row list.
 *
 * @param csr
 * @return int*
 */
int* csr_decompress_row_ptrs(CompressedSparseRow* csr) {
    int* idx_decompressed_rows = calloc(csr->num_nnzs, sizeof(int));
    int* ptr_rows = csr->ptr_rows;

    for (int idx_row = 0; idx_row < csr->num_rows; idx_row++) {
        int idx_row_begin_read = ptr_rows[idx_row];
        int idx_row_end_read = ptr_rows[idx_row + 1];

        for (int i = idx_row_begin_read; i < idx_row_end_read; i++) {
            idx_decompressed_rows[i] = idx_row;
        }
    }

    return idx_decompressed_rows;
}

/**
 * @brief Transforms a coordinate row list into a compressed sparse
 * row row-pointers list.
 *
 * @param csr
 * @param idx_decompressed_row_ptrs
 */
void csr_compress_row_ptrs(CompressedSparseRow* csr, int* idx_decompressed_row_ptrs) {
    int idx_prev_row = -1;
    int idx_curr_row = -1;

    int* ptr_rows = csr->ptr_rows;

    for (int idx_nnz = 0; idx_nnz < csr->num_nnzs; idx_nnz++) {
        idx_curr_row = idx_decompressed_row_ptrs[idx_nnz];
        if (idx_prev_row == idx_curr_row) {
            continue;
        }

        for (int i = idx_prev_row + 1; i < idx_curr_row; i++) {
            ptr_rows[i] = idx_nnz;
        }

        ptr_rows[idx_curr_row] = idx_nnz;
        idx_prev_row = idx_curr_row;
    }

    for (int i = idx_curr_row + 1; i <= csr->num_cols; i++) {
        ptr_rows[i] = csr->num_nnzs;
    }
}

CompressedSparseRow* csr_reduce(CompressedSparseRow* csr, int num_rows, int num_cols, int num_nnz, bool* removed_vertices) {
    int* ptr_ref_rows = csr->ptr_rows;
    int* idx_ref_cols = csr->idx_cols;
    int* idx_ref_weights = csr->edge_weights;

    CompressedSparseRow* reduced_csr = csr_new(num_rows, num_cols, num_nnz);
    int* ptr_rows = reduced_csr->ptr_rows;
    int* idx_cols = reduced_csr->idx_cols;
    int* idx_weights = reduced_csr->edge_weights;

    int idx_prev_row = -1;
    int idx_nnz = 0;

    for (int idx_row = 0; idx_row < csr->num_rows; idx_row++) {
        int idx_begin_read = ptr_ref_rows[idx_row];
        int idx_end_read = ptr_ref_rows[idx_row + 1];

        if (removed_vertices[idx_row]) {
            continue;
        }

        for (int idx_ref_nnz = idx_begin_read; idx_ref_nnz < idx_end_read; idx_ref_nnz++) {
            int idx_col = idx_ref_cols[idx_ref_nnz];

            if (removed_vertices[idx_col]) {
                continue;
            }

            if (idx_prev_row < idx_row) {
                for (int i = idx_prev_row + 1; i < idx_row; i++) {
                    ptr_rows[i] = idx_nnz;
                }

                ptr_rows[idx_row] = idx_nnz;
                idx_prev_row = idx_row;
            }

            idx_cols[idx_nnz] = idx_col;
            idx_weights[idx_nnz] = idx_ref_weights[idx_nnz];

            idx_nnz++;
        }
    }

    for (int i = idx_prev_row + 1; i <= num_rows; i++) {
        ptr_rows[i] = num_nnz;
    }

    assert(idx_nnz == num_nnz);

    return reduced_csr;
}

/**
 * @brief Compares the two given CompressedSparseRow objects for
 * equality.
 *
 * If both csr are NULL, then the function returns true. If one of
 * the csr is NULL and the other is not, then the function returns
 * false. If both csr are not NULL, then the function compares the
 * two csr for equality. The two csr objects are equal if they have
 * the same number of rows, columns, and non-zero elements and if the
 * row pointers, column indices, and edge weights are equal.
 *
 * @param first The first CompressedSparseRow object to compare.
 * @param second The second CompressedSparseRow object to compare.
 * @return bool True if the two csr are equal, false otherwise.
 */
bool csr_is_equal(CompressedSparseRow* first, CompressedSparseRow* second) {
    if (first == NULL && second == NULL) {
        return true;
    }

    if (first == NULL || second == NULL) {
        return false;
    }

    if (array_is_equal(first->ptr_rows, second->ptr_rows, first->num_ptr_rows, second->num_ptr_rows) == false) {
        return false;
    }

    if (array_is_equal(first->idx_cols, second->idx_cols, first->num_nnzs, second->num_nnzs) == false) {
        return false;
    }

    if (array_is_equal(first->edge_weights, second->edge_weights, first->num_nnzs, second->num_nnzs) == false) {
        return false;
    }

    return true;
}

/**
 * @brief Prints the CSR to stdout.
 *
 * @param csr The CSR to print.
 * @param should_print_newline True if a newline should be printed
 * at the end of all print statements, false otherwise.
 */
void csr_print(CompressedSparseRow* csr, bool should_print_newline) {
    assert(csr != NULL);
    printf("Adjacency Matrix: { Row Pointers: %d, Non-Zeros: %d }", csr->num_ptr_rows, csr->num_nnzs);

    if (should_print_newline) {
        printf("\n");
    }
}

// End Utility Functions