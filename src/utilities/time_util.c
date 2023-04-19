#include "time_util.h"

char* get_formatted_timestamp() {
    time_t current_time;
    struct tm* time_info;
    char* time_str = malloc(sizeof(char) * 50);

    // Get current time
    time(&current_time);
    time_info = localtime(&current_time);

    // Format time string
    strftime(time_str, sizeof(char) * 50, "%m/%d/%Y-%I:%M:%S%p", time_info);

    return time_str;
}