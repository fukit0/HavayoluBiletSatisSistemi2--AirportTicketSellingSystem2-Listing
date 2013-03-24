#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define MAX 21

struct bilet{
    char TC[12];
    float fiyat;
    struct bilet *sonraki;
};

struct ucus{
    int ucusNo;
    char kalkisYeri[MAX];
    char varisYeri[MAX];
    int zaman;
    int kapasite;
    int bosKoltuk;
    float fiyat;
    struct ucus *kalkisYerineGoreSonraki;
    struct bilet *sonraki;
};

typedef struct bilet BILET;
typedef struct ucus UCUS;

void beklet(long int);
void baslikYazdir();
void yazdir(UCUS);
void biletYazdir(BILET *);
void biletListele(UCUS **);
void ucusGoruntule(UCUS *);
void kucukHarfeCevir(char *);
void kalkisYerineGoreListele(UCUS *);
void kalkisvarisYerineGoreListele(UCUS *);
void ucusVeBiletListele(UCUS **);
void dolulugaGoreListele(UCUS *);
void kalkisYerineGoreSiraliEkle(UCUS *,UCUS *);
void tcKimligeGoreSiraliEkle(BILET **,BILET *);
int ucusNoAl();
int menuSecim();
float biletFiyatHesapla(UCUS);
UCUS *dugumCikar(UCUS *,UCUS *);
BILET *tcKimligeGoreAraVeCikar(BILET **,char *);

int main()
{
    int i,secim,ucusNo,biletSay;
    float fiyat;
    char sorgu,zaman[5],TC[12];
    UCUS *hashUcusNo[9000]={NULL};
    UCUS hashListeBasi[26];
    UCUS *ucus,*cikarilan;
    BILET *bilet,*onceki,*gecici;

    system("title Havayolu Bilet Satis Sistemi");
    system("color 8f");

    printf("Program yukleniyor");
    for(i=0;i<3;i++)
    {
        beklet(150000000);
        printf(".");
    }
    printf("\n");

    for(i=0;i<26;i++)
        hashListeBasi[i].kalkisYerineGoreSonraki=&hashListeBasi[i];

    do{
        secim=menuSecim();
        switch(secim)
        {
            case 1:
                ucusNo=ucusNoAl();
                if(hashUcusNo[ucusNo]==NULL)
                {
                    ucus=malloc(sizeof(UCUS));
                    ucus->ucusNo=ucusNo;
                    printf("Kalkis yeri: ");
                    scanf("%s",ucus->kalkisYeri);
                    kucukHarfeCevir(ucus->kalkisYeri);
                    printf("Varis yeri: ");
                    scanf("%s",ucus->varisYeri);
                    kucukHarfeCevir(ucus->varisYeri);
                    printf("Kalkis zamani (ss:dd): ");
                    scanf("%s",zaman);
                    ucus->zaman=((zaman[0]-48)*1000+(zaman[1]-48)*100+(zaman[3]-48)*10+zaman[4]-48);
                    printf("Kapasite: ");
                    scanf("%d",&ucus->kapasite);
                    printf("Baslangic fiyati: ");
                    scanf("%f",&ucus->fiyat);
                    ucus->bosKoltuk=ucus->kapasite;
                    hashUcusNo[ucusNo]=ucus;
                    ucus->sonraki=NULL;
                    kalkisYerineGoreSiraliEkle(&hashListeBasi[toupper(ucus->kalkisYeri[0])-'A'],ucus);
                    printf("\tUcus eklendi\n");
                }
                else
                {
                    printf("\t%d numarali ucus zaten var!\n",ucusNo+1000);
                }
                break;
            case 2:
                ucusNo=ucusNoAl();
                if(hashUcusNo[ucusNo]==NULL)
                {
                    printf("\t%d numarali ucus yok!\n",ucusNo+1000);
                }
                else
                {
                        ucus=hashUcusNo[ucusNo];
                        printf("Yeni kalkis saati (ss:dd): ");
                        scanf("%s",zaman);
                        cikarilan=dugumCikar(&hashListeBasi[toupper(ucus->kalkisYeri[0])-'A'],ucus);
                        ucus->zaman=((zaman[0]-48)*1000+(zaman[1]-48)*100+(zaman[3]-48)*10+zaman[4]-48);
                        kalkisYerineGoreSiraliEkle(&hashListeBasi[toupper(ucus->kalkisYeri[0])-'A'],ucus);
                        printf("\tKalkis zamani guncellendi\n");
                }
                break;
            case 3:
                ucusNo=ucusNoAl();
                if(hashUcusNo[ucusNo]==NULL)
                {
                    printf("\t%d numarali ucus yok!\n",ucusNo+1000);
                }
                else
                {
                    ucus=hashUcusNo[ucusNo];
                    do{
                        printf("Satin almak istediginiz bilet sayisi: ");
                        scanf("%d",&biletSay);
                        if(biletSay>10)
                            printf("\tBir seferde en fazla 10 bilet satin alabilirsiniz!\n");
                    }while(biletSay>10);

                    if(ucus->bosKoltuk<biletSay)
                    {
                        printf("\tBu ucusta yeterli sayida bos yer yok!\n");
                    }
                    else
                    {
                        fiyat=biletFiyatHesapla(*ucus);
                        printf("Bir biletin fiyati: %.2f\n"
                                  "Toplam fiyat: %.2f\n",fiyat,biletSay*fiyat);
                        printf("\tOnayliyor musunuz? (e,h) ");
                        fflush(stdin);
                        scanf("%c",&sorgu);
                        if(sorgu=='e')
                        {
                            printf("\n   Yolcularin TC kimlik numaralarini giriniz...\n");
                            beklet(200000000);
                            for(i=0;i<biletSay;i++)
                            {
                                bilet=malloc(sizeof(BILET));
                                printf("%4d. yolcunun TC kimlik numarasi: ",i+1);
                                scanf("%s",bilet->TC);
                                bilet->fiyat=fiyat;
                                tcKimligeGoreSiraliEkle(&(ucus->sonraki),bilet);
                                ucus->bosKoltuk--;
                            }
                            printf("\tBilet satisi tamamlandi\n");
                        }
                    }
                }
                break;
            case 4:
                ucusNo=ucusNoAl();
                if(hashUcusNo[ucusNo]==NULL)
                {
                    printf("\t%d numarali ucus yok!\n",ucusNo+1000);
                }
                else
                {
                    ucus=hashUcusNo[ucusNo];
                    onceki=ucus->sonraki;
                    if(onceki!=NULL) //satilan bilet varsa
                    {
                        gecici=onceki->sonraki;
                        while(gecici!=NULL)
                        {
                            onceki=gecici;
                            gecici=gecici->sonraki;
                            free(onceki); //satilan biletler siliniyor
                        }
                        free(gecici);
                    }
                    ucus=dugumCikar(&hashListeBasi[toupper(ucus->kalkisYeri[0])-'A'],ucus);
                    free(ucus); //ucus dugumu siliniyor
                    hashUcusNo[ucusNo]=NULL;
                    printf("\tUcus iptal edildi\n");
                }
                break;
            case 5:
                ucusNo=ucusNoAl();
                if(hashUcusNo[ucusNo]==NULL)
                {
                    printf("\t%d numarali ucus yok!\n",ucusNo+1000);
                }
                else
                {
                    printf("TC kimlik numarasi: ");
                    scanf("%s",TC);
                    if((gecici=tcKimligeGoreAraVeCikar(&(ucus->sonraki),TC))==NULL)
                        printf("\tBu ucus icin bilet satin alinmamistir!\n");
                    else
                    {
                        free(gecici);
                        ucus->bosKoltuk++;
                        printf("\tBilet iptal edildi\n");
                    }
                }
                break;
            case 6:
                kalkisYerineGoreListele(hashListeBasi);
                break;
            case 7:
                kalkisvarisYerineGoreListele(hashListeBasi);
                break;
            case 8:
                ucusVeBiletListele(hashUcusNo);
                break;
            case 9:
                dolulugaGoreListele(hashListeBasi);
                break;
            case 10:
                biletListele(hashUcusNo);
                break;
            case 11:
                ucusGoruntule(hashListeBasi);
                break;
            case 12:
                printf("\nProgram kapatiliyor");
                for(i=0;i<3;i++)
                {
                    beklet(150000000);
                    printf(".");
                }
                printf("\n");
                break;
        }
    }while(secim!=12);

    system("pause");

    return 0;
}

void beklet(long int n)
{
    int i;

    for(i=0;i<n;i++) {}
}

void baslikYazdir()
{
    printf("\n"
           "Ucus No  Kalkis Yeri  Varis Yeri  Zaman  Kapasite  Bos Koltuk  Doluluk %%\n"
           "-------  -----------  ----------  -----  --------  ----------  ---------\n");
}

void yazdir(UCUS ucus)
{
    ucus.kalkisYeri[0]=toupper(ucus.kalkisYeri[0]);
    ucus.varisYeri[0]=toupper(ucus.varisYeri[0]);
    printf("%7d  %-11s  %-10s  %02d:%02d  %8d  %10d  %9d\n",
            ucus.ucusNo+1000,ucus.kalkisYeri,ucus.varisYeri,
            ucus.zaman/100,ucus.zaman%100,ucus.kapasite,ucus.bosKoltuk,
            100*(ucus.kapasite-ucus.bosKoltuk)/ucus.kapasite);
}

void biletYazdir(BILET *bas) //kuyrugun ilk dugumunun adresi call-by-value ile aktariliyor
{
    int sayac=0;
    float toplamFiyat=0;

    while(bas!=NULL)
    {
        printf("%11s  %7.2f\n",bas->TC,bas->fiyat);
        toplamFiyat+=bas->fiyat;
        bas=bas->sonraki; //bas 1 oteleniyor ama main'de degismiyor
        sayac++;
    }

    printf("\nToplam bilet sayisi: %d\n"
           "Toplam kazanc: %.2f\n"
           "-----------------------------\n"
           "Ortalama bilet fiyati: %.2f\n",sayac,toplamFiyat,(float)toplamFiyat/sayac);
}

void biletListele(UCUS **hashUcusNo)
{
    int i,sayac=0;
    char TC[12];
    UCUS *ucus;
    BILET *gecici;

    printf("\nTC kimlik no: ");
    scanf("%s",TC);

    for(i=0;i<9000;i++)
    {
        if((ucus=hashUcusNo[i])==NULL || ucus->sonraki==NULL)
            continue;
        else
        {
            gecici=ucus->sonraki;
            while(gecici!=NULL)
            {
                if(!strcmp(TC,gecici->TC))
                {
                    sayac++;
                    if(sayac==1)
                        printf("\n"
                               "Ucus No  Kalkis Yeri  Varis Yeri  Zaman    Fiyat\n"
                               "-------  -----------  ----------  -----  -------\n");

                    ucus->kalkisYeri[0]=toupper(ucus->kalkisYeri[0]);
                    ucus->varisYeri[0]=toupper(ucus->varisYeri[0]);
                    printf("%7d  %-11s  %-10s  %02d:%02d  %7.2f\n",
                            ucus->ucusNo+1000,ucus->kalkisYeri,ucus->varisYeri,
                            ucus->zaman/100,ucus->zaman%100,gecici->fiyat);
                }
                gecici=gecici->sonraki;
            }
        }
    }
    if(sayac==0)
        printf("\tBu yolcu bilet satin almamistir!\n");
}

void ucusGoruntule(UCUS *hash)
{
    int i;
    UCUS *gecici;

    baslikYazdir();
    for(i=0;i<26;i++)
    {
        gecici=hash[i].kalkisYerineGoreSonraki;
        while(gecici!=&hash[i])
        {
            yazdir(*gecici);
            gecici=gecici->kalkisYerineGoreSonraki;
        }
    }
}

void kucukHarfeCevir(char *str)
{
    int i;

    for(i=0;i<strlen(str);i++)
        str[i]=tolower(str[i]);
}

void kalkisYerineGoreListele(UCUS *hashListeBasi)
{
    char kalkisYeri[MAX];
    UCUS *listeBasi,*simdiki;

    printf("\nKalkis yeri: ");
    scanf("%s",kalkisYeri);
    kucukHarfeCevir(kalkisYeri);

    listeBasi=&hashListeBasi[toupper(kalkisYeri[0])-'A'];
    simdiki=listeBasi->kalkisYerineGoreSonraki;

    if(simdiki==listeBasi)
        printf("\t%s sehrinden kalkan bir ucus yok!\n",kalkisYeri);
    else
    {
        baslikYazdir();
        while(simdiki!=listeBasi)
        {
            if(!strcmp(simdiki->kalkisYeri,kalkisYeri))
                yazdir(*simdiki);

            simdiki=simdiki->kalkisYerineGoreSonraki;
        }
    }
}

void kalkisvarisYerineGoreListele(UCUS *hashListeBasi)
{
    char kalkisYeri[MAX],varisYeri[MAX];
    UCUS *listeBasi,*simdiki;

    printf("\nKalkis yeri: ");
    scanf("%s",kalkisYeri);
    kucukHarfeCevir(kalkisYeri);

    listeBasi=&hashListeBasi[toupper(kalkisYeri[0])-'A'];
    simdiki=listeBasi->kalkisYerineGoreSonraki;

    if(simdiki==listeBasi)
        printf("\t%s sehrinden kalkan bir ucus yok!\n",kalkisYeri);
    else
    {
        printf("Varis yeri: ");
        scanf("%s",varisYeri);
        kucukHarfeCevir(varisYeri);
        baslikYazdir();
        while(simdiki!=listeBasi)
        {
            if(strcmp(simdiki->kalkisYeri,kalkisYeri)==0 && strcmp(simdiki->varisYeri,varisYeri)==0)
                yazdir(*simdiki);

            simdiki=simdiki->kalkisYerineGoreSonraki;
        }
    }
}

void ucusVeBiletListele(UCUS **hashUcusNo)
{
    int ucusNo;
    UCUS *ucus;

    ucusNo=ucusNoAl();
    if((ucus=hashUcusNo[ucusNo])==NULL)
        printf("\t%d numarali ucus yok!\n",ucusNo+1000);
    else
    {
        baslikYazdir();
        yazdir(*ucus);
        if(ucus->sonraki==NULL)
            printf("\n\tBu ucus icin bilet satilmamistir!\n");
        else
        {
            printf("\n"
                   "TC Kimlik No    Fiyat\n"
                   "------------  -------\n");
            biletYazdir(ucus->sonraki);
        }
    }
}

void dolulugaGoreListele(UCUS *hashListeBasi)
{
    int i;
    UCUS *gecici;

    baslikYazdir();
    for(i=0;i<26;i++)
    {
        if(hashListeBasi[i].kalkisYerineGoreSonraki==&hashListeBasi[i])
            continue;
        else
        {
            gecici=hashListeBasi[i].kalkisYerineGoreSonraki;
            while(gecici!=&hashListeBasi[i])
            {
                if((float)100*(gecici->kapasite-gecici->bosKoltuk)/gecici->kapasite<50)
                    yazdir(*gecici);

                gecici=gecici->kalkisYerineGoreSonraki;
            }
        }
    }
}

void kalkisYerineGoreSiraliEkle(UCUS *listeBasiPtr,UCUS *yeni)
{
    UCUS *onceki,*gecici;

    onceki=listeBasiPtr;
    gecici=listeBasiPtr->kalkisYerineGoreSonraki;

    while(gecici!=listeBasiPtr && strcmp(yeni->kalkisYeri,gecici->kalkisYeri)>0)
    {
        onceki=gecici;
        gecici=gecici->kalkisYerineGoreSonraki;
    }

    while(strcmp(yeni->kalkisYeri,gecici->kalkisYeri)==0 && yeni->zaman>gecici->zaman)
    {
        onceki=gecici;
        gecici=gecici->kalkisYerineGoreSonraki;
    }

    onceki->kalkisYerineGoreSonraki=yeni;
    yeni->kalkisYerineGoreSonraki=gecici;
}

void tcKimligeGoreSiraliEkle(BILET **bas,BILET *yeni)
{
    BILET *gecici, *onceki;

    if(*bas==NULL) //kuyruk bossa
    {
        yeni->sonraki=NULL;
        *bas=yeni;
    }
    else
    {
        if(strcmp(yeni->TC,(*bas)->TC)<0) //en basa eklenecekse
        {
            yeni->sonraki=*bas;
            *bas=yeni;
        }
        else //araya ya da sona eklenecekse
        {
            onceki=*bas;
            gecici=(*bas)->sonraki;
            while((gecici!=NULL) && strcmp(gecici->TC,yeni->TC)<0) //eklenecek uygun yer araniyor
            {
                onceki=gecici;
                gecici=gecici->sonraki;
            }
            yeni->sonraki=gecici; //gecici NULL ise en sona, degilse onceki dugumu ile gecici dugumu arasina ekleniyor
            onceki->sonraki=yeni;
        }
    }
}

int ucusNoAl()
{
    int ucusNo;

    printf("\n");
    do{
        printf("Ucus numarasini giriniz: ");
        fflush(stdin);
        scanf("%d",&ucusNo);
        if(ucusNo<1000)
            printf("\tUcus numarasi 1000'den kucuk olamaz!\n");
        else if(ucusNo>9999)
            printf("\tUcus numarasi 9999'dan buyuk olamaz!\n");
    }while(ucusNo<1000 || ucusNo>9999);

    return ucusNo-1000;
}

int menuSecim()
{
    int secim;

    beklet(300000000);
    printf("\n"
            "\t 1) Yeni bir ucus ekleme\n"
            "\t 2) Bir ucusun kalkis zamanini guncelleme\n"
            "\t 3) Bir ucus icin bilet satma\n"
            "\t 4) Bir ucusu iptal etme\n"
            "\t 5) Bir ucus icin satilan bir bileti iptal etme\n"
            "\t 6) Bir yerden kalkan ucus bilgilerini goruntuleme\n"
            "\t 7) Kalkis ve varis yerine gore ucus bilgilerini goruntuleme\n"
            "\t 8) Bir ucusun ve o ucus icin satilan bilet bilgilerini goruntuleme\n"
            "\t 9) Doluluk orani %%50'nin altindaki ucuslari goruntuleme\n"
            "\t10) Bir yolcunun biletlerini goruntuleme\n"
            "\t11) Tum ucuslari goruntuleme\n"
            "\t12) Cikis\n"
            "\n");

    do{
        printf("\tSecim: ");
        fflush(stdin);
        scanf("%d",&secim);
        if(secim<1 || secim>12)
            printf("\t\tHatali secim!\n");
    }while(secim<1 || secim>12);
    //system("cls");

    return secim;
}

float biletFiyatHesapla(UCUS ucus)
{
    int i;
    float fiyat,BKO;

    BKO=100-((float)100*(ucus.kapasite-ucus.bosKoltuk)/ucus.kapasite);
    fiyat=ucus.fiyat;

    for(i=0;i<10;i++)
    {
        if(BKO<=(float)((10-i)*10.0) && BKO>(float)((10-i-1)*10.0)) //dongunun her donusunde yeni araliklar hesaplaniyor
        {
            fiyat*=(float)(1.0+i/10.0); //kosulu saglayan katsayi hesaplanip fiyata yansitiliyor
            break;
        }
    }

    return fiyat;
}

UCUS *dugumCikar(UCUS *listeBasiPtr,UCUS *silinecek)
{
    UCUS *gecici,*onceki;

    onceki=listeBasiPtr;
    gecici=listeBasiPtr->kalkisYerineGoreSonraki;

    while(gecici!=listeBasiPtr && gecici->ucusNo!=silinecek->ucusNo)
    {
        onceki=gecici;
        gecici=gecici->kalkisYerineGoreSonraki;
    }

    onceki->kalkisYerineGoreSonraki=gecici->kalkisYerineGoreSonraki;
    return gecici;
}

BILET *tcKimligeGoreAraVeCikar(BILET **bas,char *TC)
{
    BILET *gecici,*onceki;

    if(*bas==NULL) //kuyruk bossa
    {
        return NULL;
    }
    else
    {
        if(strcmp((*bas)->TC,TC)==0) //ilk dugum cikariliyorsa
        {
            gecici=*bas;
            *bas=(*bas)->sonraki;
        }
        else
        {
            onceki=*bas;
            gecici=onceki->sonraki;
            while(gecici!=NULL && strcmp(gecici->TC,TC)!=0) //cikarilacak dugum araniyor
            {
                onceki=gecici;
                gecici=gecici->sonraki;
            }
            if(gecici==NULL) //dugum bulunamadiysa
                return NULL;
            else
                onceki->sonraki=gecici->sonraki;
        }
    }

    return gecici;
}
