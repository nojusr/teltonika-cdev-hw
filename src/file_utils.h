#ifndef FILE_UTILS_H
#define FILE_UTILS_H
#include "types.h"

// returns a char** of newly added files
void poll_for_new_files(tcdh_config_t config, char **old_filenames, char **new_filenames);

// gets the string category of file from filename ext (or libmagic, if i have time to impl that)
void get_file_category(char *filename, char *output);

// gets the folder, into which the file should be moved, according to it's category str
void get_path_by_category_str(char *category);

// moves a file. Uses standard means. creates folders if missing.
void move_file(char *from_path, char *to_path);

#endif /*FILE_UTILS_H*/