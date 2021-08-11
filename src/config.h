#ifndef CONFIG_H
#define CONFIG_H
#include "types.h"

// reads a config from file into tcdh_config_t
tcdh_config_t tcdh_read_config(char *path);

// writes a config from tcdh_config_t into a file. Currently a stub.
void tcdh_write_config(tcdh_config_t *config, char *path);

// prints tcdh_config_t into console.
void tcdh_print_config_debug(tcdh_config_t config);

#endif /*CONFIG_H*/