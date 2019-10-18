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
			int choix = 0;

			affiche_menu(); //Affichage du menu

			do
			{
				scanf("%d%*c", &choix); //Saisie de la commande et on mange \n present dans le buffer
			}while(choix > 6 || choix < 0);

			switch(choix)
			{
				case 0:
					continuer = FALSE;
					break;
				case 1:
					{
						char commande[TAILLE_COMMANDE]; //Commande a saisir
			
						fgets(commande, TAILLE_COMMANDE, stdin);

						fprintf(fichier, "%s" ,commande);
					}

					break;
	
				case 2:
					{
						char commande[TAILLE_COMMANDE]; //Commande a saisir
						char result[TAILLE_COMMANDE]; //Resultat de la commande
			
						fgets(commande, TAILLE_COMMANDE, stdin);

						fprintf(fichier, "%s" ,commande);

						fgets(result, TAILLE_COMMANDE, fichier);
		
						fprintf(stdout, "%s", result);
					}
			
					break;

				case 3:
					{
						char commande[TAILLE_COMMANDE]; //Commande a saisir
			
						fgets(commande, TAILLE_COMMANDE, stdin);

						fprintf(fichier, "%s" ,commande);
					
						ajouter_dans_fichier("message_envoye.txt", commande);
					}

					break;

				case 4:
					lecture_dans_fichier("message_envoye.txt");
					break;

				case 5:
					lecture_dans_fichier("message_recu_non_lu.txt");
					break;
				
				case 6:
					lecture_dans_fichier("message_recu_deja_lu.txt");
					break;
			}
		}while(continuer);

		fclose(fichier); //On ferme le fichier
	}

	return EXIT_SUCCESS;
}



