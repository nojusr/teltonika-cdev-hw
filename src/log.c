#include <stdio.h>
#include "globals.h"

//TODO: attach timestamps and flags to log_write_line and log_write_error

void init_logger(char* path) { // stub
    printf("Init logger...");
	log_fp = fopen(path, "w+");
    printf("Done.\n");
}

void close_logger() {
    fclose(log_fp);
}

void log_write_line(char *line) { // simple prototype version
    fputs(line, log_fp);
}

void log_write_error(char *line) { // simple prototype version
    fputs(line, log_fp);
}
