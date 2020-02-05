//ERHAN ÖZDOĞAN(160201039)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <time.h>
//TXT DOSYASINDAKI BILGILERIN TUTULDUGU STRUCT YAPISI
struct sehir_bilgileri{
    int komsu_sayisi;
    double lat;
    double longt;
    int plaka;
    int rakim;
    int komsu_sehirler[10];
    char komsular[100];
};


clock_t baslangic,son;
char Guzergah[82][82];
//FONKSIYON PROTOTIPLERI
double mesafe_hesapla(struct sehir_bilgileri sehirler[],int ilk,int son);
void Graf_maliyet_bulma(struct sehir_bilgileri sehirler[],double graf[][82],int kisi_sayisi,int baslangic_sehri);
void En_kisa_yol(struct sehir_bilgileri sehirler[],double graf[][82],int baslangic_sehir,double maliyetler[]);
void en_cok_kar_problemi(struct sehir_bilgileri sehirler[],double maliyetler[],int baslangic_sehir,int son_sehir,double graf[][82]);

int main(){


double zaman,zaman1;
restart:
baslangic=clock();
    int i,j,baslangic_sehri,son_sehir;
    double graf[82][82];
    struct sehir_bilgileri sehirler[82];
    double maliyetler[82];
    Lat_Long_oku(sehirler);
    Komsulari_ayir(sehirler);
    son=clock();
    zaman1=son-baslangic;
    /*KOCAELİ İSTANBUL ARASI KONRTOL

    double mesafe=mesafe_hesapla(sehirler,18,78);
    printf("%.14lf\n",mesafe_hesapla(sehirler,18,78));
    int dikey_mesafe=fabs(sehirler[78].rakim-sehirler[18].rakim);
    printf("%.14lf\n",fabs(sehirler[78].rakim-sehirler[18].rakim));
    double aci=atan(dikey_mesafe/mesafe)*180.0/3.14159265359;
    printf("%.14lf\n",aci);
    double  net_mesafe=sqrt(pow(mesafe,2)+pow((dikey_mesafe/1000.0),2));
    printf("%.14lf",net_mesafe);*/





   while(1){
        printf("Talebinizi seciniz:\n");
        printf("1-) Sabit ucret 20 TL olarak alindiginda kac yolcu ile sefer duzenlenirse maksimum kar elde edebilirim?\n");
        printf("2-) Bir yolcudan kac TL alinirsa yuzde 50 kara ulasabilirim?\n\n");
        scanf("%d",&i);

        if(i==1 || i==2)
            break;
        else
            printf("\nYanlis secim!\n");

    }



    printf("Baslangic Sehri Giriniz (1-81): ");
    scanf("%d",&baslangic_sehri);
    printf("Hedef Sehri Giriniz (1-81): ");
    scanf("%d",&son_sehir);



baslangic=clock();
    if(i==1)
        en_cok_kar_problemi(sehirler,maliyetler,baslangic_sehri,son_sehir,graf);

    else if(i==2){
        kar_50_problemi(sehirler,maliyetler,baslangic_sehri,son_sehir,graf);
    }
son=clock();

zaman=son-baslangic;

printf("\nGecen Zaman:%lf Milisaniye\n",zaman+zaman1);
int restart;
while(restart!=-1 || restart!=1){
printf("Cikmak icin -1 Yeniden Hesaplamak İcin 1: \t");
scanf("%d",&restart);
if(restart==-1)
    break;
else if(restart == 1)
    goto restart;
}
    return 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------LAT,LONG,PLAKA,RAKIM,KOMSULUK DEGERLERINI DOSYADAN OKUR VE DIZIYE ATAR-------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void Lat_Long_oku( struct sehir_bilgileri sehirler[]){
FILE*dosya=fopen("lat long.txt","r");
char virgul1,virgul2,virgul3;
int i;
for(i=1;i<82;i++){
fscanf(dosya,"%lf %c %lf %c %d %c %d",&sehirler[i].lat,&virgul1,&sehirler[i].longt,&virgul2,&sehirler[i].plaka,&virgul3,&sehirler[i].rakim);
fgets(sehirler[i].komsular,100,dosya);
}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------KOMSU SEHIRLERI STRINGDEN OKUR VE DIZIYE ATAR -------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void Komsulari_ayir(struct sehir_bilgileri sehirler[]){
int i,j;
char b1,b2,b3,b4,b5,b6,b7,b8,b9;
for(i=1;i<82;i++){
sscanf(sehirler[i].komsular,"%d",&sehirler[i].komsu_sayisi);
sscanf(sehirler[i].komsular,"%d%c%d%c%d%c%d%c%d%c%d%c%d%c%d%c%d%c%d",&sehirler[i].komsu_sehirler[0],&b1,&sehirler[i].komsu_sehirler[1],&b2,&sehirler[i].komsu_sehirler[2],&b3,&sehirler[i].komsu_sehirler[3],&b4,&sehirler[i].komsu_sehirler[4],&b5,&sehirler[i].komsu_sehirler[5],&b6,&sehirler[i].komsu_sehirler[6],&b7,&sehirler[i].komsu_sehirler[7],&b8,&sehirler[i].komsu_sehirler[8],&b9,&sehirler[i].komsu_sehirler[9]);
for(j=sehirler[i].komsu_sayisi+1;j<10;j++)
    sehirler[i].komsu_sehirler[j]=0;
}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------GRAFI OLUSTURUR MATRISTE TUTAR---------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------

void Graf_olustur(double graf[][82],struct sehir_bilgileri sehirler[]){
int i,j,k;
for(i=1;i<82;i++){
    for(j=1;j<82;j++){
            //DEFAULT OLARAK -1 DIR
            graf[i][j]=-1;
            for(k=1;k<=sehirler[i].komsu_sayisi;k++){
                    //KOMSU SEHIRLERI 1 ILE ISARETLER
        if(sehirler[i].komsu_sehirler[k]==j)
            graf[i][j]=1;
        }

    }

    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------LAT LONG ILE MESAFE HESAPLAMA (HAVERSINE FORMULU)  ---------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------

double mesafe_hesapla(struct sehir_bilgileri sehirler[],int ilk,int son){
int R=6371;
double dlat=(3.14159265359/180)*(sehirler[son].lat-sehirler[ilk].lat);
double dlon=(3.14159265359/180)*(sehirler[son].longt-sehirler[ilk].longt);
double a=(pow(sin(dlat/2),2)+cos((3.14159265359/180)*sehirler[ilk].lat)*cos((3.14159265359/180)*sehirler[son].lat)*pow(sin(dlon/2),2));
double c=2*atan2l(sqrt(a),sqrt(1-a));
return R*c;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------GRAFI DOSYAYA YAZDIRMA ---------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------

void grafi_dosyaya_yaz(double graf[][82]){
int i,j;
FILE *graf_dosyasi;
    graf_dosyasi=fopen("                    Graf.txt","w+");
    fprintf(graf_dosyasi,"    ");
    for(j=1;j<82;j++){
        fprintf(graf_dosyasi,"%10.2d ",j);
    }
    fprintf(graf_dosyasi,"\n");
    for(i=1;i<82;i++){
            fprintf(graf_dosyasi,"%02d-) ",i);
        for(j=1;j<82;j++){
         fprintf(graf_dosyasi,"%10.5lf ",graf[i][j]);
        }
        fprintf(graf_dosyasi,"\n");
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------- GRAFIN MALIYETINI BULMA ---------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------

void Graf_maliyet_bulma(struct sehir_bilgileri sehirler[],double graf[][82],int kisi_sayisi,int baslangic_sehri){
int i,j;
double mesafe,dikey_mesafe,net_mesafe,aci;
for(i=1;i<82;i++){
    for(j=1;j<82;j++){
        if(graf[i][j]==1 && i!=j){
                //YATAY MESAFEYI HESAPLAR
                mesafe=mesafe_hesapla(sehirler,i,j);
        //BASLAGIC SEHRI VE KOMSU SEHRI ARASINDAKI NET MESAFE HESAPLANACAKSA +50 EKLENIR DEGILSE EKLENMEZ
                if(sehirler[i].plaka!=baslangic_sehri)
                    dikey_mesafe=fabs(sehirler[j].rakim-sehirler[i].rakim);
                else
                dikey_mesafe=fabs(sehirler[j].rakim-sehirler[i].rakim+50);

                aci=atan(dikey_mesafe/mesafe)*180.0/3.14159265359;
                net_mesafe=sqrt(pow(mesafe,2)+pow((dikey_mesafe/1000.0),2));
                //EGER CIKAN ACI 80-KISI SAYISINDAN BUYUKSE O SEHRE GIDILEMEZ VE -1 ATANIR
                if(aci<=80-kisi_sayisi)
                    graf[i][j]=(net_mesafe/100)*1000;
                else
                    graf[i][j]=-1;


    }
}
}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------- EN KISA YOL ALGORITMASI --------- ---------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------

void En_kisa_yol(struct sehir_bilgileri sehirler[],double graf[][82],int baslangic_sehir,double maliyetler[]){

double sonsuz=999999999,enkucuk;
char *isaretci;
int i,j,l=1,k,m=1,t=1,anlik_dugum,isleme_alindi[82];
//İsleme baslarken guzergah sıfırlandı
for(i=0;i<82;i++){
        //GIDILEN SEHRE TEKRAR GITMEMEK ICIN GIDILEN SEHIRLERI DIZIDE TUTUYORUZ
isleme_alindi[i]=0;
  maliyetler[i]=sonsuz;
    for(j=0;j<82;j++){
        Guzergah[i][j]=NULL;
    }
}
//BASLANGIC SEHRI ILK ELE ALINAN DUGUM OLDU MALIYETI SIFIRLANDI VE TEKRAR GITMEMEK ICIN DIZIDE 1 ILE ISARETLENDI
maliyetler[baslangic_sehir]=0;
anlik_dugum=baslangic_sehir;
isleme_alindi[anlik_dugum]=1;

for(i=1;i<82;i++){
 for(j=1;j<82;j++){
// HEDEF SEHRE GIDILMEDIYSE (isleme_alindi[j]!=1), BASLANGIC SEHRI ILE HEDEF SEHIR ARASI BAGLANTI VARSA (graf[anlik_dugum][j]!=-1) VE YENI YOL ESKI YOLDAN
//DAHA AZ MASRAFLI ISE MALIYET DEGISTIRILIR.
    if(isleme_alindi[j]!=1 && graf[anlik_dugum][j]!=-1 && maliyetler[j]>graf[anlik_dugum][j]+maliyetler[anlik_dugum]){
                maliyetler[j]=graf[anlik_dugum][j]+maliyetler[anlik_dugum];
                //Rota kısmı
                strcpy(Guzergah[j],Guzergah[anlik_dugum]);
                isaretci=Guzergah[j];
                for(;*isaretci!=NULL;isaretci++){}
                *isaretci=anlik_dugum;
                }
    }
//GREEDY YAKLASIMI ILE SONRAKI EN KUCUK MALIYETLI DUGUMU BULMA
enkucuk=sonsuz;
for(k=1;k<82;k++){
 if(isleme_alindi[k]!=1 && maliyetler[k]<enkucuk){
    enkucuk=maliyetler[k];
    anlik_dugum=k;
  }
}
isleme_alindi[anlik_dugum]=1;
}
//GIDILEMEYEN SEHIRLER SONSUZ OLARAK KALDIGI ICIN MALIYETLERINE -1 ATANIR
for(i=0;i<82;i++){
    if(maliyetler[i]==999999999)
    maliyetler[i]=-1;
}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------- MALIYETLERI ve GUZERGAHI DOSYAYA YAZDIRMA ---------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void maliyetleri_dosyaya_yaz(double maliyetler[],int baslangic_sehir){
int i,j;
FILE *maliyet_dosyasi;
    maliyet_dosyasi=fopen("                   Guzergah ve Maliyetler.txt","w+");
    fprintf(maliyet_dosyasi,"\t\tBaslangic Sehir: %d\n\n",baslangic_sehir);
    for(i=1;i<82;i++){
            fprintf(maliyet_dosyasi,"%02d-) ",i);
            if(maliyetler[i]!=999999999)
         fprintf(maliyet_dosyasi,"%20.14lf",maliyetler[i]);
         else
            fprintf(maliyet_dosyasi,"-1");
         fprintf(maliyet_dosyasi,"      ");
         for(j=0;j<82;j++){
            if(Guzergah[i][j]!=NULL){
                fprintf(maliyet_dosyasi,"%d ",Guzergah[i][j]);
            }
         }
         fprintf(maliyet_dosyasi,"\n");

        }
        fclose(maliyet_dosyasi);
    }
//----------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------- %50 KAR PROBLEMI -----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void kar_50_problemi(struct sehir_bilgileri sehirler[],double maliyetler[],int baslangic_sehir,int son_sehir,double graf[][82]){
    int i,j;
    printf("\nBaslangic Sehri: %d\n",baslangic_sehir);
    printf("Hedef Sehir: %d\n\n",son_sehir);
    for(i=5;i<51;i=i+5){
            Graf_olustur(graf,sehirler);
            Graf_maliyet_bulma(sehirler,graf,i,baslangic_sehir);
            if(i==5)
            grafi_dosyaya_yaz(graf);
            En_kisa_yol(sehirler,graf,baslangic_sehir,maliyetler);
            if(i==5)
            maliyetleri_dosyaya_yaz(maliyetler,baslangic_sehir);
            printf("Kisi Sayisi:%d\n",i);
            if(maliyetler[son_sehir]==-1){
                printf("Sehre Gidilemiyor !!\n");
                continue;
            }
            printf("Maliyet: %.14lf\n",maliyetler[son_sehir]);
            printf("Guzergah: ");
                for(j=0;j<20;j++){
                        if(Guzergah[son_sehir][j]!=NULL)
                    printf("%d--",Guzergah[son_sehir][j]);
                }
                printf("%d",son_sehir);
            printf(" \n%50 Kar icin alinmasi gereken kisi basi ucret: %.14lf\n",(maliyetler[son_sehir]+maliyetler[son_sehir]*0.5)/i);
            printf("-------------------------------------------------------\n\n");
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------- SABIT UCRETLE EN COK KAR PROBLEMI -----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void en_cok_kar_problemi(struct sehir_bilgileri sehirler[],double maliyetler[],int baslangic_sehir,int son_sehir,double graf[][82]){
    int i,j, kisi_sayisi[46];
    double liste[46], temp, temp2;
    printf("\nBaslangic Sehri: %d\n",baslangic_sehir);
    printf("Hedef Sehir: %d\n\n",son_sehir);

    for(i=5; i<51; i++){

        Graf_olustur(graf,sehirler);
            Graf_maliyet_bulma(sehirler,graf,i,baslangic_sehir);
            if(i==5)
            grafi_dosyaya_yaz(graf);
            En_kisa_yol(sehirler,graf,baslangic_sehir,maliyetler);
            if(i==5)
            maliyetleri_dosyaya_yaz(maliyetler,baslangic_sehir);
            printf("\nKisi Sayisi:%d\n",i);

            liste[i-5]=i*100-maliyetler[son_sehir];
            kisi_sayisi[i-5]=i;

            if(maliyetler[son_sehir]==-1){
                liste[i-5]=-1;
                printf("Sehre Gidilemiyor !!\n");
                continue;
            }
            printf("Maliyet: %.14lf\n",maliyetler[son_sehir]);
            printf("Guzergah: ");
                for(j=0;j<20;j++){
                        if(Guzergah[son_sehir][j]!=NULL)
                    printf("%d--",Guzergah[son_sehir][j]);


                }
                printf("%d",son_sehir);



    }

    printf("\n----------------------------------------\n Maksimum kar siralamasi:");
/*
    for(i=0; i<46; i++){
        printf("\n%.2lf",liste[i]);
    }
*/

    for(i=0; i<46; i++){
        for(j=0; j<46; j++){
            if(liste[i]>liste[j]){
                temp=liste[i];
                temp2=kisi_sayisi[i];

                liste[i]=liste[j];
                kisi_sayisi[i]=kisi_sayisi[j];

                liste[j]=temp;
                kisi_sayisi[j]=temp2;
            }

        }
    }
    j=1;
    for(i=0; i<46; i++){
        if(liste[i]!=-1){
            printf("\n%d->  %d Kisi ile kar: %.14lf",j,kisi_sayisi[i],liste[i]);
            j++;
        }

    }
}





