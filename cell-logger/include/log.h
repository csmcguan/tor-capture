#ifndef LOG_H_
#define LOG_H_

void init_log(char *addon, char *filename);
void close_log();
void log_trace(struct clpacket *clpkt);

#endif
