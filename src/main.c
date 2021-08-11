#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "consts.h"
#include "log.h"
#include "types.h"
#include "config.h"
#include "globals.h"

FILE *log_fp; // refer to globals.h and https://stackoverflow.com/questions/8108634/global-variables-in-header-file
// if you see this, contact me for a better solution

// main loop, where file handling logic is stored. Runs in daemon enviroment.
// unsure if main.c is the right place to put this.
void main_daemon_loop(char **old_filenames, char **new_filenames);

int main(void)
{
	pid_t process_id = 0;
	pid_t sid = 0;

	char **old_filenames;
	char **new_filenames;

	// Create child process
	process_id = fork();
	// Indication of fork() failure
	if (process_id < 0) {
		printf("fork failed!\n");
		exit(1);
	}
	// PARENT PROCESS. Need to kill it.
	if (process_id > 0) {
		printf("process_id of child process %d \n", process_id);
		exit(0);
	}
	//unmask the file mode
	umask(0);
	//set new session
	sid = setsid();
	if(sid < 0) {
		exit(1);
	}
	// Change the current working directory to root.
	chdir("/");

    init_logger(LOG_FILE_PATH);
    log_write_line("Init complete.\n");
    tcdh_config_t config = tcdh_read_config(CONF_FILE_PATH);
	tcdh_print_config_debug(config);
	while (1) {
		sleep(config.poll_interval);
        main_daemon_loop(old_filenames, new_filenames);
	}
    close_logger();
	return (0);
}

void main_daemon_loop(char **old_filenames, char **new_filenames) {
}