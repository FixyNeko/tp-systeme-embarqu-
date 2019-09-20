#ifndef TCP_H
#define TCP_H

/*
 * This function is to help you tu connect to the server.
 * Its parameters are an IP address, like "172.16.32.27" and a port, like "1337"
 * It returns a file descriptor that you can use with open(2), write(2), close(2), ... (see `man 2 open`, etc)
 * If it returns -1, that means it has failed
 */
int tcp_connect(char *addr, char *port);

#endif
