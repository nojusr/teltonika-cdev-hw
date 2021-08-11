#ifndef FILE_UTILS_H
#define FILE_UTILS_H
#include "types.h"

const char *get_filename_ext(const char *filename);

const char *get_file_category_by_ext(tcdh_config_t config, const char *extension);

void tcdh_move_file(tcdh_config_t config, char *filename, char *folder);

#endif /* FILE_UTILS_H */