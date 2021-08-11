#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/file.h>
#include <errno.h>

#include "consts.h"
#include "log.h"
#include "types.h"
#include "config.h"
#include "globals.h"
#include "file_utils.h"

FILE *log_fp; // refer to globals.h and https://stackoverflow.com/questions/8108634/global-variables-in-header-file
// if you see this, contact me for a better solution

int main(void) {
	// used for forking
	pid_t process_id = 0;
	pid_t sid = 0;

	// used for inotify
	int length = 0;
	int index = 0;
	int fd;
	int wd;
	char buffer[EVENT_BUF_LEN];

	// bufvars
	const char *filename_buf;
	const char *file_category_buf;

	// used for formatting log_buf.
	char log_buf[250];

	// lock check
	int lock_file = open(LOCK_FILE_PATH, O_CREAT | O_RDWR, 0666);
	int rc = flock(lock_file, LOCK_EX | LOCK_NB);

	if (rc) {
		if(EWOULDBLOCK == errno) {
			printf("Another instance of this program is already running.\n");
			return (1);
		} 
	}

	// Create child process
	process_id = fork();
	// Indication of fork() failure
	if (process_id < 0) {
		printf("Fork failed!\n");
		exit(1);
	}
	// PARENT PROCESS. Need to kill it.
	if (process_id > 0) {
		printf("PID of child process: %d\n", process_id);
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

	// create inotify instance
	fd = inotify_init();

	// check for error
	if (fd < 0) {
		printf("Failed to init inotify.\n");
		return (1);
	}

	// init logging
	init_logger(LOG_FILE_PATH);
    log_write_line("Init complete.\n");

	// load config
	tcdh_config_t config = tcdh_read_config(CONF_FILE_PATH);
	//tcdh_print_config_debug(config);

	// add config dir to watch list
	wd = inotify_add_watch(fd, config.watch_dir_path, IN_CLOSE_WRITE | IN_CREATE);

	if (wd < 0) {
		log_write_error("Unable to add inotify watch. Do you have access to the directory?\n");
		return (1);
	}

	// begin main loop
	while (1) {
		length = read(fd, buffer, EVENT_BUF_LEN);

		if (length < 0) {
			log_write_error("Failed to read.\n");
			return(1);
		}

		//TODO: move block below into separate function

		// Read the change inotify events one-by-one and process them accordingly
		while ( index < length ) { 
			struct inotify_event *event = ( struct inotify_event * ) &buffer[index];
			if ( event->len ) {
				if ( event->mask & IN_CLOSE_WRITE || event->mask & IN_MODIFY ) {
					if (!(event->mask & IN_ISDIR)) {
						filename_buf = get_filename_ext(event->name);
						if (filename_buf && filename_buf[0] != '\0') {
							snprintf(log_buf, sizeof(log_buf), "New file: `%s` Extension: %s\n", event->name, filename_buf);
							log_write_line(log_buf);

							file_category_buf = get_file_category_by_ext(config, filename_buf);

							memset(log_buf, 0, sizeof(log_buf));
							snprintf(log_buf, sizeof(log_buf), "File category: %s \n", file_category_buf);
							log_write_line(log_buf);

							//TODO: de-hardcode
							if (strcmp(file_category_buf, DOCUMENT_ID) == 0) {
								tcdh_move_file(config, event->name, DOCUMENT_FOLDER);
							} else if (strcmp(file_category_buf, PHOTO_ID) == 0) {
								tcdh_move_file(config, event->name, PHOTO_FOLDER);
							} else if (strcmp(file_category_buf, VIDEO_ID) == 0) {
								tcdh_move_file(config, event->name, VIDEO_FOLDER);
							} else if (strcmp(file_category_buf, AUDIO_ID) == 0) {
								tcdh_move_file(config, event->name, AUDIO_FOLDER);
							} 					
						}
					}
				}
			}
			index += EVENT_SIZE + event->len;
		}

		// clear out buffers and indices
		index = 0;
		memset(buffer, 0, sizeof(buffer));  
		memset(log_buf, 0, sizeof(log_buf));
	}

	inotify_rm_watch(fd, wd);
	close(fd);
    close_logger();
	return (0);
}