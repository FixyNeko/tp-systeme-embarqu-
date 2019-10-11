#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
			char commande[TAILLE_COMMANDE]; //Commande a saisir
			char result[TAILLE_COMMANDE]; //Resultat de la commande
			
			
			fgets(commande, TAILLE_COMMANDE, stdin);

			if(strncmp(commande, "exit" , 4) != 0)
			{
				if(strncmp(commande, "send", 4) == 0)
				{
					ajouter_dans_fichier("message_envoye.txt", commande);
				}

				fprintf(fichier, "%s" ,commande);
	
				fgets(result, TAILLE_COMMANDE, fichier);
		
				fprintf(stdout, "%s", result);
			}
			
			else
			{
				continuer = FALSE;
			}

		}while(continuer);

		fclose(fichier); //On ferme le fichier
	}

	return EXIT_SUCCESS;
}

