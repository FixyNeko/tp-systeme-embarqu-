#define _POSIX_C_SOURCE 201112L

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "tcp.h"

#define ERROR_IF(F) if(F) { perror(#F); exit(errno); }

int tcp_connect(char *addr, char *port) 
{
	struct addrinfo hints, *res;
	int fd;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	ERROR_IF(getaddrinfo(addr, port, &hints, &res) != 0);
	ERROR_IF((fd = socket(res->ai_family, res->ai_socktype, 0)) == -1);
	ERROR_IF(connect(fd, res->ai_addr, res->ai_addrlen) == -1);

	return fd;
}

char* separe_nom(char* commande)
{
    int index0 = strchr(commande, ' ') - commande; 
    int index1 = strchr(&(commande[5]), ' ') - commande;

    int taille_nom = index1 - index0;

    char* nom = (char*)malloc(sizeof(char) * taille_nom);

    int i = 0;

    for(i = 0; i < taille_nom - 1; i++)
    {
        nom[i] = commande[i + index0 + 1];
    }

    nom[i] = '\0';

    return nom;
}

char* separe_message(char* commande)
{
    int index1 = strchr(&(commande[5]), ' ') - commande;
    int index2 = strchr(&(commande[5]), '\n') - commande;

    int taille_message = index2 - index1;

    char* message = (char*)malloc(sizeof(char) * taille_message);

    int i = 0;

    for(i = 0; i < taille_message - 1; i++)
    {
        message[i] = commande[i + index1 + 1];
    }

    message[i] = '\0';

    return message;
}

void date_toString(char date[TAILLE_DATE])
{
    time_t actTime;
    struct tm *timeComp;
 
    time(&actTime);
    timeComp = localtime(&actTime);

    sprintf(date, "%d-%02d-%02d %02d:%02d:%02d", timeComp->tm_year + 1900, timeComp->tm_mon + 1, timeComp->tm_mday, timeComp->tm_hour, timeComp->tm_min, timeComp->tm_sec);
}


int ajouter_dans_fichier(const char* nomFichier, char* commande)
{
	FILE* fichier = fopen(nomFichier, "a");

	int a_ecrit = FALSE;
	
	if(fichier)
	{
		a_ecrit = TRUE;

		char* nom = separe_nom(commande);
        	char* message = separe_message(commande);
        	char date[TAILLE_DATE];

        	date_toString(date);

		fprintf(fichier ,"[%s] %s : %s\n", date, nom, message);

		free(nom);
		free(message);

		fclose(fichier);
	}	

	return a_ecrit;
}

int lecture_dans_fichier(const char* nomFichier)
{
	FILE* fichier = fopen(nomFichier, "r");

	int a_lu = FALSE;

	if(fichier)
	{
		a_lu = TRUE;

		while(!feof(fichier))
		{
			char result[TAILLE_COMMANDE];
		
			if(fgets(result, TAILLE_COMMANDE , fichier))
			{
				fprintf(stdout, "%s", result);
			}
		}

		fclose(fichier);
	}	

	return a_lu;
}

void affiche_menu()
{
	printf("------------------------------MENU--------------------------------\n\nTapez une des commandes ci-dessous :\n1-S'enregister avec un nom (nick nom).\n2-Lister les clients connectes (list).\n3-Envoyer un message a un destinataire (send destinataire message).\n4-Afficher les messages envoyes.\n5-Afficher les messages recu non lus.\n6-Afficher les messages recu deja lus.\n0-Quitter (exit).\n------------------------------------------------------------------\n\n");

}
