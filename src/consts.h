#ifndef CONSTS_H
#define CONSTS_H

// Static paths
#define LOG_FILE_PATH "/var/log/tcdh.log"
#define CONF_FILE_PATH "/etc/tcdh.conf"
#define LOCK_FILE_PATH "/var/lock/tcdh.lock" //TODO: impl lock mechanism

// File category related
#define MAX_FILETYPES_CAT 25 // hard limit for number of files per category
#define MAX_CATEGORY_COUNT 4 // soft limit for number of categories to sort

// File category folders
#define AUDIO_FOLDER "./audio";
#define VIDEO_FOLDER "./video";
#define DOCUMENT_FOLDER "./documents";
#define PHOTO_FOLDER "./photos";

// Used when reading/writing config, and in
// file handling logic.
#define AUDIO_ID "audio";
#define VIDEO_ID "video";
#define DOCUMENT_ID "document";
#define PHOTO_ID "photo";

#endif /*CONSTS_H*/