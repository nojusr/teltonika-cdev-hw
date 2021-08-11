#ifndef FILE_UTILS_H
#define FILE_UTILS_H
#include "types.h"

// gets filename extension when given full filename (test.txt -> txt)
const char *get_filename_ext(const char *filename);

// gets one of the categories as defined in consts.h from a file extension
const char *get_file_category_by_ext(tcdh_config_t config, const char *extension);

// moves files. uses standard means.
void tcdh_move_file(tcdh_config_t config, char *filename, char *folder);

#endif /* FILE_UTILS_H */