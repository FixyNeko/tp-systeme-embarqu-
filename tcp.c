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

int teste_commande(char commande[TAILLE_COMMANDE], int* continuer)
{
	char* commande_base = strtok(commande, " ");

	printf("La commande saisie est : %s\n", commande_base);

	int operation = 0;	
	
	if(strncmp(commande_base, "quit" , 4) == 0)
	{
		*continuer = FALSE;

		operation = -1;
	}

	else if(strncmp(commande_base, "send", 4) == 0)
	{
		char* utilisateur = strtok(NULL, " ");

		printf("L'utilisateur est : %s\n", utilisateur);

		operation = 1;
	}

	return operation;
}
