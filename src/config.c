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

void throw_config_error(config_t cfg, char *err_msg);// logs an error message, closes the config file and exits the program. internal use only
void tcdh_config_init(tcdh_config_t *config);// initializes variables of type `tcdh_config_t`. internal use only, i think. might be useful in main scope.

tcdh_config_t tcdh_read_config(char *path) {
    tcdh_config_t output;
    config_t cfg;

    const char *cfg_str_buf;
    config_setting_t *cfg_setting_buf;
    log_write_line("Reading config....");

    // intialize conf structs
    tcdh_config_init(&output);
    config_init(&cfg);

    if (!config_read_file(&cfg, path)) {
        printf("%s, %s:%d - %s\n", "Failed to read config file:", config_error_file(&cfg),config_error_line(&cfg), config_error_text(&cfg));
        throw_config_error(cfg, "Failed to read config file.");
    }

    // read poll_interval
    config_lookup_int(&cfg, "poll_interval_sec", &output.poll_interval);

    if (output.poll_interval == -1) {// err check for poll_interval
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

    log_write_line("Done.\n");
    return output;

    /* TODO: Verify code below
    cfg_setting_buf = config_lookup(&cfg, "types_to_watch"); // get the setting object for "types_to_watch"
    if (cfg_setting_buf != NULL) { // check if setting object was found (config_lookup returns null on bad condition)
        int cfg_array_len_buf = config_setting_length(cfg_setting_buf);
        if (cfg_array_len_buf > MAX_CATEGORY_COUNT) { // overflow check
            throw_config_error(&cfg, "Too many category types to watch. (MAX: 4)"); // TODO: make line dynamic
        }
        for (int i = 0; i < cfg_array_len_buf; i++) {
            output.types_to_watch[i] = config_setting_get_string_elem(cfg_setting_buf, i);
        }
    } else {
        throw_config_error(cfg, "Could not find value: types_to_watch");
    }

    log_write_line(*log_fp, "Config read successful");
    config_destroy(&cfg);
    return output;
    */
}

void throw_config_error(config_t cfg, char *err_msg) {
    printf("throwing error: %s", err_msg);
    log_write_error(err_msg);
    config_destroy(&cfg);
    close_logger();
    exit(EXIT_FAILURE); // should probably exit more gracefully
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

void tcdh_print_config_debug(tcdh_config_t config) { // segfaults on 3rd line
    printf("config debug:\n");
    printf("watch dir: %s\n", config.watch_dir_path);
    printf("polling interval: %d\n", config.poll_interval);
}