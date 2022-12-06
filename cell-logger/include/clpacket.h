#ifndef CELL_LOGGER_PACKET_H_
#define CELL_LOGGER_PACKET_H_

#include <inttypes.h>
#include <sys/socket.h>
#include <time.h>

#define CLPACKET_SIZE 9

/* headers */
#define INCOMING	0x01    /* incoming tor cell */
#define OUTGOING	0x02    /* outgoing tor cell */
#define SHUTDOWN	0x03    /* shutdown logger */

#define GOOD_PACK 	1
#define BAD_PACK	2
#define CLOSE_CONN	3

struct clpacket {
    unsigned char header;
    struct timespec tspec;
};

struct clpacket * create_clpacket(void);
void destroy_clpacket(struct clpacket *clpkt);
void serialize_clpacket(char *buf, struct clpacket *clpkt);
void unserialize_clpacket(char *buf, struct clpacket *clpkt);

#endif
