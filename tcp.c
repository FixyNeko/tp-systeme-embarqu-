#define _POSIX_C_SOURCE 201112L

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
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

void separe_commande_send(char commande[TAILLE_COMMANDE], char nom[TAILLE_NOM], char message[TAILLE_COMMANDE])
{
	int i = 5; //Lecture a partir du nom;
	int ind_nom = 0; //Indice du nom
	int ind_message = 0; //Indice du message

	while(commande[i] != ' ')
	{
		nom[ind_nom] = commande[i];
						
		i++;
		ind_nom++; //getline plus propre
	}
			
	nom[ind_nom] = '\0';

	i++;

	while(commande[i] != '\n')
	{
		message[ind_message] = commande[i];
				
		i++;
		ind_message++;
	}

	message[ind_message] = '\0';

	printf("DESTINATAIRE %s\nMESSAGE %s\n", nom, message);
}

int ajouter_dans_fichier(const char* nomFichier, char commande[TAILLE_COMMANDE])
{
	FILE* fichier = fopen(nomFichier, "a");

	int a_ecrit = FALSE;
	
	if(fichier)
	{
		char nom[TAILLE_NOM];
		char message[TAILLE_COMMANDE];

		a_ecrit = TRUE;

		separe_commande_send(commande, nom, message);

		fprintf(fichier ,"%s : %s\n", nom, message);

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

			fgets(result, TAILLE_COMMANDE , fichier);

			fprintf(stdout, "%s", result);
		}

		fclose(fichier);
	}	

	return a_lu;
}
