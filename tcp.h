#ifndef TCP_H
#define TCP_H

#define IP "192.168.102.249"
#define PORT "1337"
#define PORT_LOG "4242"

#define TAILLE_COMMANDE 4096

#define TRUE 1
#define FALSE 0

/*
 * This function is to help you to connect to the server.
 * Its parameters are an IP address, like "172.16.32.27" and a port, like "1337"
 * It returns a file descriptor that you can use with open(2), write(2), close(2), ... (see `man 2 open`, etc)
 * If it returns -1, that means it has failed
 */

int tcp_connect(char* addr, char* port);
int teste_commande(char commande[TAILLE_COMMANDE], int* continuer);

#endif
