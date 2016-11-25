#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <time.h>
#include "fonct.h"

    int CptAcc=0; // Variable Globale qui fait le compte d'acces disque
    FILE *fs=NULL,*ft=NULL,*f=NULL,*fsCP=NULL/*pt sur la copie du fichier seq*/;
    Buffer buf,bufCP,bufTr;
    tabchaine tab[TAILLE_MAX],tabIndex[TAILLE_MAX];
    int nb_lirS=0,nb_lirT=0,nb_lirI=0,nb_ecrS=0,nb_ecrT=0,nb_ecrI=0,nbreLigne=0;
TnOVnC *ouvrir(char *filename,char mod) // mod = 'A' ancien (rb+) || mod = 'N' nouveau (wb+)
{
    TnOVnC (*I)=malloc(sizeof(TnOVnC));
    char s[4];
    if ((mod == 'A') || (mod =='a')) sprintf(s,"rb+");
    else sprintf(s,"wb+");
    (I->F)=fopen(filename,s);
    if ((mod == 'A') || (mod =='a'))
    {

        fread(&(I->entete),sizeof(Entete),1,I->F);

    }
    else
    {
        (I->entete).adr_dernier_bloc=0;             // Initialisation de l'entete
        (I->entete).nbre_enreg=0;
        (I->entete).nbre_sup=0;
        fwrite(&(I->entete),sizeof(Entete),1,I->F);
    }
    return I;
}

void fermer(TnOVnC *pF)
{

    fseek(pF->F,0,0);
    fwrite(&(pF->entete),sizeof(Entete),1,pF->F);  // Mise a jour de l'entête
    fclose(pF->F);
    pF=NULL;
}

int entete(TnOVnC *pF,int i)
{
    if (i==1) return ((pF->entete).adr_dernier_bloc);
    if (i==2) return ((pF->entete).nbre_enreg);
    return ((pF->entete).nbre_sup);
}

void aff_entete(TnOVnC *pF,int i,int val)
{
    if (i==1) (pF->entete).adr_dernier_bloc=val;
    else if (i==2) (pF->entete).nbre_enreg=val;
    else (pF->entete).nbre_sup=val;
}

void liredir(TnOVnC *pF,int i,Buffer *buf)
{
    if (i<=entete(pF,1))
    {
        CptAcc++;
        fseek(pF->F,sizeof(Entete)+(i-1)*sizeof(Tbloc),SEEK_SET);
        fread(buf,sizeof(Tbloc),1,pF->F);
    }
}

void ecriredir(TnOVnC *pF,int i,Buffer buf)
{
    if (i<=entete(pF,1))
    {
        CptAcc++;
        fseek(pF->F,sizeof(Entete)+(i-1)*sizeof(Tbloc),SEEK_SET);
        fwrite(&buf,sizeof(Tbloc),1,pF->F);
    }
}

int alloc_bloc(TnOVnC *pF)
{
    aff_entete(pF,1,entete(pF,1)+1);
    return entete(pF,1);
}

int alea(int min, int max)
{
     int rms,inter;
     rms=rand();
     srand(time(NULL)*rms);
     inter = (rand()%(max - min + 1)) + min;
     return inter;
}

void alea_chaine(int n,char *chaine)
{
    int i=0;
     for(i=0;i<n;i++)
    {
        chaine[i]=alea(97,122);
    }
    chaine[i]='\0';
    return ;
}

void alea_enre(char* chenr)  // generer un enregistrement aleatoirement
{
    int taille=0 ,sup=0,cle=0,tai=0;
    char clech[7]="",taillech[5]="",supch[2]="0" ;
    taille=alea(26,69);
    cle=alea(111111,999999);
    sprintf(taillech,"%d", taille);sprintf(clech,"%d", cle);
    char chaine[taille-9];
    sprintf(chaine,"%s","");
    tai=(taille-9)/3;
    alea_chaine(tai,chaine);
    sprintf(chenr,"%s",taillech);
    sprintf(chenr,"%s%s",chenr,supch);
    sprintf(chenr,"%s%s",chenr,clech);
    sprintf(chenr,"%s%s",chenr,chaine);
    tai=taille-9-tai;
    alea_chaine(tai,chaine);
    sprintf(chenr,"%s%s",chenr,chaine);
    chenr[taille]='\0';
    return ;
}

void chargement_initial(int n)
{
    int i=1,k=0,taille=0;
    char chaine[70]="";
    fs=ouvrir("fseq",'N');
    fsCP=ouvrir("fseqCP",'N');//ouvrir la copie du fichier sequentiel
    aff_entete(fs,2,n);//nbre d'enreg dans le fichier seq
    aff_entete(fsCP,2,n);//nbre d'enreg dans la copie du fichier seq
    aff_entete(fsCP,3,0);aff_entete(fs,3,0);
    alloc_bloc(fs);alloc_bloc(fsCP);
    buf.num_bloc= 1;
    buf.nb_to = 0;
    sprintf(buf.chaine,"",buf.chaine);
    while(k<n)
    {
        strcpy(chaine,"");
        alea_enre(chaine);
        taille=strlen(chaine);
        if( taille<=(Taille_Bloc- buf.nb_to))
        {
            sprintf(buf.chaine,"%s%s",buf.chaine,chaine);
            buf.nb_to += taille;
        }
        else
        {
            buf.chaine[buf.nb_to]='\0';
            ecriredir(fs,i,buf);
            ecriredir(fsCP,i,buf);
            i++;
            sprintf(buf.chaine,"",buf.chaine);
            sprintf(bufCP.chaine,"",bufCP.chaine);
            buf.nb_to=0;
            bufCP.nb_to=0;
            buf.num_bloc= alloc_bloc(fs);
            buf.num_bloc= alloc_bloc(fsCP);
            aff_entete(fs,1,buf.num_bloc);aff_entete(fsCP,1,buf.num_bloc);
            sprintf(buf.chaine,"%s%s",buf.chaine,chaine);
            buf.nb_to += taille;
        }
        k++;
    }
    buf.chaine[buf.nb_to]='\0';
    ecriredir(fs,i,buf);
    ecriredir(fsCP,i,buf);
    fermer(fs);
    fermer(fsCP);
}


void affichage(const char *fname,int i)
{
    int m=0,k=0,taille=0;
    char taich[3]="";
    fs=ouvrir(fname,'A');
    if (i<=entete(fs,1) && i>0)
    {
    liredir(fs,i,&buf);
    printf("Bloc numero : %d du fichier \"%s\" :\n",buf.num_bloc,fname);
    printf("La taille occupee : %d\n",buf.nb_to);
    printf("Derniere taille dans ce bloc: %d \n",buf.der_tai);
    for(m=0;m<buf.nb_to;)
        {
            taich[0]=buf.chaine[m];
            taich[1]=buf.chaine[m+1];
            taich[2]='\0';
            taille=atoi(taich);
            for(k=0;k<taille;k++)
            {
                printf("%c",buf.chaine[k+m]);
                if(k==1 || k==2 || k==8) printf(" ");
            }
            printf("\n");
            m = m + taille;
        }
    }
    else {printf("Error");}
    fermer(fs);
}
void affich_entete(const char *fname)
{
    f=ouvrir(fname,'A');
    printf("\nEntete du fichier  \"%s\" :\n",fname);
    printf("Adresse du dernier bloc :%d\n",entete(f,1));
    printf("Nbre d'enregistrement :%d\n",entete(f,2));
    printf("Nbre d'enregistrement supprime:%d\n",entete(f,3));
    fermer(f);
    return ;
}


void epuration()
{
    int i=0,m=0,p=1,taille=0,c=0,l=0;
    char taich[3]="",ch[70]="";
    /***********************************************************************/
    /*************************Remplir le tableau a trier*******************/
    fs=ouvrir("fseq",'A');
    fsCP=ouvrir("fseqCP",'A');
    i=entete(fs,1);//nbre de blocs
    if(i==0)
    {
        printf("Aucun bloc existe !!\n");
    }
    else
    {
        for(p=1;p<=i;p++)
        {
            m=0;
            liredir(fs,p,&buf);
            while(m<buf.nb_to)
            {
                tab[c].pos = m;
                tabIndex[c].pos = m;
                tab[c].blc=p;
                tabIndex[c].blc=p;
                taich[0]=buf.chaine[m];taich[1]=buf.chaine[m+1];taich[2]='\0';taille=atoi(taich);
                strcpy(ch,"");
                for(l=0;l<taille;l++)
                {
                    ch[l]=buf.chaine[m];
                    m++;
                }
                ch[taille]='\0';
                sprintf(tab[c].str,"",tab[c].str);//initialiser le chaine a vide
                sprintf(tabIndex[c].str,"",tabIndex[c].str);//initialiser le chaine a vide
                sprintf(tab[c].str,"%s",ch);////copier "ch" dans tab[c]
                sprintf(tabIndex[c].str,"%s",ch);////copier "ch" dans tab[c]
                c++;
            }
        }
    }
    nbreLigne=c;
    /************************************************************************/
    /*********************Trier le tableau selon le code :*******************/
    qsort(tab,c,sizeof(tabchaine),fct_comp);
    qsort(tabIndex,c,sizeof(tabchaine),fct_comp);
//    affich_tab(tabIndex,c);
    /*************************************************************************/
    /*****************Indexer la copie du fichier seq :***********************/
    f = fopen("Index.txt","w");
    if(f != NULL)
    {
        for(l=0;l<c;l++)
        {
            fputs(tab[l].str, f );
            sprintf(taich, "%d", tab[l].blc);// l'entier cptL en chaine de caractere
            fputs(taich, f );
            sprintf(taich, "%d", tab[l].pos);//convertirconvertir l'entier cptL en chaine de caractere
            fputs(taich, f );
            fprintf(f,"%c",'\n');
        }
    }
    /************************************************************************/
    /*******Supprimer logiquement les doublons dans les deux tableaux :******/
    int sup=0;
    l=0;
    for(m=1;m<c;m++)
    {
        if(comp_ch(tab,l,m)==0)
        {
            tab[m].str[2]='1';
            tabIndex[m].str[2]='1';
            sup++;
        }
        else{l=m;}
    }
    aff_entete(fs,3,sup);
    aff_entete(fsCP,3,sup);
    /***********************************************************************/
    /*******************Remplir le fichier trie :***************************/
    ft=ouvrir("ftrie",'N');
    buf.num_bloc=alloc_bloc(ft);
    buf.nb_to=0;buf.der_tai=0;
    m=0;//compteur des enreg non supprimes
    sprintf(buf.chaine,"",buf.chaine);
    for(l=0;l<c;l++)
    {
        if(tab[l].str[2]=='0')
        {
            m++;
            taich[0]=tab[l].str[0];taich[1]=tab[l].str[1];taich[2]='\0';taille=atoi(taich);
            if(taille<=(Taille_Bloc-buf.nb_to))
            {
                sprintf(buf.chaine,"%s%s",buf.chaine,tab[l].str);
                buf.nb_to += taille;
                buf.der_tai=taille;
            }
            else
            {
                buf.chaine[buf.nb_to]='\0';
                ecriredir(ft,buf.num_bloc,buf);
                sprintf(buf.chaine,"",buf.chaine);
                buf.nb_to=0;
                buf.num_bloc = alloc_bloc(ft);
                sprintf(buf.chaine,"%s%s",buf.chaine,tab[l].str);
                buf.nb_to += taille;
                buf.der_tai=taille;
            }
        }
    }
    aff_entete(ft,3,0t);
    aff_entete(ft,2,m);//nbre d'enreg
    aff_entete(ft,1,buf.num_bloc);
    buf.chaine[buf.nb_to]='\0';
    ecriredir(ft,buf.num_bloc,buf);
    /***********************************************************************/
    /***************Retrier le tableau selon le num du bloc :***************/
    qsort(tab,c,sizeof(tabchaine),comp);
    /***********************************************************************/
    /***************Supprimer les doublons du fichier seq et sa copie :*****/
    m=tab[0].blc;//m: contient le numero du bloc
    liredir(fs,m,&buf);
    liredir(fsCP,m,&bufCP);
    for(p=0;p<c;p++)
    {
        if(tab[p].blc != m)
        {
            ecriredir(fs,m,buf);
            ecriredir(fsCP,m,bufCP);
            m = tab[p].blc;
            liredir(fs,m,&buf);
            liredir(fsCP,m,&bufCP);
        }
        buf.chaine[tab[p].pos + 2] = tab[p].str[2];
    }
    ecriredir(fs,m,buf);
    ecriredir(fsCP,m,bufCP);

    /***********************************************************************/
    /*****************Fermeture des fichiers********************************/
    fclose(f);//fermer le fichier Index
    fermer(fs);//fermer le fichier sequentiel
    fermer(fsCP);//fermer la copie du fichier seq
    fermer(ft);//fermer le fichier trie
}



void affich_tab(tabchaine t[],int k)//k: nombre de case à afficher
{
    int m=0,n=0;
    printf("\nTableau :\n");
    for(m=0;m<k;m++)
    {
       for(n=0;t[m].str[n]!='\0';n++)
       {
            printf("%c",t[m].str[n]);
       }
       printf(" %d",t[m].blc);
       printf(" %d",t[m].pos);
       printf("\n");
    }
}

int fct_comp(const void *objet1,const void *objet2)
{
    int p=3;
    char code1[7]="",code2[7]="";
    tabchaine *e1=(tabchaine *) objet1;
    tabchaine *e2=(tabchaine *) objet2;

    int elm1,elm2;
    for(p=3;p<9;p++)
    {
        code1[p-3]=e1->str[p];
        code2[p-3]=e2->str[p];
    }
    code1[6]='\0';
    code2[6]='\0';
    elm1=atoi(code1);
    elm2=atoi(code2);
    if(elm1 > elm2) return 1;
    else if(elm1 == elm2) return 0;
    return -1;
}

int comp_ch(tabchaine t[],int i,int j)//comparer entre les chaines des cases 'k' et 'l'
{
    int p=3;
    char code1[7]="",code2[7]="";

    for(p=3;p<9;p++)
    {
        code1[p-3]=t[i].str[p];
        code2[p-3]=t[j].str[p];
    }
    code1[6]='\0';
    code2[6]='\0';
    return strcmp(code1,code2);
}

int comp(const void *objet1,const void *objet2)
{
    int elm1,elm2;
    char code1[7]="",code2[7]="";

    tabchaine *e1=(tabchaine *) objet1;
    tabchaine *e2=(tabchaine *) objet2;
    elm1 = e1->blc;
    elm2 = e2->blc;
    if(elm1 > elm2) return 1;
    else if(elm1 == elm2) return 0;
    return -1;
}
    /***********************************************************************/
    /**********************Mise A Jour :************************************/
void miseA_Jour(int nb_sup,int nb_mod,int nb_Ajout)
{
    int c=0,m=0,i=0,taille=0,l=0,bloc=0,fin=0,bInf=1,bSup=0,stop=0,cmp1=0,cmp2=0,cmp=0,taiA=0,a=0,j=0,trouv=0;
    char taich[3]="",codeAj[7]="",code[7]="",chaine[70]="",codeSup[7]="",codeInf[7]="",codeEx[7]="",ch[70]="";
    /**********************Dans le fichier Seq et sa copie :****************/
    /***********************************************************************/
    /**********************Suppression Et Modification :********************/
    fs = ouvrir("fseq",'A');
    ft = ouvrir("ftrie",'A');
    fsCP=ouvrir("fseqCP",'A');//ouvrir la copie du fichier sequentiel
    if((nb_sup+nb_mod) > (entete(fs,2)-entete(fs,3)))//entete(f,2):nbre d'enreg existant dans le fichier seq
    {
        printf("\n| Nombre d'enregistrement insuffisant pour faire la Mise A Jour |\n");
    }
    else
    {
//        printf(" AAA ");
        bloc=alea(1,entete(fs,1));liredir(fs,bloc,&buf);//printf(" BLOC=%d ",bloc);
        while(c < (nb_sup+nb_mod))
        {
//            printf(" 111 ");
            if(m==buf.nb_to)
            {
                bloc++;
                if(bloc > entete(fs,1)){bloc=1;}
                liredir(fs,bloc,&buf);m=0;nb_lirS++;
            }
            taich[0]=buf.chaine[m];taich[1]=buf.chaine[m+1];taich[2]='\0';taille=atoi(taich);
            if(buf.chaine[m+2]=='0')
            {
                tab[c].pos = m;tabIndex[c].pos = m;
                tab[c].blc=bloc;tabIndex[c].blc=bloc;
                for(l=0;l<taille;l++)
                {
                    tab[c].str[l]=buf.chaine[m];
                    tabIndex[c].str[l]=buf.chaine[m];
                    m++;
                }
                tab[c].str[taille]='\0';tabIndex[c].str[taille]='\0';
                c++;
            }
            else{m+=taille;}
        }
//        printf("\nTableau des enreg a mettre a jour :");
//        affich_tab(tab,c);
        for(l=0;l<nb_sup;l++){tab[l].str[2]='1';tabIndex[l].str[2]=tab[l].str[2];}/*********Suppression :***************/
        for(l=nb_sup;l<(nb_mod+nb_sup);l++)/*********Modificaion :***************/
        {
            tab[l].str[9] = (tab[l].str[9]+5)%123;
            if(tab[l].str[9]<97){tab[l].str[9]+=97;}
            tabIndex[l].str[9]=tab[l].str[9];
        }
    aff_entete(fs,3,entete(fs,3)+nb_sup);
    aff_entete(fsCP,3,entete(fsCP,3)+nb_sup);
    aff_entete(ft,3,entete(ft,3)+nb_sup);

//    affich_tab(tab,c);
    /***********************************************************************/
    /***************Retrier le tableau selon le num du bloc :***************/
    qsort(tab,c,sizeof(tabchaine),comp);
    /***********************************************************************/
    /************Remplir le fichier seq et sa copie  :**********************/
    m=tab[0].blc;//m: contient le numero du bloc
    liredir(fs,m,&buf);nb_lirS++;
    liredir(fsCP,m,&bufCP);nb_lirI++;
    for(l=0;l<c;l++)
    {
        if(tab[l].blc != m)
        {
            ecriredir(fs,m,buf);nb_ecrS++;
            ecriredir(fsCP,m,bufCP);nb_ecrI++;
            m = tab[l].blc;
            liredir(fs,m,&buf);nb_lirS++;
            liredir(fsCP,m,&bufCP);nb_lirI++;
        }
//        printf("\n l=%d pos=%d cB=%c cT=%c ",l,tab[l].pos,buf.chaine[tab[l].pos + 2],tab[l].str[2]);
        bufCP.chaine[tab[l].pos + 2] = tab[l].str[2];buf.chaine[tab[l].pos + 2] = tab[l].str[2];
        bufCP.chaine[tab[l].pos + 9] = tab[l].str[9];buf.chaine[tab[l].pos + 9] = tab[l].str[9];
//        printf(" l=%d pos=%d cB=%c cT=%c ",l,tab[l].pos,buf.chaine[tab[l].pos + 2],tab[l].str[2]);
    }
    ecriredir(fsCP,m,bufCP);nb_ecrI++;
    ecriredir(fs,m,buf);nb_ecrS++;
    /**************************************************************************************************************************/
    /************************Remplir le fichier trie :**********************/
    qsort(tab,c,sizeof(tabchaine),fct_comp);//trier le tab selon le code
    /***********************************************************************/
//    affich_tab(tab,c);
    /**Recherche du premier code :**/
        for(l=0;l<6;l++){codeAj[l]=tab[0].str[l+3];}codeAj[7]='\0';//code de la 1ere case
//        printf(" codeAj=%s ",codeAj);
        bInf=1;bSup=entete(ft,1);//contient l'adresse du dernier bloc
        trouv=0;
        while((bInf<=bSup) && (trouv==0))
        {
            i=(bInf+bSup)/2;
            liredir(ft,i,&buf);nb_lirT++;
            for(l=0;l<6;l++){codeInf[l]=buf.chaine[l+3];}codeInf[7]='\0';//printf("\ncodeA=%s ",codeAj);printf("codeInf=%s ",codeInf);
            for(l=buf.nb_to-buf.der_tai+3;l<buf.nb_to-buf.der_tai+9;l++){codeSup[l-buf.nb_to+buf.der_tai-3]=buf.chaine[l];}codeSup[7]='\0';//printf("codeSup=%s ",codeSup);
//            getche();printf(" 222 ");
            if((atoi(codeAj)>=atoi(codeInf)) && (atoi(codeAj)<=atoi(codeSup)))/****Recherche Ineterne :******/
            {
                taich[0]=buf.chaine[0];taich[1]=buf.chaine[1];taich[2]='\0';taille=atoi(taich);//taille du premier code
                strcpy(codeEx,codeInf);a=0;trouv=0;
//                printf("\nbuf=%s ",buf.chaine);
                while(trouv==0)
                {
//                    getche();printf(" 444 ");printf("\ncodeA=%s ",codeAj);printf("\ncodeE=%s ",codeEx);
                    if(atoi(codeAj)==atoi(codeEx))
                    {
                        trouv=1;
//                        getche();printf(" 555 ");
//                        printf(" \na=%d \n",a);
                        buf.chaine[a+9] = tab[nb_sup].str[9];
                    }//codeA=codeE
                    else/*codeA!=codeE*//**Ramener le prochain code***/
                    {
//                        getche();printf(" 666 ");
                        a+=taille;
                        taich[0]=buf.chaine[a];taich[1]=buf.chaine[a+1];taich[2]='\0';taille=atoi(taich);//taille du prochsin code
                        for(l=a+3;l<a+9;l++){codeEx[l-a-3]=buf.chaine[l];}codeEx[7]='\0';
                    }
                }
            }
            else
            {
                if(atoi(codeAj)<atoi(codeInf)){bSup=i-1;/*printf(" 111 ");*/}
                else if(atoi(codeAj)>atoi(codeSup)){bInf=i+1;/*printf(" 222 ");*/}
            }
        }
        a+=taille;l=nb_sup+1;//printf("\nbuf=%s\nnb_to=%d c=%d\n",buf.chaine,buf.nb_to,c);
    while(l<c)
    {
//        getche();printf(" CCC ");
        for(m=0;m<6;m++){codeAj[m]=tab[l].str[m+3];}codeAj[7]='\0';//le code à mettre à jour
        trouv=0;
        if(atoi(codeAj)<=atoi(codeSup) && a<buf.nb_to && trouv==0)/****Recherche Ineterne :******/
        {
//            printf(" 456 ");
            while(trouv==0 && a<buf.nb_to)
            {
                taich[0]=buf.chaine[a];taich[1]=buf.chaine[a+1];taich[2]='\0';taille=atoi(taich);
                for(m=a+3;m<a+9;m++){codeEx[m-a-3]=buf.chaine[m];}codeEx[7]='\0';
//                printf(" l=%d ",l);
//                getche();printf(" codeE=%s codeA=%s ",codeEx,codeAj);
                if(atoi(codeAj)==atoi(codeEx))
                {
//                    printf(" 777 ");
                    buf.chaine[a+2] = tab[l].str[2];
                    buf.chaine[a+9] = tab[l].str[9];
                    l++;trouv=1;
                }
                a+=taille;
            }
        }
        else
        {
            ecriredir(ft,i,buf);nb_ecrT++;
            i++;if(i>entete(ft,1)){i=1;}
            liredir(ft,i,&buf);a=0;
            for(m=buf.nb_to-buf.der_tai+3;m<buf.nb_to-buf.der_tai+9;m++){codeSup[m-buf.nb_to+buf.der_tai-3]=buf.chaine[m];}codeSup[7]='\0';//codeSup du bloc 'i'
        }
    }
    ecriredir(ft,i,buf);nb_ecrT++;
    }
    /**************************************************************************************************************************/
/***********************************************************************************************************************************************/
    /************************Ajout :****************************************/
    /***************Dans le fichier Seq :***********************************/
    int ajout=0;
    c=0;

//    affich_tab(tabIndex,nbreLigne);
    while(ajout < nb_Ajout)
    {
//        printf(" ajout=%d ",ajout);
//        printf(" BBB ");
        liredir(fs,1,&buf);nb_lirS++;
        alea_enre(chaine);//printf("\nchaine=%s\n",chaine);
        strcpy(ch,chaine);
        for(l=3;l<9;l++){codeAj[l-3]=chaine[l];}codeAj[7]='\0';
        fin=0;m=0;i=1;
        while(fin == 0)
        {
//            printf(" AAA ");
            taich[0]=buf.chaine[m];taich[1]=buf.chaine[m+1];taich[2]='\0';taille=atoi(taich);
            for(l=m+3;l<m+9;l++){code[l-m-3]=buf.chaine[l];}code[7]='\0';//extraire le code
            if(atoi(code)==atoi(codeAj))
            {
//                printf(" BBB ");
                if(buf.chaine[m+2]=='1'){buf.chaine[m+2]='0';ecriredir(fs,i,buf);ajout++;}/************************2-Insertion :**********************/
                fin=1;
            }
            else
            {
//                printf(" CCC ");
                m+=taille;/*printf(" taille=%d ",taille);*/
                if(m==buf.nb_to)//fin du bloc
                {
//                    printf(" DDD ");
                    i++;
                    if(i>entete(fs,1))
                    {
//                        printf(" EEE ");
                        if(strlen(chaine) > (Taille_Bloc-buf.nb_to))
                        {
//                            printf(" FFF ");
                            buf.num_bloc = alloc_bloc(fs);
                            aff_entete(fs,1,buf.num_bloc);
                            buf.nb_to=0;
                            strcpy(buf.chaine,"");
//                            printf(" GGG ");
                        }
                            sprintf(buf.chaine,"%s%s",buf.chaine,chaine);/************************2-Insertion :**********************************/
                            buf.nb_to += strlen(chaine);
//                            printf(" HHH ");
                            buf.chaine[buf.nb_to]='\0';
                            ecriredir(fs,entete(fs,1),buf);nb_ecrS++;
//                            printf(" III ");
                            ajout++;
                            fin=1;i=1;
                    }
                    liredir(fs,i,&buf);m=0;nb_lirS++;
                }
            }
        }
//    printf(" JJJ ");
    /**********************Dans le fichier trie :***************************/
    /***********************************************************************/
    /************************Ajout :****************************************/
        m=0;
        strcpy(tab[0].str,chaine);
        for(l=3;l<9;l++){codeAj[l-3]=chaine[l];}codeAj[7]='\0';
        bInf=1;
        bSup=entete(ft,1);//contient l<adresse du dernier bloc
        stop=0;trouv=0;
        while((bInf<=bSup) && (stop==0) && (trouv==0))
        {
            i=(bInf+bSup)/2;
            liredir(ft,i,&buf);nb_lirS++;
            for(l=3;l<9;l++){codeInf[l-3]=buf.chaine[l];}codeInf[7]='\0';/*printf("\ncodeA=%s ",codeAj);*///strcpy(chaine,codeInf);//printf("codeInf=%s ",codeInf);
            for(l=buf.nb_to-buf.der_tai+3;l<buf.nb_to-buf.der_tai+9;l++){codeSup[l-buf.nb_to+buf.der_tai-3]=buf.chaine[l];}codeSup[7]='\0';//printf("codeSup=%s ",codeSup);
            if((atoi(codeAj)>=atoi(codeInf)) && (atoi(codeAj)<=atoi(codeSup)))/****Recherche Ineterne :******/
            {
//                printf(" codeI=%s",codeInf);
                a=0;stop=0;trouv=0;
                strcpy(codeEx,codeInf);
                while((trouv==0) && (stop==0))
                {
                    if(atoi(codeAj)==atoi(codeEx))//codeA=codeE
                    {
                        trouv=1;
                    }
                    else//codeA!=codeE
                    {
                        if(atoi(codeAj)<atoi(codeEx)){stop=1;/*printf(" 333 ");*/}/*codeA<codeE*/
                        else/****Ramener le prochain code :****/
                        {
                            taich[0]=buf.chaine[a];taich[1]=buf.chaine[a+1];taich[2]='\0';taille=atoi(taich);a+=taille;
                            for(l=a+3;l<a+9;l++){codeEx[l-a-3]=buf.chaine[l];}codeEx[7]='\0';
                        }
                    }
                }
            }
            else
            {
                if(atoi(codeAj)<atoi(codeInf)){bSup=i-1;/*printf(" 111 ");*/}
                else if(atoi(codeAj)>atoi(codeSup)){bInf=i+1;/*printf(" 222 ");*/}
            }
        }
//        printf("\nTab=%s \n",tab[0].str);
        if(trouv==0)/*******Decalage :********/
        {
            if(bInf>bSup){i=bSup;if(i==0){i=1;}}//codeA entre 2 blocs
            j=i;
            /******************Remplir le tableau  :*****************************************/
            c=1;
            while(i<=entete(ft,1))//Le buffer dans le tableau
            {
                liredir(ft,i,&buf);nb_lirT++;
                m=0;
                while(m<buf.nb_to)
                {
                    taich[0]=buf.chaine[m];taich[1]=buf.chaine[m+1];taich[2]='\0';taille=atoi(taich);
                    for(l=m;l<m+taille;l++){tab[c].str[l-m]=buf.chaine[l];}tab[c].str[taille]='\0';
                    m+=taille;c++;
                }
                i++;
            }
            /******************Trier le tableau selon le code :*********************************/
            qsort(tab,c,sizeof(tabchaine),fct_comp);
            /******************Remplir le fichier trie :****************************************/
//            affich_tab(tab,c);
            buf.nb_to=0;
            sprintf(buf.chaine,"",buf.chaine);
            for(l=0;l<c;l++)
            {
                if((Taille_Bloc-buf.nb_to)>=strlen(tab[l].str))
                {sprintf(buf.chaine,"%s%s",buf.chaine,tab[l].str);buf.nb_to+=strlen(tab[l].str);buf.der_tai=strlen(tab[l].str);}
                else
                {
                    buf.chaine[buf.nb_to]='\0';buf.num_bloc=j;
//                    printf("\n    BLOC NUM=%d\n",buf.num_bloc);
                    ecriredir(ft,j,buf);nb_ecrT++;
                    buf.nb_to=0;
                    sprintf(buf.chaine,"",buf.chaine);
                    sprintf(buf.chaine,"%s%s",buf.chaine,tab[l].str);
                    buf.nb_to+=strlen(tab[l].str);buf.der_tai=strlen(tab[l].str);
                    j++;
                    if(j>entete(ft,1))
                    {
//                        printf("\nABCDEFGHIJKLMNOP\n");
                        alloc_bloc(ft);
                        aff_entete(ft,1,j);
                    }
                }
            }
//            printf("\n    BLOC NUM=%d\n",buf.num_bloc);
            buf.num_bloc=j;
            buf.chaine[buf.nb_to]='\0';ecriredir(ft,j,buf);nb_ecrT++;
        }
    /**********************Dans le fichier Index:***************************/
    /***********************************************************************/
    /************************Ajout :****************************************/
    //affich_tab(tabIndex,nbreLigne);
        bInf=1;bSup=nbreLigne-1;trouv=0;//printf(" nbreL=%d ",nbreLigne);
        for(m=0;m<6;m++){codeAj[m]=ch[m+3];}codeAj[7]='\0';
        while(bInf<=bSup && trouv==0)
        {
//            printf(" 111 ");
            i=(bInf+bSup)/2;
            for(m=0;m<6;m++){codeEx[m]=tabIndex[i].str[m+3];}codeEx[7]='\0';
            if(atoi(codeAj)==atoi(codeEx))
            {
                if(tabIndex[i].str[2]=='1')
                {
                    while(comp_ch(tabIndex,i,i-1)==0){i--;}
                    if(tabIndex[i].str[2]=='1')
                    {
                        tabIndex[i].str[2]='0';
                        liredir(fsCP,tabIndex[i].blc,&bufCP);
                        bufCP.chaine[tabIndex[i].pos+2]='0';
                        ecriredir(fsCP,tabIndex[i].blc,bufCP);nb_ecrI++;
                    }

                }
                trouv=1;
            }
            else if(atoi(codeAj)<atoi(codeEx)){bSup=i-1;}
            else if(atoi(codeAj)>atoi(codeEx)){bInf=i+1;}
        }
        if(trouv==0)
        {
//            printf(" 222 ");
            strcpy(tabIndex[nbreLigne].str,ch);nbreLigne++;
            liredir(fsCP,entete(fsCP,1),&bufCP);nb_lirI++;//printf("\nbuf=%s\nentete=%d num_bloc=%d ",bufCP.chaine,entete(fsCP,1),bufCP.num_bloc);
            if(strlen(ch)>(Taille_Bloc-bufCP.nb_to)){/*printf(" 123 ");*/bufCP.num_bloc= alloc_bloc(fsCP);bufCP.nb_to=0;strcpy(bufCP.chaine,"");}/*printf(" num_bloc=%d ",buf.num_bloc);*/
            sprintf(bufCP.chaine,"%s%s",bufCP.chaine,ch);//inserer le code dans le dernier bloc du fichier
            bufCP.nb_to+=strlen(ch);bufCP.chaine[bufCP.nb_to]='\0';//printf("\nch=%s\n",ch);printf("\nbufAp=%s\nenteteA=%d num_blocA=%d ",bufCP.chaine,entete(fsCP,1),bufCP.num_bloc);
            ecriredir(fsCP,entete(fsCP,1),bufCP);nb_ecrI++;
            qsort(tabIndex,nbreLigne,sizeof(tabchaine),fct_comp);//trier le tableau selon le code
        }
//        printf(" 333 ");
    }
//    affich_tab(tabIndex,nbreLigne);
    aff_entete(ft,2,entete(ft,2)+ajout);
    aff_entete(fs,2,entete(fs,2)+ajout);
    aff_entete(fsCP,2,entete(fsCP,2)+ajout);



    printf("\n\nNombre de \"liredir\" dans le fichier seq est : %d\n",nb_lirS);
    printf("Nombre de \"liredir\" dans le fichier trie est : %d\n",nb_lirT);
    printf("Nombre de \"liredir\" dans le fichier Index est : %d\n",nb_lirI);
    printf("Nombre de \"ecriredir\" dans le fichier seq est : %d\n",nb_ecrS);
    printf("Nombre de \"ecriredir\" dans le fichier trie est : %d\n",nb_ecrT);
    printf("Nombre de \"ecriredir\" dans le fichier Index est : %d\n",nb_ecrI);
    fermer(fsCP);
    fermer(ft);
    fermer(fs);
}
