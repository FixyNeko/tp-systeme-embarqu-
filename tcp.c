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

#ifdef RASP
#include "led.h"
#endif

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

void* receive(void* _r)
{
	struct reception* r = (struct reception*)_r;

	while(*r->continuer == TRUE)
	{
		fgets(r->message_retour, TAILLE_MESSAGE, r->fichier);

		if(type_message_retour(r->message_retour) == MESSAGE_SERVEUR)
		{
			write(r->pipe_entree, r->message_retour, strlen(r->message_retour));
		}

		else
		{
			ajouter_dans_fichier("message_recu_non_lu.txt", r->message_retour);
			
			#ifdef RASP
			blinkInfo b = {.led = VERT, .period = 300};
			blink_led( b );
			#endif
		}
	}

	return NULL;
}

int type_message_retour(char message_retour[TAILLE_MESSAGE])
{
	return (strncmp(message_retour, "recv", 4) == 0) ? MESSAGE_UTILISATEUR : MESSAGE_SERVEUR; 
}

void nickError(char code_erreur)
{
	switch(code_erreur)
	{
		case '2':
			fprintf(stdout, "Il manque un argument\n");
			break;
		case '3':
			fprintf(stdout, "Nom deja utilise\n");
			break;
		case '4':	
			fprintf(stdout, "Nom invalide\n");
			break;
	}
}

void sendError(char code_erreur)
{
	switch(code_erreur)
	{
		case '2':
			fprintf(stdout, "Il manque un ou des argument(s)\n");
			break;
		case '5':
			fprintf(stdout, "L'expediteur n'as pas de nom\n");
			break;
		case '6':	
			fprintf(stdout, "Le destinataire est inconnu\n");
			break;
		case '7':	
			fprintf(stdout, "Message invalide\n");
			break;
	}
}
