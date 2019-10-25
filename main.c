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
				printf("Faites un choix : ");
				scanf("%d%*c", &choix); //Saisie de la commande et on mange \n present dans le buffer
				printf("\n");

			}while(choix > 6 || choix < 0);

			switch(choix)
			{
				case 0:
					continuer = FALSE;

					break;
				case 1:
					{
						char commande[] = "nick";
						char nom[TAILLE_NOM];
						char* commande_concat = NULL;

						printf("Veuillez saisir un nom : ");

						fgets(nom , TAILLE_NOM, stdin);
						
						printf("\n");

						int taille_commande_reel = 5 + strlen(nom) + 1; //On rajoute \0

						commande_concat = (char*)malloc(taille_commande_reel * sizeof(char)); 

						snprintf(commande_concat, taille_commande_reel, "%s %s%c", commande, nom, '\0');

						fprintf(fichier, "%s" ,commande_concat);

						fflush(fichier);

						free(commande_concat);
					}

					break;
	
				case 2:
					{
						char commande[] = "list\n";
						char result[TAILLE_COMMANDE]; //Resultat de la commande

						fprintf(fichier, "%s" ,commande);
								
						fflush(fichier);

						fgets(result, TAILLE_COMMANDE, fichier);
		
						fprintf(stdout, "%s", result);
					}
			
					break;

				case 3:
					{
						char commande[] = "send";
						char destinataire[TAILLE_NOM];
						char message[TAILLE_MESSAGE];
						char* commande_concat = NULL;

						printf("Veuillez saisir le destinaire : ");

						fgets(destinataire , TAILLE_NOM, stdin);

						printf("\nVeuillez saisir votre message : ");
						
						fgets(message , TAILLE_MESSAGE, stdin);

						printf("\n");

						int taille_commande_reel = 5 + strlen(destinataire) + 1 + strlen(message) + 1; //On rajoute un espace et \0

						commande_concat = (char*)malloc(taille_commande_reel * sizeof(char));
						
						destinataire[strlen(destinataire) - 1] = '\0';

						snprintf(commande_concat, taille_commande_reel, "%s %s %s%c", commande, destinataire, message,'\0');

						fprintf(fichier, "%s" ,commande);
			
						fflush(fichier);
					
						ajouter_dans_fichier("message_envoye.txt", commande_concat);

						free(commande_concat);
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
				
				default:
					continuer = FALSE;
					break;
			}
		}while(continuer);

		fclose(fichier); //On ferme le fichier
	}

	return EXIT_SUCCESS;
}



