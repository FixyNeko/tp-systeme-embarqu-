#ifndef TCP_H
#define TCP_H

#define IP "192.168.102.249"
#define PORT "1337"
#define PORT_LOG "4242"

#define TAILLE_COMMANDE 4096
#define TAILLE_NOM 25

#define TRUE 1
#define FALSE 0

/*
 * This function is to help you to connect to the server.
 * Its parameters are an IP address, like "172.16.32.27" and a port, like "1337"
 * It returns a file descriptor that you can use with open(2), write(2), close(2), ... (see `man 2 open`, etc)
 * If it returns -1, that means it has failed
 */

int tcp_connect(char* addr, char* port);
void separe_commande_send(char commande[TAILLE_COMMANDE], char nom[TAILLE_NOM], char message[TAILLE_COMMANDE]);
int ajouter_dans_fichier(const char* nomFichier, char commande[TAILLE_COMMANDE]);
int lecture_dans_fichier(const char* nomFichier);

#endif
