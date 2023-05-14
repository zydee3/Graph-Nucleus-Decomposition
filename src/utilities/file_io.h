#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#define FILE_READ_EXISTING "r"
#define FILE_READ_WRITE_EXISTING "r+"
#define FILE_WRITE_CREATE_TRUNCATE "w"
#define FILE_READ_WRITE_CREATE_TRUNCATE "w+"
#define FILE_APPEND_CREATE "a"
#define FILE_APPEND_READ_CREATE "a+"

#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// #include "../collections/csr.h"
const char* file_extract_name_from_path(const char* file_path);
FILE* file_open(const char* filename, const char* mode);

// void file_write_cliques(const char* filename, GenericLinkedList* cliques, int k);

#endif