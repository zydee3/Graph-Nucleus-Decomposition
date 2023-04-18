#ifndef PRINT_FORMAT_H_INCLUDED
#define PRINT_FORMAT_H_INCLUDED

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "file_io.h"
#include "time_util.h"

void print_green_text(char* text);
void print_red_text(char* text);
void print_test_result(const char* test_file_path, const char* result, bool is_passing);

#endif