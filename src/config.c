#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <libconfig.h>

#include "types.h"
#include "consts.h"
#include "log.h"
#include "globals.h"

// logs an error message, closes the config file and exits the program. internal use only
void throw_config_error(config_t cfg, char *err_msg);

// initializes variables of type `tcdh_config_t`. internal use only, i think. might be useful in main scope.
void tcdh_config_init(tcdh_config_t *config);

// reads all possible filetypes for a category. internal use only.
void tcdh_config_read_category(config_t cfg, char *type, char *output[MAX_FILETYPES_CAT]); 

// prints a string array. used in tcdh_config_print_debug.
void tcdh_print_str_arr_debug(char *array[MAX_FILETYPES_CAT]);

tcdh_config_t tcdh_read_config(char *path) {
    tcdh_config_t output;
    config_t cfg;

    const char *cfg_str_buf;
    config_setting_t *cfg_setting_buf;
    log_write_line("Reading config...\n");

    // intialize conf structs
    tcdh_config_init(&output);
    config_init(&cfg);

    // err check for whole file.
    if (!config_read_file(&cfg, path)) {
        printf("%s, %s:%d - %s\n", "Failed to read config file:", config_error_file(&cfg),config_error_line(&cfg), config_error_text(&cfg));
        throw_config_error(cfg, "Failed to read config file.");
    }

    // read poll_interval
    config_lookup_int(&cfg, "poll_interval_sec", &output.poll_interval);

    // err check for poll_interval
    if (output.poll_interval == -1) {
        throw_config_error(cfg, "Could not find value: poll_interval_sec\n");
    }

    // read dir_to_watch
    config_lookup_string(&cfg, "dir_to_watch", &cfg_str_buf);

    // copy cfg_str_buf to output.watch_dir_path
    output.watch_dir_path = malloc(strlen(cfg_str_buf)+1);
    if (output.watch_dir_path) {
        strcpy(output.watch_dir_path, cfg_str_buf);
    } else {
        throw_config_error(cfg, "Out of memory.");
    }

    if (strcmp(output.watch_dir_path, "") == 0) {// err check for dir_to_watch
        throw_config_error(cfg, "Could not find value: dir_to_watch\n");
    }

    // read types_to_watch
    cfg_setting_buf = config_lookup(&cfg, "types_to_watch"); // get the setting object for "types_to_watch"
    if (cfg_setting_buf != NULL) { // check if setting object was found (config_lookup returns null on bad condition)
        int cfg_array_len_buf = config_setting_length(cfg_setting_buf);
        if (cfg_array_len_buf > MAX_CATEGORY_COUNT) { // overflow check
            throw_config_error(cfg, "Too many category types to watch. (MAX: 4)"); // TODO: make line dynamic
        } else if (cfg_array_len_buf < 1) { // underflow check
            throw_config_error(cfg, "No category to watch specified.");
        }

        for (int i = 0; i < cfg_array_len_buf; i++) { // for every item in array,
            cfg_str_buf = config_setting_get_string_elem(cfg_setting_buf, i); // get const char*
            // convert to char*
            output.types_to_watch[i] = malloc(strlen(cfg_str_buf)+1);
            strcpy(output.types_to_watch[i], cfg_str_buf);
        }
    } else {
        throw_config_error(cfg, "Could not find value: types_to_watch");
    }

    // dependant on MAX_CATEGORY_COUNT, will need to de-hardcode later.
    tcdh_config_read_category(cfg, DOCUMENT_ID, output.document_types);
    tcdh_config_read_category(cfg, AUDIO_ID, output.audio_types);
    tcdh_config_read_category(cfg, VIDEO_ID, output.video_types);
    tcdh_config_read_category(cfg, PHOTO_ID, output.photo_types);
   
    log_write_line("Config read successful.\n");
    config_destroy(&cfg);
    return output;
    
}

void throw_config_error(config_t cfg, char *err_msg) {
    printf("Fatal error: %s", err_msg);
    log_write_error(err_msg);
    config_destroy(&cfg);
    close_logger();
    exit(EXIT_FAILURE); // should probably exit more gracefully
} 

void tcdh_config_read_category(config_t cfg, char *type, char *output[MAX_FILETYPES_CAT]) {
    char path[100] = ""; // no point in making this dynamic yet, though this does need a hard limit specified in consts.h
    char err_msg[500] = ""; // too much work to make dynamic, increase size if error messages begin cutting out.

    snprintf(path, sizeof(path), "%s_types", type);
    //log_write_debug(path);

    const char *cfg_str_buf;
    config_setting_t *cfg_setting_buf;

    cfg_setting_buf = config_lookup(&cfg, path);
    if (cfg_setting_buf != NULL) {
        int cfg_array_len_buf = config_setting_length(cfg_setting_buf);
        if (cfg_array_len_buf > MAX_FILETYPES_CAT) { // overflow check
            snprintf(err_msg, sizeof(err_msg), "Too many values in %s, (MAX: %d)\n", path, MAX_FILETYPES_CAT);
            throw_config_error(cfg, err_msg);
        } if (cfg_array_len_buf < 1) {
            snprintf(err_msg, sizeof(err_msg), "Found 0 values in: %s\n", path);
            log_write_line(err_msg);
        }

        for (int i = 0; i < cfg_array_len_buf; i++) { // for every item in array,
            cfg_str_buf = config_setting_get_string_elem(cfg_setting_buf, i); // get const char*
            output[i] = malloc(strlen(cfg_str_buf)+1); // convert to char*
            strcpy(output[i], cfg_str_buf); // strcpy to output.
        }
    } else {
        snprintf(err_msg, sizeof(err_msg), "Could not find value: %s\n", path);
        log_write_line(err_msg);
    }    
}

void tcdh_config_init(tcdh_config_t *config) { 
    config->poll_interval = -1;
    config->watch_dir_path = "";

    for (int i = 0; i < MAX_FILETYPES_CAT; i++){
        config->audio_types[i] = "";
        config->video_types[i] = "";
        config->photo_types[i] = "";
        config->document_types[i] = "";
    }
}

void tcdh_print_config_debug(tcdh_config_t config) {
    printf("----------------------------\n");
    printf("config debug:\n");
    printf("watch dir: %s\n", config.watch_dir_path);
    printf("polling interval: %d\n", config.poll_interval);
    printf("types_to_watch: \n");
    for (int i = 0; i < MAX_CATEGORY_COUNT; i++) { 
        printf("%s\n", config.types_to_watch[i]);
        // TODO: verify that check below won't break with 3 categories
        if (config.types_to_watch[i+1] == NULL || strcmp(config.types_to_watch[i+1], "") == 0) { 
            break;
        }
    }

    printf("%s:\n", AUDIO_ID);
    tcdh_print_str_arr_debug(config.audio_types);
    printf("%s:\n", VIDEO_ID);
    tcdh_print_str_arr_debug(config.video_types);
    printf("%s:\n", PHOTO_ID);
    tcdh_print_str_arr_debug(config.photo_types);
    printf("%s:\n", DOCUMENT_ID);
    tcdh_print_str_arr_debug(config.document_types);       
    printf("----------------------------\n");
}

void tcdh_print_str_arr_debug(char *array[MAX_FILETYPES_CAT]) {
    for (int i = 0; i < MAX_FILETYPES_CAT; i++) {
        printf("%s\n", array[i]);
        // TODO: verify that check below won't break with MAX_FILETYPES_CAT-1 items
        if (array[i+1] == NULL || strcmp(array[i+1], "") == 0) {
            break;
        }
    }
}