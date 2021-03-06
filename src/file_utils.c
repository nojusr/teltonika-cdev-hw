#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "consts.h"
#include "types.h"
#include "log.h"

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

bool check_if_file_is_in_category(const char *extension, char *filetypes[MAX_FILETYPES_CAT]) {
    if (filetypes[0] == NULL || strcmp(filetypes[0], "") == 0) {
        return false;
    }

    for (int i = 0; i < MAX_FILETYPES_CAT; i++) { 
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

void tcdh_move_file(tcdh_config_t config, char *filename, char *folder) {
    // these could probably be a lot smaller in practice, but these are also faster, mem-safe and take up 6kb total, 
    // which is marginal in most enviroments. Also, i'm too lazy to do it dynamically.
    char oldpath[512] = {'\0'};
    char newpath[512] = {'\0'};
    char normalized_path[512] = {'\0'};
    char normalized_folder[512] = {'\0'};

    struct stat st = {0}; // used for dircheck

    // add trailing slash to dir_to_watch
    if (config.watch_dir_path[strlen(config.watch_dir_path)-1] == '/') {
        snprintf(normalized_path, sizeof(normalized_path), "%s", config.watch_dir_path);
    } else {
        snprintf(normalized_path, sizeof(normalized_path), "%s/", config.watch_dir_path);
    }

    // add trailing slash to output folder
    if (folder[strlen(folder)-1] == '/') {
        snprintf(normalized_folder, sizeof(normalized_folder), "%s", folder);
    } else {
        snprintf(normalized_folder, sizeof(normalized_folder), "%s/", folder);
    }

    // using oldpath as temporary buffer
    //snprintf(oldpath, sizeof(oldpath), "%s%s", normalized_path, folder);// generate absolute folder path
    if (stat(normalized_folder, &st) == -1) { // if folder doesn't exist, create it.
        mkdir(normalized_folder, 0777); // TODO: figure out how to make this folder not-as-root
    }

    // get oldpath
    snprintf(oldpath, sizeof(oldpath), "%s%s", normalized_path,filename);
    
    // get newpath
    snprintf(newpath, sizeof(newpath), "%s%s", normalized_folder,filename);

    // do the dew
    rename(oldpath, newpath);

    // log changes
    char log_buf[1038] = {'\0'};// size is what cc told me to change it to
    snprintf(log_buf, sizeof(log_buf), "Moved `%s` to `%s`\n", oldpath, newpath); // Can skimp on memory for non-critical application
    log_write_line(log_buf);
}

