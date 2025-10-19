#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <syslog.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/resource.h>


#define PID_FILE "pidprocess.log"
#define INTERVAL 10

void logger (const char *filename) {
    FILE *file = fopen(filename, "a");

    fputs("I'm alive \n", file);
    fflush(file);
    fclose(file);
}

void log_cpu_stats(FILE *log) {
    
    if (!log) return;
    
    time_t now = time(NULL);
    char *time_str = ctime(&now);
    time_str[24] = '\0';
    
    FILE *fp = popen("top -l 1 -n 0 | grep CPU | head -1", "r");
    if (fp) {
        char buffer[256];
        if (fgets(buffer, sizeof(buffer), fp)) {
            fprintf(log, "[%s] %s", time_str, buffer);
        }
        pclose(fp);
    }
    
}

int main () {
    
    pid_t           pid;
    struct sigaction        sa;
    FILE                *file, *file1;
    int                 fd;
    file = fopen("cpu_monitor.log", "a");

    fd = fileno(file);
    if (flock(fd, LOCK_EX | LOCK_NB) == -1) {
        
        fclose(file);
        return 1;
    }
    umask(0);
    pid = fork();
    if (pid > 0) {
        return 0;
    }


    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    freopen("daemon_errors", "w", stderr);
    fflush(stdout);
    setbuf(stderr, NULL);
    setbuf(file, NULL);

    file1 = fopen(PID_FILE, "w");
    fprintf(file1, "%d", getpid());
    fclose(file1);
    setsid();
    setbuf(file, NULL);

    while (1) {
        log_cpu_stats(file);
        sleep(INTERVAL);
    }
    
}