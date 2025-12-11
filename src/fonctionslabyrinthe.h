// L2 info
// TP4 - Recherche du chemin le plus court avec une file

void afficher_lab(int lab[N][M]);
void nettoyer(int lab[N][M]);
int chercher_chemin(int lab[N][M],int xd, int yd, int xa, int ya);
void marquer_chemin(int lab[N][M],int x, int y,int* lgmin);
void init_lab(int lab[N][M]);
int valides(int i, int j);
int est_vide(int i, int j, int lab[N][M]);
int est_mur(int i, int j, int lab[N][M]);
int blocage(int i, int j, int lab[N][M]);
void creer_lab(int lab[N][M]);

