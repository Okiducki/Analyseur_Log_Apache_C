// Projet DUT R&T - 1re annee
// Mini-Projet en C
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libgraphique.h"

// Chemins des fichiers : valeurs par defaut, modifiables a la compilation
// (ex. : make LOG=/var/log/apache2/access.log)
#ifndef LOG_FILE
#define LOG_FILE "/var/log/apache2/access.log"
#endif
#ifndef STYLE_FILE
#define STYLE_FILE "style.html"
#endif
#ifndef ATTAQUES_FILE
#define ATTAQUES_FILE "Attaques.txt"
#endif
#ifndef NAVIGATEURS_FILE
#define NAVIGATEURS_FILE "Navigateurs.txt"
#endif

int convert (float verg)// permet de convertir des float en entier -> 2 lignes
{ 
	int ent = (int) verg;
	return ent;
}

void lecture_style() //-> 11 lignes
{
	FILE *fd;
	char str[1024];
	if ((fd = fopen(STYLE_FILE,"r")) == NULL)
		fprintf(stderr,"ERREUR DE CHARGEMENT DE FICHIER\n");
	else {
	while (!feof(fd)){
		fgets(str,1024,fd);
		printf("%s",str);}
	fclose(fd);}
}

void versionweb(float total,float *tabmois)// donne la version web -> 18 lignes
{
	int connex = convert(total);// permet d'avoir total en entier
	printf("Content-type: text/html\n\n"); 
	printf("<!doctype html>\n");
	printf("<html lang=\"fr\">\n"); 
	printf("<body>\n");
	lecture_style();
	printf("Depuis le 01/Jan/2018 on a enregistr&eacute; %d connexions.<br>\n",connex);
	printf("<div id = \"vertgraph\">\n");
	printf("    <ul>\n");
	int abs=10; // On initialise les abscisses
	for(int i=0;i<12;i++){ // On rempli ici le graphique du web pour chaque mois
		int pourcent = convert (tabmois[i]);
		printf("        <li style=\"left: %dpx;  height: %fpx;\">%d</li>\n",abs,tabmois[i]*10,pourcent);
		abs+=31;} // la largeur est de 30 px et il y a un espace de 1 px
	printf("    </ul>\n");
	printf("</div>\n");
	printf("</body>\n\n"); 
	printf("</html>\n");
}

void dep(float total,char *depuis) // Permet de faire la concaténation en réduisant la taille de la fonction versiongraphqiue -> 7 lignes
{
	char connexion[255], fin[255];
	strcpy(depuis,"Depuis le 01/Jan/2018 on a enregistre ");
	int connex = convert (total);
	sprintf(connexion,"%d",connex); // La librairie n'accepte que les strings
	strcpy(fin," connexions.");
	strcat(depuis,connexion);
	strcat(depuis,fin); //Pas besoin de return car on modifie un tableau
}

void versiongraphique(float total, float *tab) //donne la version graphique -> 26 lignes = 6 lignes en trop
{
	Point p = {30,50};
	ouvrir_fenetre(430,350);
	dessiner_rectangle((Point){0,0},430,350,blanc);// Ouverture de la fenetre blanche

	char depuis[255];
	dep(total,depuis);// fait la concaténation
	
	char texte[255];

	int abs = 30;// initialise les abscisses

	afficher_texte(depuis,12,p,noir); // affiche le nb de connexions

	p.y = 300;// On change l'ordonnée du point p

	afficher_texte("Jan  Fev Mar Avr  Mai Juin Juil Aou Sep Oct Nov Dec",14,p,vert);  

	for(int i=0;i<12;i++){// affichage des rectangles et des stats pour chaque mois

		float hauteur=tab[i]*10;
		int ordon = convert(300-hauteur);

		Point p2 = {abs,ordon};
		Point p3={abs+2,ordon};// décalge de 2 px pour l'affichage des stats

		int taille = convert (hauteur);
		dessiner_rectangle(p2,30,taille,orange);
		
		float nb = tab[i];
		int pourcentage = convert (nb);// sprintf n'accepte que les entier
		sprintf(texte,"%d",pourcentage);
		afficher_texte(texte,15,p3,blanc);  
		abs += 31;}// Comme pour le web 

	actualiser();
	attendre_clic();
	fermer_fenetre();
}

void versiontexte(float total, float *tab) //Donne la version texte -> 7 lignes
{
	char* tabAssocs[] = {"Janvier", "Fevrier", "Mars", "Avril", "Mai", "Juin", "Juillet", "Aout", "Septembre", "Octobre", "Novembre", "Decembre"}; // tableau pour l'affichage
	int connex = convert (total);
	printf("Depuis le 01/Jan/2018 on a enregistre %d connexions.\n",connex);
	for (int i = 0; i < 12; i++) // affichage des mois et des stats avec 2 chiffres après la virgule
		printf("%s :  %.1f %%\n",tabAssocs[i] , tab[i] );
	printf("Veuillez consulter le fichier " ATTAQUES_FILE " pour voir les possibles hackeur du site\n");
	printf("Veuillez consultez le fichier " NAVIGATEURS_FILE " pour visualiser les navigateurs utilisés lors de chaque requete\n");
}

void reset (char * chemin) // ->2 lignes
{
	FILE* fichier = NULL; // Warning mais bon....
	fichier = fopen(chemin, "w+"); // w != a car il efface alors que a ajoute
	//fclose(fichier); //pproblème crée une erreur sur serveur but why ??
}

void attaq (char *array) // Test les attaques -> 12 lignes
{
	int h =0;
	char att[1000];
	
	int len = strlen(array);
	if (len >900){//test pour les attaques
		for(;array[h] !=' ';h++) // similaire à line sur les ip cette fois
			att[h]=array[h];
		att[h]='\0';
		FILE* fichier = NULL;
		fichier = fopen(ATTAQUES_FILE, "a"); // On enregistre toutes les ip dans ce fichier .txt
		if (fichier != NULL){
			fprintf(fichier,"%s a sûrement attaqué le site\n",att);
			fclose(fichier);}
	}
}

void navig (char *array) //Récupère les navigateurs -> 15 lignes
{
	int ba =0;
	int bb = 0;
	char nav[1000];
	for(;array[ba] !='(' && array[ba] != '\0';ba++); // Meme procédure que line mais un peu plus complexe
	if (array[ba] == '\0') // pas de parenthese : pas de navigateur identifiable
		return;
	for(; ba > 0 && array[ba] != '"'; ba--); // demi tour
	ba++;
	for (; array[ba] != '/' && array[ba] != '\0' && bb < 999; ba++, bb++) //enregistrement (borne : taille de nav)
		nav[bb]=array[ba];
	nav[bb]='\0';
	FILE* fa = NULL;
	fa = fopen(NAVIGATEURS_FILE, "a"); // On enregistre tous les navigateurs dans ce fichier .txt
	if (fa != NULL){
		fprintf(fa,"%s\n",nav);
		fclose(fa);}
}

char* line(char* array) // Fonction pour ne récupérer que le mois dans le tableau array -> 12 lignes
{
	int i = 0;
	int j = 0;
	static char mem[5000];// il y a eu des attaques donc certaines phrases sont longues (static : le tableau doit survivre au return)
	attaq(array); // Va tester les attaques
	navig(array); // Va tester les navigateurs
	for (; array[i]!='/'; i++);// Tant que l'on a pas trouvé un / on incrémente
	i++;
	for (; array[i] != '/'; i++, j++) // Meme chose pour enregistrer cette fois ci
		mem[j]=array[i];
	mem[j]='\0';
	array=mem;
	return array;
}

void calcul(float * compt,char **av,int ac) // Va permettre de faire les stats, le total et la redirection
{
	float total = 0;

	for (int a = 0; a < 12; a++) // Permet d'avoir le total de connexions
		total += compt[a];

	for (int b = 0; b < 12; b++) // Permet de faire les stats de chaque mois
		compt[b] = (compt[b]*100)/total;
	

	if (ac > 1) {
		if (strcmp(av[1],"-txt")==0)
			versiontexte(total,compt);
		else if (strcmp(av[1],"-gr")==0)
			versiongraphique(total,compt);
		else // on sécurise les arguments 
			printf("Recommence avec les arguments suivants:\n -txt pour le texte\n -gr pour le graphique\n et vide pour la version web\n");}
	else
		versionweb(total,compt);
}

int tabmois(char* string) //Permet de comparer les mois avec le mois récupéré par la fonction line
{
	char* tabAssoc[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};// tableau de comparaison
	for(int i = 0; i< 12; i++){ // On test pour chaque mois si ça correspond
		if(strcmp(string, tabAssoc[i]) == 0) 
			return i;}// permet de renvoyer dans compt quel mois a été récupéré
	return -1; // coupe le getline (plus de ligne = plus de mois donc strcmp stoppé)
}

void enregistrement(FILE *fd, char **av,int ac) // détermine le nombre de connexion par mois et l'enregistre
{
	char *array = NULL;// methode getline
	size_t longueur = 0;
	ssize_t read;
	int mois;
	float compt[12];// pour le nb de connexions sur chaque mois
	for(int b=0;b<12;b++) // Initialisation à 0 par sécurité
		compt[b]=0;
	while ((read = getline (&array,&longueur,fd)) != -1) {
		mois = tabmois(line(array));// détermine le mois récupéré
		compt[mois]++;} // ce mois s'incrémente de 1 connexion
	free(array);
	calcul(compt,av,ac); // redirection vers le calcul des stats
}

int ouverture(char **av, int ac) // Ouverture du fichier acces.log
{
	FILE *fd;
	if ((fd = fopen(LOG_FILE,"r")) == NULL) { //Si il y a une erreur mettre un message
		write(2, "The file doesn't exist\n", 23);
		return (84);}
	enregistrement(fd, av,ac); // Tout se passe bien donc redirection vers l'enregistrement des mois
	return (0);
}

int main(int ac, char **av) //main rapide pour trouver le choix de l'utilisateur
{
	reset(ATTAQUES_FILE); // on initialise les fichiers de stockage
	reset(NAVIGATEURS_FILE);
	ouverture(av,ac); //On lance la suite du programme
	return (0);
}
// Plus ou moins 20 lignes par fonction c'est parfait
// Fin du Mini-Projet en C
// Projet DUT R&T - 1re annee

