#ifndef TYPES_H
#define TYPES_H
#include "consts.h"

typedef struct tcdh_config_t {
    int poll_interval;
    char *watch_dir_path;
    char *types_to_watch[MAX_CATEGORY_COUNT];
    char *document_types[MAX_FILETYPES_CAT];
    char *audio_types[MAX_FILETYPES_CAT];
    char *video_types[MAX_FILETYPES_CAT];
    char *photo_types[MAX_FILETYPES_CAT];
} tcdh_config_t;

#endif /*TYPES_H*/