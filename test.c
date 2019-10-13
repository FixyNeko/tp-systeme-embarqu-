#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAILLE_COMMANDE 4096
#define TAILLE_DATE 20

#define TRUE 1
#define FALSE 0

char* separe_nom(char commande[TAILLE_COMMANDE]);
char* separe_message(char commande[TAILLE_COMMANDE]);
void date_toString(char date[TAILLE_DATE]);
int ajouter_dans_fichier(const char* nomFichier, char commande[TAILLE_COMMANDE]);
int lecture_dans_fichier(const char* nomFichier);

int main()
{
    ajouter_dans_fichier("test.txt", "send nicoco bienvenue a tous\n");
    ajouter_dans_fichier("test.txt", "send luki yo tout le monde\n");
    ajouter_dans_fichier("test.txt", "send paolo ca va bien ?\n");

    lecture_dans_fichier("test.txt");

    return 0;
}

char* separe_nom(char commande[TAILLE_COMMANDE])
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

char* separe_message(char commande[TAILLE_COMMANDE])
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


int ajouter_dans_fichier(const char* nomFichier, char commande[TAILLE_COMMANDE])
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
