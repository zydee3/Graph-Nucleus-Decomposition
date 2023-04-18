#include "print_format.h"

static inline char* _format_test_file_name(const char* file_name) {
    int idx_dir_delim = 0;

    // -2 to account for the .c extension
    // +1 to account for \0 at the end
    int len_file_name = strlen(file_name) - 1;

    for (int i = 0; i < len_file_name; i++) {
        if (file_name[i] == '/') {
            idx_dir_delim = i;
        }
    }

    int len_formatted_name = len_file_name - idx_dir_delim;
    char* formatted_name = (char*)malloc(len_formatted_name);
    assert(formatted_name != NULL);
    memcpy(formatted_name, &file_name[idx_dir_delim], len_formatted_name);
    formatted_name[len_formatted_name - 1] = '\0';

    return formatted_name;
}

/**
 * @brief Prints the param text in green font.
 *
 * The text color is set to green, the param text is printed, and the
 * text color is reset to the default color.
 *
 * @param text
 */
void print_green_text(char* text) {
    printf("\033[0;32m");
    printf("%s", text);
    printf("\033[0m");
}

/**
 * @brief Prints the param text in red font.
 *
 * The text color is set to red, the param text is printed, and the
 * text color is reset to the default color.
 *
 * @param text
 */
void print_red_text(char* text) {
    printf("\033[0;31m");
    printf("%s", text);
    printf("\033[0m");
}

/**
 * @brief Prints the result of a test to stdout. If param is_passing
 * is true, the result SUCCESS is printed in green font, otherwise,
 * the result FAILED is printed in red font.
 *
 * @param result The name of the test being ran.
 * @param is_passing True if the test passed, false otherwise.
 */
void print_test_result(const char* test_file_path, const char* result, bool is_passing) {
    const char* file_name = file_extract_name_from_path(test_file_path);
    char* formatted_file_name = _format_test_file_name(file_name);
    char* formatted_time = get_formatted_timestamp();

    printf("[ %s ] %s::%s: ", formatted_time, formatted_file_name, result);
    if (is_passing) {
        print_green_text("PASSED");
    } else {
        print_red_text("FAILED");
    }
    printf("\n");

    free(formatted_file_name);
    free(formatted_time);
}
