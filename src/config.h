#ifndef CONFIG_H
#define CONFIG_H
#include "types.h"

tcdh_config_t tcdh_read_config(char *path);

void tcdh_write_config(tcdh_config_t *config);

void tcdh_print_config_debug(tcdh_config_t config);

#endif /*CONFIG_H*/