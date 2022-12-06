#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#include "clpacket.h"
#include "log.h"

/* sec.nsec\tdirection\n0 */
#define MAX_LINE 26
#define MASK S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH

static int fd;

void init_log(char *dir, char *filename)
{
    char logfile[1024], dirdir[1024];
    const char *log = "./log";
    struct stat sb;

    /* set up log directory if needed */
    if (!(stat(log, &sb) == 0 && S_ISDIR(sb.st_mode)))
        mkdir(log, 0755);

    /* set up dir directory if needed */
    sprintf(dirdir, "%s/%s", log, dir);
    if (!(stat(dirdir, &sb) == 0 && S_ISDIR(sb.st_mode)))
        mkdir(dirdir, 0755);

    /* create new log file */
    sprintf(logfile, "%s/%s/%s", log, dir, filename);
    fd = open(logfile, O_RDWR | O_CREAT, MASK);
}

void close_log() { close(fd); }

/* log the relative timestamp and direction */
void log_trace(struct clpacket *clpkt)
{
    char buf[MAX_LINE];

    memset(buf, 0, MAX_LINE);
    switch (clpkt->header) {
        case INCOMING:
            snprintf(buf, MAX_LINE, "%u.%u\t-1\n",
                    (unsigned int) clpkt->tspec.tv_sec,
                    (unsigned int) clpkt->tspec.tv_nsec);
            break;
        case OUTGOING:
            snprintf(buf, MAX_LINE, "%u.%u\t1\n",
                    (unsigned int) clpkt->tspec.tv_sec,
                    (unsigned int) clpkt->tspec.tv_nsec);
            break;
    }
    write(fd, buf, strlen(buf));
}
