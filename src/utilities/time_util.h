#ifndef TIME_UTIL_H_INCLUDED
#define TIME_UTIL_H_INCLUDED

#define HHMMSS "%I:%M:%S"
#define HHMMSSNS "%I:%M:%S:%p"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char* get_formatted_timestamp();

#endif