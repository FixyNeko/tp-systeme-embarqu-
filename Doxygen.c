#define IP "192.168.102.249"
#define PORT "1337"
#define PORT_LOG "4242"

#define TAILLE_COMMANDE 4096
#define TAILLE_NOM 20
#define TAILLE_MESSAGE 4096
#define TAILLE_DATE 20

#define TRUE 1
#define FALSE 0

#define MESSAGE_SERVEUR 2
#define MESSAGE_UTILISATEUR 3

#define ERROR_IF(F) if(F) { perror(#F); exit(errno); }

struct reception
{
	FILE* fichier;
	int* continuer;
	char message_retour[TAILLE_MESSAGE];
	int pipe_entree;
};

int tcp_connect(char* addr, char* port);
char* separe_nom(char commande[TAILLE_COMMANDE]);
char* separe_message(char commande[TAILLE_COMMANDE]);
void date_toString(char date[TAILLE_DATE]);
int ajouter_dans_fichier(const char* nomFichier, char* commande);
int lecture_dans_fichier(const char* nomFichier);
void* receive(void* r);
int type_message_retour(char message_retour[TAILLE_MESSAGE]);
void nickError(char code_erreur);
void sendError(char code_erreur);

/**
 * \file tcp.c
 * \brief Programme permettant la communication avec le serveur
 * \author Nicolas LOXOL et Felix DHUMES
 * \version 1.0
 * \date 15/11/2019
 *
 * Ensemble de fonctions permattant de communiquer avec le serveur.
 * Cela inclut une ecoute permanente de celui-ci et la possibilitee
 * d'executer certaines commandes a sa destination.
 */


/**
 * \def IP
 * Adresse IP du serveur.
 */

/**
 * \def PORT
 * Port associe au serveur.
 */

/**
 * \def PORT_LOG
 * Port associe au log du serveur.
 */

/**
 * \def TAILLE_COMMANDE
 * Nombre de caractere maximal pour une commande.
 */

/**
 * \def TAILLE_NOM
 * Nombre de caractere maximal pour un nom d'utilisateur.
 */

/**
 * \def TAILLE_MESSAGE
 * Nombre de caractere maximal pour un message.
 */

/**
 * \def TAILLE_DATE
 * Nombre de caractere exacte pour la date actuel.
 */

/**
 * \def TRUE
 * Simulation de la valeur booleene Vrai (1).
 */

/**
 * \def FALSE
 * Simulation de la valeur booleene Faux (0).
 */

/**
 * \def MESSAGE_SERVEUR
 * Entier associe au message renvoye par le serveur.
 */

/**
 * \def MESSAGE_UTILISATEUR
 * Entier associe au message renvoye par un utilisateur.
 */

/**
 * \def ERROR_IF(F)
 * Macro verifiant si le predicat F est Faux.
 * Si celui-ci est faux une erreur est generee et le probleme est quite.
 */

/**
 * \struct reception
 * \brief Variable qui contient les variables necessaires
 * pour ecouter en permanence le serveur et transmettre celle-ci
 * au main.
 * \param fichier Une variable de type FILE* representant le file descripteur associe a l'ecoute du serveur.
 * \param continuer Un pointeur sur entier qui permet de savoir si l'on peut arreter ou non la communication avec le serveur.
 * \param message_retour Un tableau de caractere qui contient les messages en provenance du serveur.
 * \param pipe_entree Un entier qui correspond a l'entree d'un pipe.
 */

/**
 * \fn int tcp_connect(char* addr, char* port)
 * \brief Realise une connection en tcp sur une adresse et un port fournit un parametre.
 * \param addr Une chaine de caractere representant l'adresse IP du serveur.
 * \param port Une chaine de caractere representant le port du serveur.
 * \return Retourne un file descriptor sur le serveur.
 */

/**
 * \fn char* separe_nom(char commande[TAILLE_COMMANDE])
 * \brief Separe le nom d'un utilisateur d'une commande.
 * \param commande La chaine de caractere contenant la commande a executer.
 * \return Retourne la chaine de caractere contenant le nom de l'utilisateur.
 */

/**
 * \fn char* separe_message(char commande[TAILLE_COMMANDE])
 * \brief Separe le message d'un utilisateur d'une commande.
 * \param commande La chaine de caractere contenant la commande a executer.
 * \return Retourne la chaine de caractere contenant le message saisie par l'utilisateur.
 */

/**
 * \fn void date_toString(char date[TAILLE_DATE])
 * \brief Convertit une date en chaine de caractere.
 * \param date Un tableau de caractere contenant la date (modifie dans la fonction via pointeur).
 */

/**
 * \fn int ajouter_dans_fichier(const char* nomFichier, char* commande)
 * \brief Ajoute une chaine formatee dans un fichier texte.
 * \param nomFichier Le nom du fichier texte present sous forme de chaine de caractere.
 * \param commande La commande saisie par l'utilisateur (heure / nom utilisateur / message).
 * \return Retourne un booleen qui permet de savoir si l'ecriture a fonctionné.
 */

/**
 * \fn int lecture_dans_fichier(const char* nomFichier)
 * \brief Lit un fichier texte.
 * \param nomFichier Le nom de fichier texte present sous forme de chaine de caractere.
 * \return Retourne un booleen qui permet de savoir si la lecture a fonctionné.
 */

/**
 * \fn void* receive(void* r)
 * \brief Lit en permanence les informations (message / ack) du serveur.
 * \param r Un pointeur sur notre structure reception.
 * \return retourne NULL (fonction d'un thread).
 */

/**
 * \fn int type_message_retour(char message_retour[TAILLE_MESSAGE])
 * \brief Permet de savoir si les messages adresses par le serveur sont des ack
 * ou des messages des autres utilisateurs.
 * \param message_retour La chaine de caractere symbolisant le message du serveur.
 * \return Retourne le type de message (MESSAGE_SERVEUR / MESSAGE_UTILISATEUR).
 */

/**
 * \fn void nickError(char code_erreur)
 * \brief Affiche l'erreur adequate selon la commande nick saisie.
 * \param code_erreur Un caractere symbolisant le code d'erreur.
 */

/**
 * \fn sendError(char code_erreur)
 * \brief Affiche l'erreur adequate selon la commande send saisie.
 * \param code_erreur Un caractere symbolisant le code d'erreur.
 */