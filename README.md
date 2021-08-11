# Teltonika C development homework

## about
This is a linux-only daemon that would scan and sort files by category in a specified folder.
In order to keep things reasonable in terms of file paths and naming conventions,
the internal name of the program, and therefore config and log filenames will be shortened to `tcdh`.

## dependencies
* `libconfig`

## installation/compiling
1. `git clone` this repository
2. `cd` into the folder of this repository
3. `$make`
4. `$make install`

## troubleshooting
If the program doesn't work, the reason why can usually be found in the logfile.

## config
The main config file of this program is to be located in `/etc/tcdh.conf`
An example config file can be found here, as `example.conf`

## logs
The main log file of this program can be found in `/var/log/tcdh.log`

## lockfile
The program uses a lockfile system in order to ensure that only one instance is running at a time.
Normally, the program would delete the lock file upon termination, however, this is *my* software, so if that doesn't happen, you can find and delete the lockfile in `/var/lock/tcdh.lock`
