#include <stdio.h>
#include <conio.h>
#include <time.h>
#include "fonct.h"


    int chg=0;
char affichage_Menu()
{
    char Choix = '0';

    system("cls");
    system("color 9f");
    do
	{
    system("cls");
    system("color 3f");
    printf("\n\n\n\n\n\n\n");
    printf("                          *****************************                       \n\n");
    printf("                          |  1- Chargement initial    |               \n\n\n");//L'Index sera trié automatiquement
    printf("                          |  2- Epuration             |                        \n\n\n");
    printf("                          |  3- Mise a jour           |                         \n\n");
    printf("                          |  4- Entetes               |                         \n\n");
    printf("                          |  5- Quitter               |                         \n\n");
    printf("                          *****************************                       \n");
    printf("\n\n\n\n\n\n\nChoix :\t");

//    while(Choix <'1' || Choix >'5')
//    {
//
//        time_t t =time(NULL);
//        gotoxy(45,27);
//        printf("Date is : %s\n", ctime(&t));
//        Choix=getch();
//    }
        Choix=getch();




  	} while (Choix <'1' || Choix >'5');
	system("cls");
	return Choix;
}

/*******************************************************************************/
/*******************************************************************************/

void menuSwitch()
{
    int n=0,nb_sup=0,nb_mod=0,nb_ajout=0;
    char ch='0';
    do
    {
    char qt = '0';
        ch=affichage_Menu();
        switch(ch)
        {
            case '1':
                system("color 4f");
                printf("Introduire le nombre de codes:\t");
                scanf("%d",&n);
                chargement_initial(n);
                chg=1;
                printf("Quel bloc voulez-vous voir :\t");
                scanf("%d",&n);
                affichage("fseq",n);
                gotoxy(0,24);
                printf("\n\n\n\nAppuyer sur une touche pour retourner au menu principal    \t");
                getche();

                break;
            case '2':
                system("color 4f");
                if(chg==0){printf("Le chargement initial n'est pas encore fait !");}
                else
                {
                    epuration();
                    printf("Quel bloc voulez-vous voir :\t");
                    scanf("%d",&n);
                    affichage("ftrie",n);
                }

                gotoxy(0,22);
                printf("\n\n\n\nAppuyer sur une touche pour retourner au menu principal    \t");
                getche();

                break;
            case '3':
                system("color 4f");

                printf("Veuillez donner le nombre de supp:\t");scanf("%d",&nb_sup);
                printf("Veuillez donner le nombre de modi:\t");scanf("%d",&nb_mod);
                printf("Veuillez donner le nombre d'ajout:\t");scanf("%d",&nb_ajout);
                miseA_Jour(nb_sup,nb_mod,nb_ajout);
                gotoxy(0,21);
                printf("\n\n\n\nAppuyer sur une touche pour retourner au menu principal    \t");
                getche();
                break;
            case '4':
                system("color 4f");
                puts("\n");
                affich_entete("fseq");
                affich_entete("ftrie");
                affich_entete("fseqCP");
                gotoxy(0,24);
                printf("\n\n\n\nAppuyer sur une touche pour retourner au menu principal    \t");
                getche();

                break;
            case '5':
                system("color Cf");
                printf("\nvoulez vous vraiment quitter ?   ( 1/0 )");
            qt=getch();

            if(qt=='0')
            {
                ch='0';
            }
            if(ch!='0')
            {
                system("cls");
                system("color 08");
                dessin(0);
                dessin(61);
                system("color FC");


    printf("\t\t\trealise par:\n");
printf("*\t _____   ____        __                                               *\n");
printf("*\t|  _ \ | |____|   || / /                                               *\n");
printf("*\t| |_| | | |__    ||/ /                                                *\n");
printf("*\t|  _  { |  __|   |/ /         Ahmed Khernache Mohammed                *\n");
printf("*\t| |_| | | |___   / /                                                  *\n");
printf("*\t|_____/ |_____| /_/                                                   *\n");
puts("*                                                                             *");
    printf("*\t    ___   _____   _    _____      ___  ___       ___   _   _          *\n");
    printf("*\t   /   | |  _  \| | |  / _  /     /   |/   |     /   | | | | |         *\n");
   printf("*\t  / /| | | |_| | | | | |___     / /|   /| |    / /| | | | | |         *\n");
  printf("*\t / / | | |  _  { | | \___  \     / |__/ | |     / | | | | | |   Walid   *\n");
 printf("*\t/ /  | | | |_| | | |  ___| |  / /       | |  / /  | | | | | |___      *\n");
printf("*      /_/   |_| |_____/ |_| /_____/ /_/        |_| /_/   |_| |_| |_____|     *\n");
puts("*                                                                             *\n*                                                                             *\n*                                                                             *");
//}

printf("*******************************************************************************");
exit(1);

            }
        break;


            default:
            break;
        }
    }while(ch!='5');

}
/*******************************************************************************/
/*******************************************************************************/

void dessin(int i)
 {
   textcolor(2);
   gotoxy(i,0);
   printf(" _____   _____   _\n");
   gotoxy(i,1);
   printf("| ____| /  ___/ | |\n");
   gotoxy(i,2);
   printf("| |__   | |___  | |\n");
   gotoxy(i,3);
   printf("|  __|  \___  \ | |\n");
   gotoxy(i,4);
   printf("| |___   ___| | | |\n");
   gotoxy(i,5);
   printf("|_____| /_____/ |_|\n");
   textcolor();
 }


void prepresentatation()
{
    system("color 2f");
    puts("");
    puts("               Republique Algerienne Democtratique et Populaire\n\n");

    puts("                   Ecole nationale Supeieure d'Informatique\n\n");

    puts("                     Classes Preparatoires Integrees 2CPI\n");
    printf("                                 Groupe: 04                   \n\n");
            puts("                           _____   _____   _____    ");
            puts("                          |_   _| |  _  \  /___  \   ");
              puts("                            | |   | |_| |  ___| |   ");
              puts("                            | |   |  ___/ /  ___/   ");
              puts("                            | |   | |     | |___    ");
              puts("                            |_|   |_|     |_____|   ");


    puts("\n\n                     Encadre par M. :Boulekradeche Mohammed");
    puts("\n\n\n\n\n\n\n                 Annee universitaire :2013-2014  /*\\ 1434-1435 ");
    getche();
}
