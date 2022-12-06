#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "clpacket.h"

/* malloc a packet */
struct clpacket * create_clpacket() {
    struct addr *addr;
    struct clpacket *clpkt;

    clpkt = malloc(sizeof(struct clpacket));

    if (clpkt == NULL) {
        fprintf(stderr, "[!] failed to allocate memory for packet\n");
        return (clpkt);
    }

    return (clpkt);
}

void destroy_clpacket(struct clpacket *clpkt)
{
    free(clpkt);
}

/* pack packet contents into buffer */
void serialize_clpacket(char *buf, struct clpacket *clpkt)
{
    buf[0] = clpkt->header;
    *(uint32_t *)(buf + 1) = htonl(clpkt->tspec.tv_sec);
    *(uint32_t *)(buf + 5) = htonl(clpkt->tspec.tv_nsec);

}

/* unpack buffer into packet */
void unserialize_clpacket(char *buf, struct clpacket *clpkt)
{
    clpkt->header = buf[0];
    clpkt->tspec.tv_sec = ntohl(*(uint32_t *)(buf + 1));
    clpkt->tspec.tv_nsec = ntohl(*(uint32_t *)(buf + 5));
}

