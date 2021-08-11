#ifndef LOG_H
#define LOG_H

// logfile location definition can be found in consts.h

// opens logging file, initialized log_fp
void init_logger(char* path);

// closes logging file.
void close_logger();

// methods below each write a line with a specific flag
// into logging file.
void log_write_line(char* line);

void log_write_error(char* line);

void log_write_debug(char* line);

#endif /*LOG_H*/