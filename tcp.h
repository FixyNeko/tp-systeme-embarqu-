#ifndef TCP_H
#define TCP_H

#define IP "192.168.102.249"
#define PORT "1337"
#define PORT_LOG "4242"

#define TAILLE_COMMANDE 4096
#define TAILLE_NOM 20
#define TAILLE_MESSAGE 4096
#define TAILLE_DATE 20

#define TRUE 1
#define FALSE 0

#define MESSAGE_SERVEUR 2
#define MESSAGE_UTILISATEUR 3

struct reception
{
	FILE* fichier;
	int* continuer;
	char message_retour[TAILLE_MESSAGE];
};

/*
 * This function is to help you to connect to the server.
 * Its parameters are an IP address, like "172.16.32.27" and a port, like "1337"
 * It returns a file descriptor that you can use with open(2), write(2), close(2), ... (see `man 2 open`, etc)
 * If it returns -1, that means it has failed
 */

int tcp_connect(char* addr, char* port);
char* separe_nom(char commande[TAILLE_COMMANDE]);
char* separe_message(char commande[TAILLE_COMMANDE]);
void date_toString(char date[TAILLE_DATE]);
int ajouter_dans_fichier(const char* nomFichier, char* commande);
int lecture_dans_fichier(const char* nomFichier);
void affiche_menu();
void* receive(void* r);
int type_message_retour(char message_retour[TAILLE_MESSAGE]);

#endif
