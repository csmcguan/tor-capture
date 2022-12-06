#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "clpacket.h"
#include "log.h"
#include "network.h"
#include "handle.h"

#define MASK S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH

int main(int argc, char **argv)
{
    int sockfd, ret, ready;
    const char r = '1';
    const char *ready_file = ".ready";

    if (argc != 3) {
        fprintf(stderr, "Usage: cell-logger <dir> <log-file.csv>\n");
        exit(1);
    }

    /* initialize log file */
    init_log(argv[1], argv[2]);

    /* setup socket to receive on */
    sockfd = setup();

    /* couldn't get a socket */
    if (sockfd == -1)
        goto done;

    if ((ready = open(ready_file, O_RDWR | O_CREAT, MASK)) == -1) {
        fprintf(stderr, "cell logger: error creating ready file\n");
        exit(1);
    }
    write(ready, &r, 1);
    close(ready);

    /* loop forever until control says we're done */
    for (;;) {
        ret = get_cell(sockfd);

        switch (ret) {
            case CLOSE_CONN:
                printf("cell logger: closing\n");
                close_log();
                goto done;
        }
    }
done:
    remove(ready_file);
    return (0);
}
