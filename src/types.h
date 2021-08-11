#ifndef TYPES_H
#define TYPES_H
#include "consts.h"

// main config struct
typedef struct tcdh_config_t {
    char *watch_dir_path;
    char *document_types[MAX_FILETYPES_CAT];
    char *audio_types[MAX_FILETYPES_CAT];
    char *video_types[MAX_FILETYPES_CAT];
    char *photo_types[MAX_FILETYPES_CAT];
} tcdh_config_t;

#endif /*TYPES_H*/