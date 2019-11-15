#include "tcp.h"

#ifdef RASP
#include "led.h"
#endif

int main()
{
	#ifdef RASP
	init_leds();
	#endif
	
	int fd;  //Ouverture du File descriptor

	while((fd = tcp_connect(IP, PORT)) == -1) //On teste la connection au serveur 
	{
		fprintf(stderr ,"Erreur lors de la connection\n");
	}

	fprintf(stdout, "Connexion au serveur %s reussit\n", IP);

	FILE* fichier = fdopen(fd, "r+"); //Creation d'un "fichier" a partir du fd
	FILE* fichier_recevoir = fdopen(fd, "r+"); //Creation d'un "fichier numero 2" a partir du meme fd pour ecouter en permanence le serveur

	if(fichier && fichier_recevoir) //On teste l'ouverture du fichier 
	{	
		int continuer = TRUE; //Booleen qui sert a sortir de la boucle si le message envoye est "quit"

		int io_pipe[2]; //Tableau pour le pipe

		pipe(io_pipe); //io_pipe[0] --> sortie io_pipe[1] --> entree

		struct reception r = {.fichier = fichier_recevoir, .continuer = &continuer, .pipe_entree = io_pipe[1]};
	
		pthread_t tid; //Declaration du thread
		
		pthread_create(&tid, NULL, receive, &r); //Initialisation du thread sur la fonction receive
		
		do
		{
			int choix = 0; //Choix dans le menu

			lecture_dans_fichier("Menu.txt"); //Affichage menu

			do
			{
				printf("Faites un choix : ");
				scanf("%d%*c", &choix); //Saisie de la commande et on mange \n present dans le buffer
				printf("\n");

			}while(choix > 6 || choix < 0);

			switch(choix)
			{
				case 0:
					continuer = FALSE; //On quitte le programme

					break;
				case 1:
					{
						char commande[] = "nick";
						char nom[TAILLE_NOM];
						char reponse_serveur[TAILLE_MESSAGE] = {0};
						char* commande_concat = NULL;

						printf("Veuillez saisir un nom : ");

						fgets(nom , TAILLE_NOM, stdin);
						
						printf("\n");

						int taille_commande_reel = 5 + strlen(nom) + 1; //On rajoute \0

						commande_concat = (char*)malloc(taille_commande_reel * sizeof(char)); 

						snprintf(commande_concat, taille_commande_reel, "%s %s%c", commande, nom, '\0'); //Concatenation des chaines

						fprintf(fichier, "%s" ,commande_concat);

						fflush(fichier); //On force l'ecriture

						free(commande_concat);

						read(io_pipe[0], reponse_serveur, TAILLE_MESSAGE);

						fprintf(stdout, "%s", reponse_serveur);
					}

					break;
	
				case 2:
					{
						char commande[] = "list\n";
						char reponse_serveur[TAILLE_MESSAGE] = {0};

						fprintf(fichier, "%s" ,commande);
								
						fflush(fichier); //On force l'ecriture

						read(io_pipe[0], reponse_serveur, TAILLE_MESSAGE);

						fprintf(stdout, "%s", reponse_serveur);
					}
			
					break;

				case 3:
					{
						char commande[] = "send";
						char destinataire[TAILLE_NOM];
						char message[TAILLE_MESSAGE] = {0};
						char* commande_concat = NULL;
						char reponse_serveur[TAILLE_MESSAGE];

						printf("Veuillez saisir le destinaire : ");

						fgets(destinataire , TAILLE_NOM, stdin);

						printf("\nVeuillez saisir votre message : ");
						
						fgets(message , TAILLE_MESSAGE, stdin);

						printf("\n");

						int taille_commande_reel = 5 + strlen(destinataire) + 1 + strlen(message) + 1; //On rajoute un espace et \0

						commande_concat = (char*)malloc(taille_commande_reel * sizeof(char));
						
						destinataire[strlen(destinataire) - 1] = '\0'; //On remplace \n par \0

						snprintf(commande_concat, taille_commande_reel, "%s %s %s%c", commande, destinataire, message,'\0'); //Concatenation des strings

						fprintf(fichier, "%s" , commande_concat);
			
						fflush(fichier); //Force affichage
					
						ajouter_dans_fichier("message_envoye.txt", commande_concat); //On ajoute le message au fichier des messages envoyés

						free(commande_concat);

						//read(io_pipe[0], reponse_serveur, TAILLE_MESSAGE);
					}

					break;

				case 4:
					lecture_dans_fichier("message_envoye.txt");
					break;

				case 5:
					lecture_dans_fichier("message_recu_non_lu.txt");
					
					
					#ifdef RASP
					clear_led(VERT);
					#endif
					
					break;
				
				case 6:
					lecture_dans_fichier("message_recu_deja_lu.txt");
					break;
				
				default:
					continuer = FALSE;
					break;
			}
		}while(continuer);

		
		close(io_pipe[0]);
		close(io_pipe[1]);

		fclose(fichier); //On ferme le fichier
	}

	return EXIT_SUCCESS;
}
