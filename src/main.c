#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/inotify.h>


#include "consts.h"
#include "log.h"
#include "types.h"
#include "config.h"
#include "globals.h"

FILE *log_fp; // refer to globals.h and https://stackoverflow.com/questions/8108634/global-variables-in-header-file
// if you see this, contact me for a better solution

// main loop, where file handling logic is stored. Runs in daemon enviroment.
// unsure if main.c is the right place to put this.
//void main_daemon_loop(tcdh_config_t config, char **old_filenames, char **new_filenames);

int main(void)
{
	// used when forking
	pid_t process_id = 0;
	pid_t sid = 0;

	// used for inotify
	int length = 0;
	int index = 0;
	int fd;
	int wd;
	char buffer[EVENT_BUF_LEN];

	// used for formatting debug.
	char debug[250];

	//char **old_filenames = (char**) malloc(1 * sizeof(char*)); 
	//char **new_filenames = (char**) malloc(1 * sizeof(char*));

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

	// create inotify instance
	fd = inotify_init();

	// check for error
	if (fd < 0) {
		log_write_error("Failed to init inotify.\n");
		return (1);
	}

	// add config dir to watch list
	//wd = inotify_add_watch(fd, config.watch_dir_path, IN_CLOSE_WRITE | IN_MODIFY);
	wd = inotify_add_watch(fd, config.watch_dir_path, IN_CLOSE_WRITE | IN_MODIFY);

	if (wd < 0) {
		log_write_error("Unable to add inotify watch. Do you have access to the directory?\n");
		return (1);
	}

	// read

	while (1) {
		
		length = read(fd, buffer, EVENT_BUF_LEN);

		if (length < 0) {
			log_write_error("Failed to read.\n");
			return(1);
		}

		/*actually read return the list of change events happens. Here, read the change event one by one and process it accordingly.*/
		while ( index < length ) {     
			struct inotify_event *event = ( struct inotify_event * ) &buffer[index];
			log_write_line("Sussin.\n");
			if ( event->len ) {
				if ( event->mask & IN_CLOSE_WRITE | event->mask & IN_MODIFY ) {
					if (!(event->mask & IN_ISDIR)) {
						snprintf(debug, sizeof(debug), "New file %s created.\n", event->name );
						log_write_line(debug);
					}
				}
			}
			index += EVENT_SIZE + event->len;
		}
		index = 0;
		memset(buffer, 0, sizeof(buffer));  
		memset(debug, 0, sizeof(debug));
	}


	/*
	while (1) {
		sleep(config.poll_interval);
        main_daemon_loop(config, old_filenames, new_filenames);
	}*/

	//free(old_filenames);
	//free(new_filenames);

	inotify_rm_watch(fd, wd);
	close(fd);
    close_logger();
	return (0);
}

/*
void main_daemon_loop(tcdh_config_t config, char **old_filenames, char **new_filenames) {
	poll_for_new_files(config, old_filenames, new_filenames);
}*/