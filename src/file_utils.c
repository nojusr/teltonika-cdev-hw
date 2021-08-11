#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "consts.h"
#include "types.h"

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

bool check_if_file_is_in_category(const char *extension, char *filetypes[MAX_FILETYPES_CAT]) {
    if (filetypes[0] == NULL || strcmp(filetypes[0], "") == 0) {
        return false;
    }

    for (int i = 0; i < MAX_CATEGORY_COUNT; i++) { 
        if (strcmp(extension, filetypes[i]) == 0) {
            return true;
        }
        if (filetypes[i+1] == NULL || strcmp(filetypes[i+1], "") == 0) { 
            break;
        }
    }
    return false;
}

const char *get_file_category_by_ext(tcdh_config_t config, const char *extension) {

    if (check_if_file_is_in_category(extension, config.photo_types)) {
        return PHOTO_ID;
    } else if (check_if_file_is_in_category(extension, config.video_types)) {
        return VIDEO_ID;
    } else if (check_if_file_is_in_category(extension, config.audio_types)) {
        return AUDIO_ID;
    } else if (check_if_file_is_in_category(extension, config.document_types)) {
        return DOCUMENT_ID;
    }
    return NULL_ID;
}

