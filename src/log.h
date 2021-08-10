#ifndef LOG_H
#define LOG_H

void init_logger(char* path);

void close_logger();

void log_write_line(char* line);

void log_write_error(char* line);

#endif /*LOG_H*/