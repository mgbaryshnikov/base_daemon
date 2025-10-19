This is a simple Unix daemon that implements the basic features of a daemon process. 
Functionality - displaying information about CPU load
Daemon is separated from the parent process and the console, running independently.
For debugging, the daemon's PID is written to the pidprocess.log file.
The daemon cannot be started multiple times at the same time, since the log file is locked while at least one fork is running.
Daemon messages are logged in cpu_monitor.log
Possible errors will be displayed in the daemon_errors.log file.
