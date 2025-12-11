// L2 info
// TP4 - Recherche du chemin le plus court avec une file
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include "pile.h"

// Booleens
#define VRAI 1
#define FAUX 0

// Dimensions du labyrinthe: N lignes et M colonnes (attention: toujours impairs)
#define N 25
#define M 55

// Constantes representant le contenu des cases
#define COULOIR 0
#define MUR -1
#define CHEMIN -2


//****************************************************
// Programme principal
// Cree le labyrinthe, cherche le chemin et l'affiche
//****************************************************
#include"fonctionslabyrinthe.h"

int main(){	
	int labyrinthe[N][M];
	int lgmin;		// longueur du chemin le plus court
	
	srand(time(0));
	system("clear");

	creer_lab(labyrinthe);
	afficher_lab(labyrinthe);
	sleep(1);
/*
	if(chercher_chemin(labyrinthe,N-1,M-1,0,0)){
		marquer_chemin(labyrinthe,0,0,&lgmin);
		system("clear");
		afficher_lab(labyrinthe);
		printf("Chemin le plus court de longueur %i\n",lgmin);
	}
	else
		printf("Pas de chemin solution\n");
*/
	return EXIT_SUCCESS;
}

//*******************************
// Partie affichage du labyrinthe
//*******************************

// Couleurs disponibles
#define BLACK_COLOR        30
#define DARK_RED_COLOR     31
#define DARK_GREEN_COLOR   32
#define YELLOW_COLOR       33
#define DARK_BLUE_COLOR    34
#define PURPLE_COLOR       35
#define DARK_CYAN_COLOR    36
#define LIGHT_GRAY_COLOR   37
#define GRAY_COLOR         90
#define RED_COLOR          91
#define GREEN_COLOR        92
#define ORANGE_COLOR       93
#define BLUE_COLOR         94
#define MAGENTA_COLOR      95
#define CYAN_COLOR         96
#define WHITE_COLOR        97

#define DEFAULT_CHAR_COLOR 97

void couleur(int id_couleur){
// Modifie la couleur d'affichage des prochains caracteres

    printf("\033[%dm", id_couleur);
}


//******************************
// Partie creation du labyrinthe
//******************************

void init_lab(int lab[N][M]){
//Initialise le labyrinthe avec des murs 
	int i,j;

	for(i=0;i<N;i++)
		for(j=0;j<M;j++)
			lab[i][j] = MUR;
}

int valides(int i, int j){
// renvoie VRAI si i et j designent une case de la matrice
	return(i>=0 && i<N &&j>=0 && j<M);
}

int est_vide(int i, int j, int lab[N][M]){
// renvoie VRAI si i et j designent une case couloir
	return(valides(i,j) && lab[i][j]==COULOIR);
}

int est_mur(int i, int j, int lab[N][M]){
// renvoie VRAI si i et j designent une case mur
	return(valides(i,j) && lab[i][j]==MUR);
}

int blocage(int i, int j, int lab[N][M]){
// renvoie VRAI si aucune case voisine n'est un mur
	return (!est_mur(i+2,j,lab) && !est_mur(i-2,j,lab)
		&& !est_mur(i,j+2,lab) && !est_mur(i,j-2,lab));
}

void creer_lab(int lab[N][M]){
// Cree le labyrinthe de maniere aleatoire avec une pile

	int termine=FAUX;	// booleen indiquant si labyrinthe est termine
	int trouve=FAUX;	// booleen indiquant si une case voisine MUR a ete trouvee
	int i,j,alea;		// alea permet de prendre une case voisine au hasard

	init_lab(lab);
	initpile();

	i=0, 
	j=0;
	lab[0][0]=COULOIR;

   while(!termine){

	if(blocage(i,j,lab)){
	// cas de blocage: repartir de la derniere case visitee, stockee dans la pile
		if(!pilevide()){
			depiler(&j);
			depiler(&i);
		}
		else
			termine=VRAI;
	}
	else{
	// cas general: chercher une case voisine contenant un mur, abattre ce mur et aller sur cette case
		trouve=FAUX;
		while(!trouve){
			alea=rand()%4; 
			switch(alea){
				case 0: 
					if(est_mur(i+2,j,lab)){ 
						empiler(i);
						empiler(j);
						lab[i+1][j]=COULOIR;
						lab[i+2][j]=COULOIR;
						i=i+2;
						trouve=VRAI;
						break;
					}	
				case 1: 	
					if(est_mur(i-2,j,lab)){
						empiler(i);
						empiler(j);
						lab[i-1][j]=COULOIR;
						lab[i-2][j]=COULOIR;
						i=i-2;
						trouve=VRAI;
						break;
					}	
				case 2: 	
					if(est_mur(i,j+2,lab)){
						empiler(i);
						empiler(j);
						lab[i][j+1]=COULOIR;
						lab[i][j+2]=COULOIR;
						j=j+2;
						trouve=VRAI;
						break;
					}	
				case 3: 	
					if(est_mur(i,j-2,lab)){
						empiler(i);
						empiler(j);
						lab[i][j-1]=COULOIR;
						lab[i][j-2]=COULOIR;
						j=j-2;
						trouve=VRAI;
					}	
			}
		}
	}
   }

}


