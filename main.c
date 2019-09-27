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

	FILE* fichier = fdopen(fd, "r+"); //Creation d'un "fichier" a partir du fd 

	if(fichier) //On teste l'ouverture du fichier 
	{
		int continuer = TRUE; //Booleen qui sert a sortir de la boucle si le message envoye est "quit"

		do
		{
			char commande[TAILLE_COMMANDE];
			char result[TAILLE_COMMANDE]; //resultat de la commande

			fgets(commande, TAILLE_COMMANDE, stdin);

			int type_commande = teste_commande(commande, &continuer);

			if(type_commande != -1)
			{

				fprintf(fichier, "%s" ,commande);
	
				fgets(result, TAILLE_COMMANDE, fichier);
		
				fprintf(stdout, "%s", result);
			}

		}while(continuer);

		fclose(fichier); //On ferme le fichier
	}

	return EXIT_SUCCESS;
}

