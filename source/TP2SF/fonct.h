#define TAILLE_MAX 100000
#define Taille_Bloc 512
#define T_Taille 3
#define T_Cle 6


typedef struct tabchaine tabchaine ;
struct tabchaine
{
  char str[70];
  int blc;
  int pos;
 };

typedef struct Tbloc
{
    char chaine[Taille_Bloc+1];
    int nb_to; //taille occupe donc nbr de car inseres
    int num_bloc; // numero du bloc
    int der_tai;//la derniere taille de l<enreg du bloc
}Tbloc;

typedef struct Tbloc Buffer;
typedef char semienreg[Taille_Bloc+1];// un semi-enregistrement = chaine representatrice de l'enregistrement
typedef struct Enreg
{
    int taille;// taille de la taille = 3
    int cle; // cle < = 999999, taille de la cle = 6
    int sup; // booléen
    char info[Taille_Bloc-T_Taille-T_Cle];// la taille indiqué dans la chaine representative de l'enregistrement est la taille de l'info
                                          // c-a-d ( la taille de la chaine representative - T_Taille-T_Cle-1)
} Enreg;
typedef struct Entete
{
    int adr_dernier_bloc; //numero d'ordre
    int nbre_enreg; //nombre d'enregistrement dans le fichiers (qui ne sont pas supprimés)
    int nbre_sup;   //nombre d'éléments supprimés logiquement
}Entete;
typedef struct TnOVnC
{
    FILE *F;
    Entete entete;
}TnOVnC;


TnOVnC *ouvrir(char *filename,char mod) ;// mod = 'A' ancien (rb+) || mod = 'N' nouveau (wb+)
void fermer(TnOVnC *pF);
int entete(TnOVnC *pF,int i);
void aff_entete(TnOVnC *pF,int i,int val);
void liredir(TnOVnC *pF,int i,Buffer *buf);
void ecriredir(TnOVnC *pF,int i,Buffer buf);
int alloc_bloc(TnOVnC *pF);
int alea(int min, int max);
void alea_chaine(int n,char *chaine);
void alea_enre(char* chenr) ; // generer un enregistrement aleatoirement
void chargement_initial(int n);
void affichage(const char *fname,int i);//afficher le bloc de numero i dans ke fichier nomme "fname"
void affich_entete(const char *fname);//afficher le contenu de l'entete du fichier "fname"
void epuration();//operation de l'Epuration du fichier sequentiel
void affich_tab(tabchaine t[],int k);//k: nombre de case à afficher
int fct_comp(const void *objet1,const void *objet2);//comparer entre les chaines des cases du tableau
int comp_ch(tabchaine t[],int i,int j);//comparer entre les chaines des cases 'i' et 'j'
int comp(const void *objet1,const void *objet2);//comparer entre 2 cases du tableau selon le num bu bloc
void miseA_Jour(int nb_sup,int nb_mod,int nb_Ajout);//Mettre a jour les fichiers
