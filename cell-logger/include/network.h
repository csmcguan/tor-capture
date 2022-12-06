#ifndef NETOWRK_H_
#define NETWORK_H_

#include "clpacket.h"

#define RECV_PORT "7000"

int setup(void);
struct clpacket * get_clpacket(int sockfd);

#endif
