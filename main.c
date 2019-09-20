#include <stdio.h>
#include <stdlib.h>

#include "tcp.h"

int main()
{
	int fd;  //Ouverture du File descriptor

	while((fd = tcp_connect(IP, PORT)) == -1) //On teste la connection au serveur 
	{
		fprintf(stderr ,"Erreur lors de la connection\n");
	}

	fprintf(stdout, "Connexion au serveur %s reussit\n", IP);


	return EXIT_SUCCESS;
}

//fdopen

