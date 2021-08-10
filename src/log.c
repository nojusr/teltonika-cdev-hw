#include <stdio.h>
#include "globals.h"

//TODO: attach timestamps and flags to log_write_line and log_write_error

void init_logger(char* path) { // stub
    printf("Initializing logger....");
	log_fp = fopen(path, "w+");
    printf("Done. Logfile is in /var/tcdh.log\n");
}

void close_logger() {
    fclose(log_fp);
}

void log_write_line(char *line) { // simple prototype version
    fprintf(log_fp, line);
    fflush(log_fp);
}

void log_write_error(char *line) { // simple prototype version
    fprintf(log_fp, line);
    fflush(log_fp);
}
