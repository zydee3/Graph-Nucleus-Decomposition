#include "file_io.h"

const char* file_extract_name_from_path(const char* file_path) {
    const char* file_name = strrchr(file_path, '/');
    if (!file_name) {
        file_name = strrchr(file_path, '\\');
    }

    if (!file_name) {
        file_name = file_path;  // No directory separator found, use the whole path
    } else {
        file_name++;  // Move past the directory separator
    }

    return file_name;
}

// char* _get_generation_date_time() {
//     time_t now = time(NULL);
//     struct tm* time_info = localtime(&now);
//     char* date_time = malloc(sizeof(char) * 20);
//     strftime(date_time, 20, "%Y-%m-%d %H:%M:%S", time_info);
//     return date_time;
// }

// void directory_clear(const char* directory_name) {
//     DIR* directory = opendir(directory_name);
//     assert(directory != NULL);

//     struct dirent* ent;
//     char* directory_file_path = malloc(sizeof(char) * 100);

//     while ((ent = readdir(directory)) != NULL) {
//         sprintf(directory_file_path, "%s/%s", directory_name, ent->d_name);
//         remove(directory_file_path);
//     }

//     free(directory_file_path);
//     closedir(directory);
// }

/**
 * @brief Open a file with the given filename and mode. If the file
 * cannot be opened, exit the program.
 *
 * @param filename File to open.
 * @param mode Mode to open the file in.
 * @return FILE* File opened with the given filename and mode.
 */
FILE* file_open(const char* filename, const char* mode) {
    FILE* file = fopen(filename, mode);
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    return file;
}

// void file_write_graph(CompressedSparseRow* graph, const char* file_name, char* generation_time) {
//     assert(graph != NULL && file_name != NULL);

//     printf("filename: %s\n", file_name);

//     FILE* file = file_open(file_name, FILE_WRITE_CREATE_TRUNCATE);
//     fprintf(file, "%% Generation Time: %s\n", generation_time);
//     fprintf(file, "%% %d %d %d\n", graph->is_directed ? 1 : 0, graph->num_rows, graph->num_rows);

//     Node* curr_vertex = graph->vertices->head;
//     while (curr_vertex != NULL) {
//         Node* curr_edge = curr_vertex->children->head;
//         while (curr_edge != NULL) {
//             fprintf(file, "%d %d %d\n", curr_vertex->data, curr_edge->data, curr_edge->meta);
//             curr_edge = curr_edge->next;
//         }
//         curr_vertex = curr_vertex->next;
//     }
// }

// void file_write_graph_gen_res(CompressedSparseRow* graph, bool (*graph_apply_fn)(CompressedSparseRow*)) {
//     const char* directory_name = "data/output";
//     directory_clear(directory_name);

//     int iteration = 0;
//     char* file_path = malloc(sizeof(char) * 3 + sizeof(directory_name));

//     do {
//         char* generation_time = _get_generation_date_time();
//         printf("Iteration: %d, Generation Time: %s\n", iteration, generation_time);
//         sprintf(file_path, "%s/%d", directory_name, iteration);
//         file_write_graph(graph, file_path, generation_time);

//         free(generation_time);
//         iteration++;

//     } while (graph_apply_fn(graph));
// }

// CompressedSparseRow* csr_new_from(const char* file_path) {
//     FILE* file = file_open(file_path, FILE_READ_EXISTING);
//     assert(file != NULL);

//     const int buffer_size = 100;
//     char* buffer = calloc(buffer_size, sizeof(char));

//     // parse the first line for graph type (directed or undirected)
//     assert(fgets(buffer, buffer_size, file) != NULL);
//     bool is_directed = strcmp(buffer, "%% directed") == 0;

//     // parse the second line for number of nodes and edges
//     int* num_nodes = malloc(sizeof(int));
//     int* num_edges = malloc(sizeof(int));
//     assert(fgets(buffer, buffer_size, file) != NULL);
//     assert(sscanf(buffer, "%% %d %d", num_nodes, num_edges) == 2);

//     // create csr using the number of nodes and edges parsed above.
//     CompressedSparseRow* csr = crs_new(num_nodes, num_nodes, num_edges);
//     assert(csr != NULL);

//     // parse the edge list

//     while (fgets(buffer, buffer_size, file) != NULL) {
//         assert(buffer[0] != '%');

//         int u, v, weight = 0;

//         assert(sscanf(buffer, "%d %d %d", &u, &v, &weight) != 3);

//         print("%d %d %d\n", u, v, weight);
//     }

//     free(buffer);
//     return csr;
// }