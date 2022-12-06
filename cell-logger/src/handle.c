#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clpacket.h"
#include "log.h"
#include "network.h"
#include "handle.h"

/* pick apart function code and log accordingly */
int process_clpacket(struct clpacket *clpkt) {

    switch (clpkt->header) {
        case INCOMING:
        case OUTGOING:
            /* log cell */
            log_trace(clpkt);
            break;
        case SHUTDOWN:
            /* shutting down */
            goto shutdown;
        default:
            /* packet malformed */
            goto fail;
    }
    return (GOOD_PACK);

shutdown:
    return (CLOSE_CONN);

    /* some sort of bad packet */
fail:
    fprintf(stderr, "[!] bad packet\n");
    return (BAD_PACK);
}

/* get the next packet and determine what to do */
int get_cell(int sockfd)
{
    int ret;
    struct clpacket *clpkt;

    clpkt = get_clpacket(sockfd);
    if (clpkt != NULL)
        ret = process_clpacket(clpkt);

    switch (ret) {
        case CLOSE_CONN:
            destroy_clpacket(clpkt);
            return (CLOSE_CONN);
        case BAD_PACK:
            destroy_clpacket(clpkt);
            return (BAD_PACK);
    }

    destroy_clpacket(clpkt);
    return (GOOD_PACK);
}

