#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define LJ "\033[35m"
#define ZL "\e[0;32m"
#define KR "\033[0m"
#define PL "\e[0;34m"

#define BP "\e[47m"
#define ZP "\e[42m"
#define DB "\e[1;90m"

typedef struct{
int adresa;
char rec[10];
struct FAT *sledeci;
}FAT;

typedef struct{
char nazivFajla[9];
char ekstenzija[4];
char vremeKreiranja[20];
int prviKlaster;
FAT *prvi;
}Root;

void fPocetakPrograma()
{
    int izbor;
    printf("%s%s*---*---*---* |FAT Sistem Datoteka Simulacija| *---*---*---* %s\n",LJ,BP,KR);
    printf("\t Odaberite opciju: \n");
    printf("1. Zapocnite program\n");
    printf("2. O programu \n");
    scanf("%d",&izbor);
    switch(izbor)
    {
    case 1:
        system("cls");
        return;
    case 2:
        system("cls");
        printf("%s%s *---*---*---* FAT Sistem Datoteka Simulacija *---*---*---* %s\n",LJ,BP,KR);
        printf("Trenutna simulacija je pojednostavljena sema rada FAT sistem datoteke. \n");
        printf("Bazira se na tome da korisnik unese podate o fajlu koji zeli da kreira\n");
        printf("sto predstavlja %sROOT odeljak%s, zatim korisnik unosi podatke u datom fajlu\n",DB,KR);
        printf("(fokusiracemo se na na txt) sto rekreira %sDATA odeljak%s, svaki fajl u pravom \n",DB,KR);
        printf("%sROOT odeljku%s sadrzi informaciju o prvom klasteru datog podatka, sto cemo mi iskorisiti\n",DB,KR);
        printf("kao pocetak jednostruko spregnute liste. Svaka lista sadrzi klaster i podatak o klasteru\n");
        printf("sto predstavlja %sFAT odeljak%s, i sa tim smo simuliraLi sve delove FAT Sistem datoteke\n",DB,KR);
        printf("\033[35m*---------------------------------------------------------------------------------*\033[0m \n");
    return;
    default:
        printf("Pogresna vrednost");
        break;
    }
}

void fNamesti(Root *fajl,int *x,int *klaster)
{
    char vreme[20];
    char putanja[20];
    time_t t=time(NULL);
    struct tm *tm_info = localtime(&t);
    int b=*x;  //Potrebno jer zbog nekog razloga *x nece svuda da radi

//Unos u Strukturu
    printf("Unesite ime datoteke:> ");
    scanf("%s",fajl[b].nazivFajla);
    printf("Unesite ime extenzije:> ");
    scanf("%s",fajl[b].ekstenzija);
    fajl[b].prviKlaster=*klaster;

//Odeljak za vreme
    int godina=tm_info->tm_year + 1900;
    int mesec=tm_info->tm_mon +1;
    int dan=tm_info->tm_mday;
    int sat=tm_info->tm_hour;
    int minut=tm_info->tm_min;

    sprintf(vreme,"%04d.%02d.%02d %02d:%02d",godina,mesec,dan,sat,minut);
    printf("\t %sTrenutno je: %s %s",LJ,vreme,KR);
    strcpy(fajl[b].vremeKreiranja,vreme);

//Odeljak za FILE
    sprintf(putanja,"%s.%s",fajl[b].nazivFajla,fajl[b].ekstenzija);
    FILE *f=fopen(putanja,"w");
    if (f==NULL)
    {
        printf("Greska.!");
        return;
    }

    printf("\n%s*----|Text Editor|-----|%s|-----|k:0x%d|--* %s\n",PL,putanja,*klaster,KR);
    char text[256];
    while (getchar() != '\n');
    fgets(text,256,stdin);

//Logika isecanja stringa//
    for(int i=0;i<strlen(text);i+=8)
    {
        fprintf(f,"%d ",(*klaster)++);
        for (int j=i;j<i+8 && j<strlen(text);j++)
        {
            fprintf(f,"%c",text[j]);
        }
        fprintf(f,"\n");
    }
    printf("%s*-----------------------------------------------------------------* %s\n",LJ,KR);
//Linked List//

    FAT *glava=(FAT*)malloc(sizeof(FAT));
    fajl[b].prvi=glava;
    glava->sledeci=NULL;
//Inkrementacije klastera i indeksa strukture, potrebni da se zna koliko cega ima
      b++;
     *x=b;
//Za mene da znam gde je kraj

    fclose(f);
    printf("Uspesno ispisan tekst u fajl: %s, sledeci slobodan klaster: 0x%d \n", putanja, *klaster);
    printf("%s*-----------------------------------------------------------------* %s \n",LJ,KR);\
}

void fPrikaz(Root *fajl,int x)
{
    printf("%s*----------------|File Explorer|----------------*%s: \n",PL,KR);
    printf("%s*-----*-----*-----*-----*-----*-----*-----*-----*%s\n",LJ,KR);
    //Stampanje svih struktura, a struktura=fajl koji smo stvorili//

    for(int i=0;i<x;i++)
    {
        printf("%d. > %s.%s\tFajl napravljen: %s \n",i+1,fajl[i].nazivFajla,fajl[i].ekstenzija,fajl[i].vremeKreiranja);
    }

    //odabir za dalje
    printf("%s*-----*-----*-----*-----*-----*-----*-----*-----*%s\n",LJ,KR);
}

void fIspis(Root *fajl)
{
//Pocetak
    int x;
    printf("Koji fajl zelit1e da ispisete? \n");
    scanf("%d",&x);
    int i=x-1;
//Liste pocetak
    FAT *trenutni;
    trenutni=(fajl[i].prvi);
    trenutni->sledeci=NULL;

//FILE i Putanja odeljak
    char put[20];
    sprintf(put,"%s.%s",fajl[i].nazivFajla,fajl[i].ekstenzija);

    FILE *f=fopen(put,"r");
    if (f==NULL)
    {
        printf("Greska u otvaranju fajla");
    }
/*  Moram Ostaviti da pratim tok kretanja strukture i liste
    printf("Vrednost je: %p \n",fajl[0].prvi);
    printf("Vrednost je: %p \n",trenutni);
    printf("Vrednost je: %p \n",trenutni->sledeci);
*/
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), f) != NULL)
    {
        sscanf(buffer, "%d %[^\n]", &trenutni->adresa, trenutni->rec);
        if (trenutni->sledeci == NULL)
        {
            printf("link: %d %s \n",trenutni->adresa,trenutni->rec);
            FAT *nov=(FAT*)malloc(sizeof(FAT));
            trenutni->sledeci=nov;
            trenutni=trenutni->sledeci;
            trenutni->sledeci=NULL;

        }  else {
            printf("U Elsu smo");
            trenutni=trenutni->sledeci;
            printf("Greska u alociranju na sledeci");
        }
    }
    char ispis[60];
    trenutni=(fajl[i].prvi);
    while(trenutni!=NULL)
    {
        strcat(ispis,trenutni->rec);
        trenutni=trenutni->sledeci;

    }
// Deo ispisa
    printf("%s*----------|File Viewer|-------------|f:%s|-----* %s \n",PL,put,KR);
    printf("%s \n",ispis);
    printf("%s*----------*----------*----------*----------*----------*%s \n",PL,KR);
    fclose(f);
}

void fObrisi(Root *fajl)
{
    int i,x;
    printf("Koji fajl zelite da obrisete: \n");
    scanf("%d",&i);
    x=i-1;

    const char *putanja;
    sprintf(putanja,"%s.%s",fajl[x].nazivFajla,fajl[x].ekstenzija);
    remove(putanja);

    FAT *trenutni=(fajl[x].prvi);
    FAT *sledeci;

    /* Pracenje memorijske adrese strukture i liste
    printf("memorijska adresa ll %p",trenutni);
    printf("memorijska adresa st %p",fajl[x].prvi);
    */

    while(trenutni!=NULL)
    {
        sledeci=trenutni->sledeci;
        free(trenutni);
        trenutni=sledeci;
    }

    memset(&fajl[x],0,sizeof(Root));
    printf("%s*---*-%s|Fajl |%s| je uspesno obrisan|%s%s-*----* %s \n",ZL,BP,putanja,KR,ZL,KR);
    printf("\n");
}

int main()
{
    /*Pocetne vrednosti samo jedno se citaju, preko pointera
    se kasnije samo izmenjuju jer moraju se uvek inkrementovati*/
    int x=0;
    int klaster=1001;
    int izbor;
    Root fajl[30];
    fPocetakPrograma();

    do
    {
        printf("%s%s*---*---*---* |FAT Sistem Datoteka Simulacija| *---*---*---* %s\n",LJ,BP,KR);
        printf("%sTrenutne vrednosti klaster: 0x%d X: %d %s\n",ZL,klaster,x,KR);
        printf("Da li zelite da: \n");
        printf("1> Namestite novi fajl \n");
        printf("2> Pogledate postojece fajlove \n");
        printf("3> Ispisite fajl \n");
        printf("4> Obrisite fajl \n");
        printf("5> Izlaz \n");

        scanf("%d",&izbor);
                switch(izbor)
                {
                case 1:
                    fNamesti(fajl,&x,&klaster);
                    break;
                case 2:
                    fPrikaz(fajl,x);
                    break;
                case 3:
                    fIspis(fajl);
                    break;
                case 4:
                    fObrisi(fajl);
                    break;
                case 5:
                    printf("%s%sIzlaz....%s",BP,LJ,KR);
                    break;
                case 6:
                    fOcistiEkran();
                    break;
                case 7:
                    fVidi(fajl,&klaster,&x);
                    break;
                case 8:
                    fVreme();
                    break;
                default:
                    printf("Pogresan izbor");
                    break;
                }
    }while(izbor !=5);
    return 0;

}

void fVidi(Root *fajl,int *klaster,int *x)
{
    printf("Memorijska adresa pokazivaca u strukturi: %p \n",fajl[0].prvi);
    printf("Vrednost klastera: %d \n",*klaster);
    printf("Vrednost indeksa: %d \n",*x);
   // printf("Vrednost je: %p",glava);
}

void fVreme()
{
    fOcistiEkran();
    time_t t=time(NULL);
    struct tm *tm_info = localtime(&t);
    int godina=tm_info->tm_year + 1900;
    int mesec=tm_info->tm_mon +1;
    int dan=tm_info->tm_mday;
    int sat=tm_info->tm_hour;
    int minut=tm_info->tm_min;
    char vreme[20];
    sprintf(vreme,"%04d.%02d.%02d %02d:%02d",godina,mesec,dan,sat,minut);
    printf("\t%sTrenutno je: %s %s \n",ZL,vreme,KR);

}

void fOcistiEkran()
{
    system("cls");
}
