#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h> //sleep ve system fonksiyonu icin

struct ehliyet_bilgi{
        char tip[5];
        struct ehliyet_bilgi *sonraki;
};
struct is_bilgi{
        char adi[50];
        char adres[20];
        char pozisyon[50];
        float sure;
        struct is_bilgi *sonraki;
};
struct okul_bilgi{
    int not_ortalamasi;
    char adi[50];
    char tur[20];
    char bolum[50];
    int baslangic;
    int bitis;
    struct okul_bilgi *sonraki;
};
struct dil_bilgi{
    char adi[20];
    struct dil_bilgi *sonraki;
};
struct kisi{
    char ad[30];
    char adres[20];
    char telefon[11];
    char dogum_tarihi[5];
    char mail[20];

    struct dil_bilgi *dil;
    struct is_bilgi *is;
    struct okul_bilgi *okul;
    struct ehliyet_bilgi *ehliyet;

    struct kisi *sol;
    struct kisi *sag;

};
struct aranan_kisi{
    int deneyim;
    char egitim[20];
    int maks_yasi;

    struct ehliyet_bilgi *ehliyet;
    struct dil_bilgi *dil;
};

void ln(int n,char buf[n]){
    size_t ln=strlen(buf);
    if (ln > 0 && buf[ln-1] == '\n') {
    buf[--ln] = '\0';
    }
}
void fgetss(int n,char buf[n],FILE *st){
    fgets(buf,n,st);
    size_t ln=strlen(buf);
    if (ln > 0 && buf[ln-1] == '\n') {
        buf[--ln] = '\0';
    }
}
void fgetsss(int n,char buf[n]){
    fflush(stdin);
    fgets(buf,n,stdin);
    size_t ln=strlen(buf);
    if (ln > 0 && buf[ln-1] == '\n') {
        buf[--ln] = '\0';
    }
}

/** kisi_ekle fonksiyonu kok ve eklenecek dugumu alir agac olusturur                                         */
/** giris_ekle fonkisoyunu gonderilern dugum icin kullanicidan veri alir                                    */
/** is, dil, okul, ehliyet ekle fonksiyonlari gonderilen baglanti listesinin sonuna veri girisi alip ekler */
void kisi_ekle(struct kisi **kok,struct kisi **dugum);//bireysel
void giris_ekle(struct kisi **kok);//bireysel
void is_ekle(struct is_bilgi **is);//bireysel
void dil_ekle(struct dil_bilgi **dil);//bireysel
void okul_ekle(struct okul_bilgi **okul);//bireysel
void ehliyet_ekle(struct ehliyet_bilgi **ehliyet); //bireysel

/** gsiz ekle fonksiyonlari baslangici ve eklenecek veriyi alir baglati liste sonuna ekler           */
void gsiz_is_ekle(struct is_bilgi **is,struct is_bilgi **ekle); //okuma
void gsiz_dil_ekle(struct dil_bilgi **dil,struct dil_bilgi **ekle); //okuma
void gsiz_okul_ekle(struct okul_bilgi **okul,struct okul_bilgi **ekle); //okuma
void gsiz_ehliyet_ekle(struct ehliyet_bilgi **ehliyet,struct ehliyet_bilgi **ekle); //okuma

/** kisi sil fonksiyonlari dosyadan veriyi cekip kontrol eder siler veya duzeltir tekrar yazar  */
/** secenek kisiyi silme ve duzeltme kontrolu yapar                                            */
/** agac null fonksiyonu ramdeki agaci bosaltir                                               */
void kisi_sil_oku(struct kisi **kok,const int secenek);
void kisi_sil_yaz(struct kisi *dugum);
void kisi_sil_kopyala();
void kisi_sil_agac_null(struct kisi **kok);
void kisi_duzelt(struct kisi **dugum);//bireysel

/**  belirlenen kisiyi dosyaya veya ekrana yazim yapilir                                */
void kisi_goruntule(struct kisi *kok);//bireysel
void kisi_dosyaya_yaz(struct kisi *dugum);
void dosyaya_yaz(struct kisi *dugum);

/** dosya oku fonksiyonu dosyadan verileri okur                                     */
/** listele fonksiyonu ikili agaci inorder sekilde listeler                        */
void dosya_oku(struct kisi **kok);
void listele(struct kisi *kok);

/** arama istegi coklu ozellik aramak icin kullanicidan veri alir                                              */
/** arama istegi kontrol coklu ozellikleri kontrol eder                                                       */
/** calisma zamaninda arama sonuclari dosyaya yazilir kayit kontrole gore asil dosyaya yazilir ya da silinir */
void arama_dosya_kopyala();
void arama_istegi(struct aranan_kisi **aranan);
void arama_istegi_kontrol(const struct kisi *kok,const struct aranan_kisi *aranan);
void arama_dosya_yaz();
void arama_dosya_yaz_calisma_zamani(const struct kisi *dugum);
void kisi_yaz(const struct kisi *dugum);
void kayit_kontrol(const int a);

/** arama fonksiyonlari yazma fonksiyonlari ile birlikte calisir sonuclar ekrana ve dosyaya yazilir     */
void ad_ara(struct kisi *kok,const char aranan_ad[30]);
void en_az_lisans_kisi_ara(struct kisi *kok);
void ing_bilen_ara(struct kisi *kok);
void fazla_dil_bilen_ara(struct kisi *kok);
void deneyim_ara(struct kisi *kok,const int deneyim);
void deneyimsiz_ara(struct kisi *kok);
void yas_kisi_ara(struct kisi *kok,const int maks);
void ehliyet_ara(struct kisi *kok,const char tip[5]);


/** http://www.geeksforgeeks.org/write-a-c-program-to-calculate-size-of-a-tree/ */
int size(struct kisi *kok);


/** turlere gore listeleme fonksiyonlari sadece kisi adlarini ekrana listeler dosyaya yazim yapilmaz */
void listele_ad_postorder(struct kisi *kok);
void listele_ad_inorder(struct kisi *kok);
void listele_ad_preorder(struct kisi *kok);

/** agacin derinligini bulan fonksiyon                                           */
/** http://articles.leetcode.com/2010/04/maximum-height-of-binary-tree.html     */
/** adresinden alinmistir                                                      */
int maksimum_derinlik(struct kisi *kok);//


int main(){
    int secenek=-1;
    struct aranan_kisi *aranan=NULL;

    struct kisi *kok=NULL;

    FILE *oku=fopen("kisi.txt","r");
    FILE *oku_d=fopen("dil.txt","r");
    FILE *oku_i=fopen("is.txt","r");
    FILE *oku_o=fopen("okul.txt","r");
    FILE *oku_e=fopen("ehliyet.txt","r");
    if(oku!=NULL&&oku_d!=NULL&&oku_i!=NULL&&oku_o!=NULL&&oku_e!=NULL){
        if(fgetc(oku)!=EOF&&fgetc(oku_d)!=EOF&&fgetc(oku_e)!=EOF&&fgetc(oku_i)!=EOF&&fgetc(oku_o)!=EOF){
            fclose(oku);
            fclose(oku_d);
            fclose(oku_i);
            fclose(oku_o);
            fclose(oku_e);
            dosya_oku(&kok);
        }
    }
    do{
        printf("0:\tbireysel\n1:\tkurumsal\nsecim:");
        scanf("%d",&secenek);
    }while(secenek!=1&& secenek!=0);

    if(secenek==0){
         system("cls");
         system("color 1F");
        do{
            fflush(stdin);
            printf("SECENEKLER:\n");
            printf("0:\tCikis\n1:\tKisi Kayit\n2:\tKisi Arama\n3:\tKisi Silme\n4:\tKisi Duzelt\nsecim:");
            scanf("%d",&secenek);
            switch(secenek) {
                case 0: {
                    secenek=0;
                    break;
                }
                case 1: {
                    if(kok==NULL){
                        giris_ekle(&kok);
                    }
                    else if(kok!=NULL){
                        struct kisi *dugum=malloc(sizeof(struct kisi));
                        dugum->sag=NULL;
                        dugum->sol=NULL;
                        giris_ekle(&dugum);
                        kisi_ekle(&kok,&dugum);
                    }
                    break;
                }
                case 2: {
                    char aranan_ad[30];
                    printf("aranan ad ve soyad:");
                    fgetsss(30,aranan_ad);
                    //fgetss(30,aranan_ad,stdin);
                    //fgets(aranan_ad,30,stdin);
                    //ln(30,aranan_ad);
                    ad_ara(kok,aranan_ad);
                    remove("arama_sonuc2.txt");
                    break;
                }
                case 3: {
                    kisi_sil_agac_null(&kok);
                    kok=NULL;
                    FILE *oku=fopen("kisi.txt","r");
                    FILE *oku_d=fopen("dil.txt","r");
                    FILE *oku_i=fopen("is.txt","r");
                    FILE *oku_o=fopen("okul.txt","r");
                    FILE *oku_e=fopen("ehliyet.txt","r");
                    if(oku!=NULL&&oku_d!=NULL&&oku_i!=NULL&&oku_o!=NULL&&oku_e!=NULL) {
                        if(fgetc(oku)!=EOF&&fgetc(oku_d)!=EOF&&fgetc(oku_e)!=EOF&&fgetc(oku_i)!=EOF&&fgetc(oku_o)!=EOF) {
                            fclose(oku);
                            fclose(oku_d);
                            fclose(oku_i);
                            fclose(oku_o);
                            fclose(oku_e);
                            kisi_sil_oku(&kok,3);
                        }
                    }
                    break;
                }
                case 4: {
                    kisi_sil_agac_null(&kok);
                    kok=NULL;
                    FILE *oku=fopen("kisi.txt","r");
                    FILE *oku_d=fopen("dil.txt","r");
                    FILE *oku_i=fopen("is.txt","r");
                    FILE *oku_o=fopen("okul.txt","r");
                    FILE *oku_e=fopen("ehliyet.txt","r");
                    if(oku!=NULL&&oku_d!=NULL&&oku_i!=NULL&&oku_o!=NULL&&oku_e!=NULL) {
                        if(fgetc(oku)!=EOF&&fgetc(oku_d)!=EOF&&fgetc(oku_e)!=EOF&&fgetc(oku_i)!=EOF&&fgetc(oku_o)!=EOF) {
                            fclose(oku);
                            fclose(oku_d);
                            fclose(oku_i);
                            fclose(oku_o);
                            fclose(oku_e);
                            kisi_sil_oku(&kok,4);
                        }
                    }
                    break;
                }
                default: {
                    printf("Gecerli bir secenek giriniz\n");
                    secenek=6;
                    Sleep(1000);
                    system("cls");
                    break;
                }
            }
        } while(secenek!=0);
    }
    else if(secenek==1){
        system("cls");
        system("color 2F");
        remove("arama_sonuc.txt");
        remove("arama_sonuc2.txt");
       do{
            printf("SECENEKLER:\n");
            printf("uyari!\n-arama dosyasina sadece son arama kayit edilir.\n");
            printf("-arama dosyaya kayit edilirse onceki arama silinir.\n\n");
            printf("0:\tcikis\n1:\tagac derinligi ve listeleme turleri\n2:\ttum adaylar\n3:\tad kisi arama\n4:\ten az lisans arama\n5:\tingilizce bilenler\n");
            printf("6:\tbirden fazla dil bilenler\n7:\tmin deneyime gore arama\n8:\tdeneyimsiz arama\n9:\tmaksimum yasa gore\n");
            printf("10:\tehliyet arama\n11:\tcoklu ozellik arama\n12:\tgecmisi temizle\nsecim:");
            scanf("%d",&secenek);
            switch(secenek) {
                case 0: {
                    secenek=0;
                    break;
                }
                case 1: {
                    int s_listeleme;
                    do{
                        fflush(stdin);
                        printf("0:\tlistelemeden cikis\n1:\tmaksimum derinlik\n2:\tpostorder ad listeleme\n3:\tinorder ad listeleme\n4:\tpreorder ad listeleme\nsecim:");
                        scanf("%d",&s_listeleme);
                        switch(s_listeleme){
                            case 0:{
                                    s_listeleme=0;
                                break;
                            }
                            case 1:{
                                    printf("maksimum derinlik:%d\n",maksimum_derinlik(kok));
                                break;
                            }
                            case 2:{
                                listele_ad_postorder(kok);
                                printf("\nkisi sayisi:%d\n\n",size(kok));
                                break;
                            }
                            case 3:{
                                listele_ad_inorder(kok);
                                printf("\nkisi sayisi:%d\n\n",size(kok));
                                break;
                            }
                            case 4:{
                                listele_ad_preorder(kok);
                                printf("\nkisi sayisi:%d\n\n",size(kok));
                                break;
                            }
                            default:{
                                printf("yanlis secim yaptiniz\n");
                                s_listeleme=-1;
                                Sleep(1000);
                                system("cls");
                                break;
                            }
                        }
                    }while(s_listeleme==-1||s_listeleme!=0);

                    break;
                }
                case 2: {
                    listele(kok);
                    kayit_kontrol(1);
                    break;
                }
                case 3: {
                    char aranan_ad[30];
                    printf("aranan ad ve soyad:");
                    fgetsss(30,aranan_ad);
                    //fgetss(30,aranan_ad,stdin);
                    //fgets(aranan_ad,30,stdin);
                    //ln(30,aranan_ad);
                    ad_ara(kok,aranan_ad);
                    kayit_kontrol(1);
                    arama_dosya_kopyala();
                    break;
                }
                case 4: {
                    en_az_lisans_kisi_ara(kok);
                    kayit_kontrol(1);
                    arama_dosya_kopyala();
                    break;
                }
                case 5: {
                    ing_bilen_ara(kok);
                    kayit_kontrol(1);
                    arama_dosya_kopyala();
                    break;
                }
                case 6: {
                    fazla_dil_bilen_ara(kok);
                    kayit_kontrol(1);
                    arama_dosya_kopyala();
                    break;
                }
                case 7: {
                    int deneyim;
                    printf("deneyim miktari:");
                    scanf("%d",&deneyim);
                    deneyim_ara(kok,deneyim);
                    kayit_kontrol(1);
                    arama_dosya_kopyala();
                    break;
                }
                case 8: {
                    deneyimsiz_ara(kok);
                    kayit_kontrol(1);
                    arama_dosya_kopyala();
                    break;
                }
                case 9: {
                    printf("maks yas:");
                    int maks_yas;
                    scanf("%d",&maks_yas);
                    yas_kisi_ara(kok,maks_yas);
                    kayit_kontrol(1);
                    arama_dosya_kopyala();
                    break;
                }
                case 10: {
                    char ehliyet_tipi[5];
                    printf("ehliyet tipi(sadece 1 tane):");
                    fgetsss(5,ehliyet_tipi);
                    //fgetss(3,ehliyet_tipi,stdin);
                    //fgets(ehliyet_tipi,3,stdin);
                    //ln(3,ehliyet_tipi);
                    ehliyet_ara(kok,ehliyet_tipi);
                    kayit_kontrol(1);
                    arama_dosya_kopyala();
                    break;
                }
                case 11: {
                    arama_istegi(&aranan);
                    arama_istegi_kontrol(kok,aranan);
                    kayit_kontrol(1);
                    arama_dosya_kopyala();
                    break;
                }
                case 12:{
                    remove("tum_aramalar.txt");
                    break;
                }
                default: {
                    printf("Gecerli bir secenek giriniz\n");
                    secenek=-1;
                    Sleep(1000);
                    system("cls");
                    break;
                }
            }
       }while(secenek==-1||secenek!=0);

    }
    return 0;
}
void kisi_sil_agac_null(struct kisi **kok){
    if((*kok)!=NULL){
        kisi_sil_agac_null(&((*kok)->sol));
        kisi_sil_agac_null(&((*kok)->sag));
        free(*kok);
    }
}
void kisi_sil_oku(struct kisi **kok,const int secenek){
    char ad[30];
    printf("silmek/duzeltmek istediginiz kisinin adi:");
    fgetsss(30,ad);

    FILE *dosya_dil=fopen("dil.txt","r");
    FILE *dosya_kisi=fopen("kisi.txt","r");
    FILE *dosya_okul=fopen("okul.txt","r");
    FILE *dosya_is=fopen("is.txt","r");
    FILE *dosya_ehliyet=fopen("ehliyet.txt","r");

    /*if(dosya_kisi==NULL||dosya_is==NULL||dosya_dil==NULL||dosya_okul==NULL||dosya_ehliyet==NULL){
        printf("dosya acilamadi");
        exit(3);
    }*/
    int c;
    int say=0;
    do{
        if(say!=0){
            fseek(dosya_kisi,-1,SEEK_CUR);
        }
        struct kisi *ekle=malloc(sizeof(struct kisi));
        ekle->sol=NULL;
        ekle->sag=NULL;

        ekle->dil=NULL;
        ekle->is=NULL;
        ekle->okul=NULL;
        ekle->ehliyet=NULL;

        fscanf(dosya_kisi,"%[^,]s",ekle->ad);
        fgetc(dosya_kisi);
        fscanf(dosya_kisi,"%[^,]s",ekle->adres);
        fgetc(dosya_kisi);
        fscanf(dosya_kisi,"%[^,]s",ekle->telefon);
        fgetc(dosya_kisi);
        fscanf(dosya_kisi,"%[^,]s",ekle->dogum_tarihi);
        fgetc(dosya_kisi);
        fscanf(dosya_kisi,"%[^\n]s",ekle->mail);
        fgetc(dosya_kisi);

        char kontrol;
        char kontrol1;

        do{
            struct ehliyet_bilgi *ekle_ehliyet=malloc(sizeof(struct ehliyet_bilgi));
            ekle_ehliyet->sonraki=NULL;
            fscanf(dosya_ehliyet,"%[^,]s",ekle_ehliyet->tip);
            fgetc(dosya_ehliyet);
            kontrol=fgetc(dosya_ehliyet);
            gsiz_ehliyet_ekle(&(ekle->ehliyet),&(ekle_ehliyet));
        }while(kontrol=='+');
        fgetc(dosya_ehliyet);

        do{
            struct okul_bilgi *ekle_okul=malloc(sizeof(struct okul_bilgi));
            ekle_okul->sonraki=NULL;
            fscanf(dosya_okul,"%[^,]s",ekle_okul->adi);
            fgetc(dosya_okul);
            fscanf(dosya_okul,"%[^,]s",ekle_okul->bolum);
            fgetc(dosya_okul);
            fscanf(dosya_okul,"%[^,]s",ekle_okul->tur);
            fgetc(dosya_okul);
            fscanf(dosya_okul,"%d%[^,]",&(ekle_okul->not_ortalamasi),&kontrol1);
            fgetc(dosya_okul);
            fscanf(dosya_okul,"%d%[^,]",&(ekle_okul->baslangic),&kontrol1);
            fgetc(dosya_okul);
            fscanf(dosya_okul,"%d%[^,]",&(ekle_okul->bitis),&kontrol1);
            fgetc(dosya_okul);
            kontrol=fgetc(dosya_okul);
            gsiz_okul_ekle(&(ekle->okul),&(ekle_okul));
        }while(kontrol=='+');
        fgetc(dosya_okul);

        do{
            struct dil_bilgi *ekle_dil=malloc(sizeof(struct dil_bilgi));
            ekle_dil->sonraki=NULL;
            fscanf(dosya_dil,"%[^,]s",ekle_dil->adi);
            fgetc(dosya_dil);
            kontrol=fgetc(dosya_dil);
            gsiz_dil_ekle(&(ekle->dil),&(ekle_dil));
        }while(kontrol=='+');
        fgetc(dosya_dil);

        do{
            struct is_bilgi *ekle_is=malloc(sizeof(struct is_bilgi));
            ekle_is->sonraki=NULL;
            fscanf(dosya_is,"%[^,]s",ekle_is->adi);
            fgetc(dosya_is);
            fscanf(dosya_is,"%[^,]s",ekle_is->adres);
            fgetc(dosya_is);
            fscanf(dosya_is,"%[^,]s",ekle_is->pozisyon);
            fgetc(dosya_is);
            fscanf(dosya_is,"%f%[^,]",&(ekle_is->sure),&kontrol1);
            fgetc(dosya_is);
            kontrol=fgetc(dosya_is);
            gsiz_is_ekle(&(ekle->is),&(ekle_is));
        }while(kontrol=='+');
        fgetc(dosya_is);

        if(secenek==4){
            if(strcmp(ekle->ad,ad)==0){
                kisi_duzelt(&ekle);
            }
            kisi_sil_yaz(ekle);
            kisi_ekle(&(*kok),&ekle);
        }
        else if(secenek==3){
            if(strcmp(ekle->ad,ad)!=0){
                kisi_ekle(&(*kok),&ekle);
                kisi_sil_yaz(ekle);
            }
        }
    say++;
    c=fgetc(dosya_kisi);
    }while(c!=EOF);

fclose(dosya_ehliyet);
fclose(dosya_kisi);
fclose(dosya_okul);
fclose(dosya_is);
fclose(dosya_dil);
    if((*kok)!=NULL){
        kisi_sil_kopyala();
    }
    if((*kok)==NULL){
        remove("dil.txt");
        remove("is.txt");
        remove("kisi.txt");
        remove("okul.txt");
        remove("ehliyet.txt");
    }
}

void kisi_sil_yaz(struct kisi *dugum){
    FILE *dosya_kisi=fopen("kisi2.txt","a");
    FILE *dosya_is=fopen("is2.txt","a");
    FILE *dosya_dil=fopen("dil2.txt","a");
    FILE *dosya_okul=fopen("okul2.txt","a");
    FILE *dosya_ehliyet=fopen("ehliyet2.txt","a");

    if(dosya_kisi==NULL||dosya_is==NULL||dosya_dil==NULL||dosya_okul==NULL||dosya_ehliyet==NULL){
        printf("dosya acilamadi");
        exit(2);
    }
            fprintf(dosya_kisi,"%s,%s,%s,%s,%s\n",dugum->ad,dugum->adres,dugum->telefon,dugum->dogum_tarihi,dugum->mail);

            struct ehliyet_bilgi *c;
            for(c=dugum->ehliyet; c!=NULL; c=c->sonraki) {
                if(c->sonraki!=NULL) {
                    fprintf(dosya_ehliyet,"%s,+",c->tip);
                } else {
                    fprintf(dosya_ehliyet,"%s,-\n",c->tip);
                }
            }

              struct dil_bilgi *e;
                for(e=dugum->dil ; e!=NULL ; e=e->sonraki){
                    if(e->sonraki!=NULL){
                         fprintf(dosya_dil,"%s,+",e->adi);
                    }
                    else{
                        fprintf(dosya_dil,"%s,-\n",e->adi);
                    }
                }

                struct okul_bilgi *b;
                for(b=dugum->okul;b!=NULL;b=b->sonraki){
                    if(b->sonraki!=NULL){
                        fprintf(dosya_okul,"%s,%s,%s,%d,%d,%d,+",b->adi,b->bolum,b->tur,b->not_ortalamasi,b->baslangic,b->bitis);
                    }else {
                    fprintf(dosya_okul,"%s,%s,%s,%d,%d,%d,-\n",b->adi,b->bolum,b->tur,b->not_ortalamasi,b->baslangic,b->bitis);
                    }
                }


            struct is_bilgi *a;
            for(a=dugum->is; a!=NULL; a=a->sonraki) {
                if(a->sonraki!=NULL) {
                    fprintf(dosya_is,"%s,%s,%s,%0.1f,+",a->adi,a->adres,a->pozisyon,a->sure);
                } else {
                    fprintf(dosya_is,"%s,%s,%s,%0.1f,-\n",a->adi,a->adres,a->pozisyon,a->sure);
                }
            }
fclose(dosya_ehliyet);
fclose(dosya_okul);
fclose(dosya_is);
fclose(dosya_dil);
fclose(dosya_kisi);
}

void kisi_sil_kopyala(){
    remove("dil.txt");
    remove("is.txt");
    remove("kisi.txt");
    remove("okul.txt");
    remove("ehliyet.txt");

    FILE *dosya_kisi=fopen("kisi.txt","a");
    FILE *dosya_is=fopen("is.txt","a");
    FILE *dosya_dil=fopen("dil.txt","a");
    FILE *dosya_okul=fopen("okul.txt","a");
    FILE *dosya_ehliyet=fopen("ehliyet.txt","a");

    FILE *dosya_kisi2=fopen("kisi2.txt","r");
    FILE *dosya_is2=fopen("is2.txt","r");
    FILE *dosya_dil2=fopen("dil2.txt","r");
    FILE *dosya_okul2=fopen("okul2.txt","r");
    FILE *dosya_ehliyet2=fopen("ehliyet2.txt","r");
    if(dosya_kisi==NULL||dosya_is==NULL||dosya_dil==NULL||dosya_okul==NULL||dosya_ehliyet==NULL||dosya_kisi2==NULL||dosya_is2==NULL||dosya_dil2==NULL||dosya_okul2==NULL||dosya_ehliyet2==NULL){
        printf("kisi sil kopyala fonk dosya acilamadi");
        exit(8);
    }

    char ch_k;
    while( ( ch_k = fgetc(dosya_kisi2) ) != EOF ){
      fputc(ch_k, dosya_kisi);
    }
    char ch_i;
    while( ( ch_i = fgetc(dosya_is2) ) != EOF ){
      fputc(ch_i, dosya_is);
    }
    char ch_d;
    while( ( ch_d = fgetc(dosya_dil2) ) != EOF ){
      fputc(ch_d, dosya_dil);
    }
    char ch_o;
    while( ( ch_o = fgetc(dosya_okul2) ) != EOF ){
      fputc(ch_o, dosya_okul);
    }
    char ch_e;
    while( ( ch_e = fgetc(dosya_ehliyet2) ) != EOF ){
      fputc(ch_e, dosya_ehliyet);
    }

fclose(dosya_ehliyet);
fclose(dosya_okul);
fclose(dosya_is);
fclose(dosya_dil);
fclose(dosya_kisi);


fclose(dosya_ehliyet2);
fclose(dosya_okul2);
fclose(dosya_is2);
fclose(dosya_dil2);
fclose(dosya_kisi2);
remove("dil2.txt");
remove("is2.txt");
remove("kisi2.txt");
remove("okul2.txt");
remove("ehliyet2.txt");

}

void kisi_duzelt(struct kisi **dugum){//bireysel

    struct kisi *ekle=malloc(sizeof(struct kisi));

    ekle->sol=(*dugum)->sol;
    ekle->sag=(*dugum)->sag;

    ekle->ehliyet=NULL;
    ekle->dil=NULL;
    ekle->is=NULL;
    ekle->okul=NULL;

    printf("ad soyad giriniz:");
    fgetsss(30,ekle->ad);

    printf("adres giriniz:");
    fgetsss(20,ekle->adres);

    printf("telefon giriniz:");
    fgetsss(11,ekle->telefon);

    printf("mail giriniz:");
    fgetsss(20,ekle->mail);

    printf("dogum tarihi giriniz:");
    fgetsss(5,ekle->dogum_tarihi);


    system("cls");

    fflush(stdin);
    printf("ehliyet:\n");
    ehliyet_ekle(&(ekle->ehliyet));
    system("cls");

    printf("dil:\n");
    fflush(stdin);
    dil_ekle(&(ekle->dil));
    system("cls");

    printf("is:\n");
    fflush(stdin);
    is_ekle(&(ekle->is));
    system("cls");

    printf("okul:\n");
    fflush(stdin);
    okul_ekle(&(ekle->okul));
    system("cls");

    int giris_secimi;
    do{
        fflush(stdin);
        printf("0:\tbitir\n1:\tehliyet ekle\n2:\tdil ekle\n3:\tis ekle\n4:\tokul ekle\nsecim:");
        scanf("%d",&giris_secimi);
        switch(giris_secimi){
            case 0:{ giris_secimi=0;
                break;
            }
            case 1:{ ehliyet_ekle(&(ekle->ehliyet));
                    system("cls");
                break;
            }
            case 2:{ dil_ekle(&(ekle->dil));
                     system("cls");
                break;
            }
            case 3:{ is_ekle(&(ekle->is));
                     system("cls");
                break;
            }
            case 4:{ okul_ekle(&(ekle->okul));
                     system("cls");
                break;
            }
            default:{ printf("lutfen gecerli secenek giriniz.");
                break;
            }
        }
    }while(giris_secimi!=0);
    system("cls");
    *dugum=ekle;
}


void kisi_goruntule(struct kisi *kok){//bireysel
    char aranan_ad[30];
    printf("aranan ad:");
    fgetsss(30,aranan_ad);
    //fgetss(30,aranan_ad,stdin);
    //fgets(aranan_ad,30,stdin);
    //ln(30,aranan_ad);

    ad_ara(kok,aranan_ad);
}
void kisi_ekle(struct kisi **kok,struct kisi **dugum){
    if(*kok==NULL){
        *kok=*dugum;
    }
    else if((*kok)!=NULL){
        int kontrol=strcmp((*kok)->ad,(*dugum)->ad);
        if (kontrol >= 0){
            if((*kok)->sol==NULL){
                (*kok)->sol=malloc(sizeof(struct kisi));
                (*kok)->sol=*dugum;
            }else{
                kisi_ekle(&((*kok)->sol),&(*dugum));
            }
        }else if (kontrol < 0){
            if((*kok)->sag==NULL){
                (*kok)->sag=malloc(sizeof(struct kisi));
                (*kok)->sag=*dugum;
            }else{
                kisi_ekle(&((*kok)->sag),&(*dugum));
            }
        }
    }
}

void giris_ekle(struct kisi **kok){
    struct kisi *ekle=malloc(sizeof(struct kisi));

    ekle->sol=NULL;
    ekle->sag=NULL;

    ekle->ehliyet=NULL;
    ekle->dil=NULL;
    ekle->is=NULL;
    ekle->okul=NULL;

    printf("isim ve soyisim giriniz:");
    fgetsss(30,ekle->ad);
    //fgetss(30,ekle->ad,stdin);
    //fgets(ekle->ad,30,stdin);
    //ln(30,ekle->ad);

    printf("adres giriniz:");
    fgetsss(20,ekle->adres);
    //fgetss(20,ekle->adres,stdin);
    //fgets(ekle->adres,20,stdin);
    // ln(20,ekle->adres);

    printf("telefon giriniz:");
    fgetsss(11,ekle->telefon);
    //fgetss(11,ekle->telefon,stdin);
    //fgets(ekle->telefon,11,stdin);
    //ln(11,ekle->telefon);

    printf("mail giriniz:");
    fgetsss(20,ekle->mail);
    //fgetss(20,ekle->mail,stdin);
    //fgets(ekle->mail,20,stdin);
    //ln(20,ekle->mail);

    printf("dogum tarihi giriniz:");
    fgetsss(5,ekle->dogum_tarihi);
    //fgetss(5,ekle->dogum_tarihi,stdin);
    //fgets(ekle->dogum_tarihi,5,stdin);
    //ln(5,ekle->dogum_tarihi);

    system("cls");

    fflush(stdin);
    printf("ehliyet:\n");
    ehliyet_ekle(&(ekle->ehliyet));
    system("cls");

    printf("dil:\n");
    fflush(stdin);
    dil_ekle(&(ekle->dil));
    system("cls");

    printf("is:\n");
    fflush(stdin);
    is_ekle(&(ekle->is));
    system("cls");

    printf("okul:\n");
    fflush(stdin);
    okul_ekle(&(ekle->okul));
    system("cls");

    int giris_secimi;
    do{
        fflush(stdin);
        printf("0:\tbitir\n1:\tehliyet ekle\n2:\tdil ekle\n3:\tis ekle\n4:\tokul ekle\nsecim:");
        scanf("%d",&giris_secimi);
        switch(giris_secimi){
            case 0:{ giris_secimi=0;
                break;
            }
            case 1:{ ehliyet_ekle(&(ekle->ehliyet));
                    system("cls");
                break;
            }
            case 2:{ dil_ekle(&(ekle->dil));
                     system("cls");
                break;
            }
            case 3:{ is_ekle(&(ekle->is));
                     system("cls");
                break;
            }
            case 4:{ okul_ekle(&(ekle->okul));
                     system("cls");
                break;
            }
            default:{ printf("lutfen gecerli secenek giriniz.");
                break;
            }
        }
    }while(giris_secimi!=0);

    system("cls");

    *kok=ekle;
   kisi_dosyaya_yaz(*kok);
}

void is_ekle(struct is_bilgi **is){
    struct is_bilgi *ekle=malloc(sizeof(struct is_bilgi));
     struct is_bilgi *is_son=NULL;
       struct is_bilgi *e;
    for(e=(*is);e!=NULL;e=e->sonraki){
        if(e->sonraki==NULL){
            is_son=e;
        }
    }
    printf("is adi:");
    fgetsss(50,ekle->adi);
    //fgetss(50,ekle->adi,stdin);
    //fgets(ekle->adi,50,stdin);
    //ln(50,ekle->adi);

    printf("is adresi(1'den fazla ise bosluk birakarak giriniz):");
    fgetsss(20,ekle->adres);
    //fgetss(20,ekle->adres,stdin);
    //fgets(ekle->adres,20,stdin);
    //ln(20,ekle->adres);

    printf("is pozisyon:");
    fgetsss(50,ekle->pozisyon);
    //fgetss(50,ekle->pozisyon,stdin);
    //fgets(ekle->pozisyon,50,stdin);
    //ln(50,ekle->pozisyon);

    fflush(stdin);
    printf("is suresi:");
    scanf("%f",&(ekle->sure));

    ekle->sonraki=NULL;
    if(*is==NULL){
         *is=ekle;
    }
    else if(*is!=NULL){
            is_son->sonraki=ekle;
    }
}
void dil_ekle(struct dil_bilgi **dil){

    struct dil_bilgi *ekle=malloc(sizeof(struct dil_bilgi));
    struct dil_bilgi *dil_son=NULL;
    struct dil_bilgi *e;
    for(e=(*dil);e!=NULL;e=e->sonraki){
        if(e->sonraki==NULL){
            dil_son=e;
        }
    }

    printf("1 tane dil giriniz:");
    fgetsss(20,ekle->adi);
    //fgetss(20,ekle->adi,stdin);
    //fgets(ekle->adi,20,stdin);
    //ln(20,ekle->adi);

    ekle->sonraki=NULL;
    if(*dil==NULL){
        *dil=ekle;
    }
    else if(*dil!=NULL){
        dil_son->sonraki=ekle;
    }
}
void okul_ekle(struct okul_bilgi **okul){
    struct okul_bilgi *ekle=malloc(sizeof(struct okul_bilgi));
    struct okul_bilgi *okul_son=NULL;

    struct okul_bilgi *e;
    for(e=(*okul);e!=NULL;e=e->sonraki){
        if(e->sonraki==NULL){
            okul_son=e;
        }
    }
    printf("okul adi:");
    fgetsss(50,ekle->adi);
    //fgetss(50,ekle->adi,stdin);
    //fgets(ekle->adi,50,stdin);
    //ln(50,ekle->adi);

    printf("okul bolumu:");
    fgetsss(50,ekle->bolum);
    //fgetss(50,ekle->bolum,stdin);
    //fgets(ekle->bolum,50,stdin);
    //ln(50,ekle->bolum);

    printf("okul turu:");
    fgetsss(20,ekle->tur);
    //fgetss(10,ekle->tur,stdin);
    //fgets(ekle->tur,10,stdin);
    //ln(10,ekle->tur);

    do{fflush(stdin);
        printf("okul not ortalamasi(hesaplanmadiysa 0 girin):");
        scanf("%d",&(ekle->not_ortalamasi));
    }while(ekle->not_ortalamasi>100||ekle->not_ortalamasi<0);

    do{fflush(stdin);
        printf("okul baslangic:");
        scanf("%d",&(ekle->baslangic));
        printf("okul bitis(bitmediyse 0 girin):");
        scanf("%d",&(ekle->bitis));
        if(ekle->bitis==0){
            ekle->bitis=3000;
         }
    }while(ekle->baslangic>ekle->bitis);

ekle->sonraki=NULL;
    if(*okul==NULL) {
        *okul=ekle;
    } else if(*okul!=NULL) {
        okul_son->sonraki=ekle;
    }
}

void ehliyet_ekle(struct ehliyet_bilgi **ehliyet){

    struct ehliyet_bilgi *ekle=malloc(sizeof(struct ehliyet_bilgi));
    struct ehliyet_bilgi *ehliyet_son=NULL;
    struct ehliyet_bilgi *e;
    for(e=(*ehliyet);e!=NULL;e=e->sonraki){
        if(e->sonraki==NULL){
            ehliyet_son=e;
        }
    }
    printf("1 tane ehliyet tipi giriniz:");
    fgetsss(5,ekle->tip);
    //fgetss(3,ekle->tip,stdin);
    //fgets(ekle->tip,3,stdin);
    //ln(3,ekle->tip);

    ekle->sonraki=NULL;
    if(*ehliyet==NULL){
        *ehliyet=ekle;
    }
    else if(*ehliyet!=NULL){
        ehliyet_son->sonraki=ekle;
    }
}
void kisi_dosyaya_yaz(struct kisi *dugum){

    FILE *dosya_kisi=fopen("kisi.txt","a");
    FILE *dosya_is=fopen("is.txt","a");
    FILE *dosya_dil=fopen("dil.txt","a");
    FILE *dosya_okul=fopen("okul.txt","a");
    FILE *dosya_ehliyet=fopen("ehliyet.txt","a");

    if(dosya_kisi==NULL||dosya_is==NULL||dosya_dil==NULL||dosya_okul==NULL||dosya_ehliyet==NULL){
        printf("dosya acilamadi");
        exit(2);
    }
            fprintf(dosya_kisi,"%s,%s,%s,%s,%s\n",dugum->ad,dugum->adres,dugum->telefon,dugum->dogum_tarihi,dugum->mail);

            struct ehliyet_bilgi *c;
            for(c=dugum->ehliyet; c!=NULL; c=c->sonraki) {
                if(c->sonraki!=NULL) {
                    fprintf(dosya_ehliyet,"%s,+",c->tip);
                } else {
                    fprintf(dosya_ehliyet,"%s,-\n",c->tip);
                }
            }

              struct dil_bilgi *e;
                for(e=dugum->dil ; e!=NULL ; e=e->sonraki){
                    if(e->sonraki!=NULL){
                         fprintf(dosya_dil,"%s,+",e->adi);
                    }
                    else{
                        fprintf(dosya_dil,"%s,-\n",e->adi);
                    }
                }

                struct okul_bilgi *b;
                for(b=dugum->okul;b!=NULL;b=b->sonraki){
                    if(b->sonraki!=NULL){
                        fprintf(dosya_okul,"%s,%s,%s,%d,%d,%d,+",b->adi,b->bolum,b->tur,b->not_ortalamasi,b->baslangic,b->bitis);
                    }else {
                    fprintf(dosya_okul,"%s,%s,%s,%d,%d,%d,-\n",b->adi,b->bolum,b->tur,b->not_ortalamasi,b->baslangic,b->bitis);
                    }
                }


            struct is_bilgi *a;
            for(a=dugum->is; a!=NULL; a=a->sonraki) {
                if(a->sonraki!=NULL) {
                    fprintf(dosya_is,"%s,%s,%s,%0.1f,+",a->adi,a->adres,a->pozisyon,a->sure);
                } else {
                    fprintf(dosya_is,"%s,%s,%s,%0.1f,-\n",a->adi,a->adres,a->pozisyon,a->sure);
                }
            }

fclose(dosya_ehliyet);
fclose(dosya_okul);
fclose(dosya_is);
fclose(dosya_dil);
fclose(dosya_kisi);
}

void dosyaya_yaz(struct kisi *dugum){
    FILE *dosya_kisi=fopen("kisi.txt","a");
    FILE *dosya_is=fopen("is.txt","a");
    FILE *dosya_dil=fopen("dil.txt","a");
    FILE *dosya_okul=fopen("okul.txt","a");
    FILE *dosya_ehliyet=fopen("ehliyet.txt","a");

    if(dosya_kisi==NULL||dosya_is==NULL||dosya_dil==NULL||dosya_okul==NULL||dosya_ehliyet==NULL){
        printf("dosya acilamadi");
        exit(2);
    }
    void yaz(struct kisi *dugum) {
        if(dugum!=NULL) {
            yaz(dugum->sol);

            fprintf(dosya_kisi,"%s,%s,%s,%s,%s\n",dugum->ad,dugum->adres,dugum->telefon,dugum->dogum_tarihi,dugum->mail);

            struct ehliyet_bilgi *c;
            for(c=dugum->ehliyet; c!=NULL; c=c->sonraki) {
                if(c->sonraki!=NULL) {
                    fprintf(dosya_ehliyet,"%s,+",c->tip);
                } else {
                    fprintf(dosya_ehliyet,"%s,-\n",c->tip);
                }
            }

              struct dil_bilgi *e;
                for(e=dugum->dil ; e!=NULL ; e=e->sonraki){
                    if(e->sonraki!=NULL){
                         fprintf(dosya_dil,"%s,+",e->adi);
                    }
                    else{
                        fprintf(dosya_dil,"%s,-\n",e->adi);
                    }
                }

                struct okul_bilgi *b;
                for(b=dugum->okul;b!=NULL;b=b->sonraki){
                    if(b->sonraki!=NULL){
                        fprintf(dosya_okul,"%s,%s,%s,%d,%d,%d,+",b->adi,b->bolum,b->tur,b->not_ortalamasi,b->baslangic,b->bitis);
                    }else {
                    fprintf(dosya_okul,"%s,%s,%s,%d,%d,%d,-\n",b->adi,b->bolum,b->tur,b->not_ortalamasi,b->baslangic,b->bitis);
                    }
                }


            struct is_bilgi *a;
            for(a=dugum->is; a!=NULL; a=a->sonraki) {
                if(a->sonraki!=NULL) {
                    fprintf(dosya_is,"%s,%s,%s,%0.1f,+",a->adi,a->adres,a->pozisyon,a->sure);
                } else {
                    fprintf(dosya_is,"%s,%s,%s,%0.1f,-\n",a->adi,a->adres,a->pozisyon,a->sure);
                }
            }
            yaz(dugum->sag);
        }
    }
    yaz(dugum);

fclose(dosya_ehliyet);
fclose(dosya_okul);
fclose(dosya_is);
fclose(dosya_dil);
fclose(dosya_kisi);
}
void dosya_oku(struct kisi **kok){

    FILE *dosya_dil=fopen("dil.txt","r");
    FILE *dosya_kisi=fopen("kisi.txt","r");
    FILE *dosya_okul=fopen("okul.txt","r");
    FILE *dosya_is=fopen("is.txt","r");
    FILE *dosya_ehliyet=fopen("ehliyet.txt","r");

    if(dosya_kisi==NULL||dosya_is==NULL||dosya_dil==NULL||dosya_okul==NULL||dosya_ehliyet==NULL){
        printf("dosya acilamadi");
        exit(3);
    }
    int c;
    int say=0;
    do{
        if(say!=0){
            fseek(dosya_kisi,-1,SEEK_CUR);
        }
        struct kisi *ekle=malloc(sizeof(struct kisi));
        ekle->sol=NULL;
        ekle->sag=NULL;

        ekle->dil=NULL;
        ekle->is=NULL;
        ekle->okul=NULL;
        ekle->ehliyet=NULL;

        fscanf(dosya_kisi,"%[^,]s",ekle->ad);
        fgetc(dosya_kisi);
        fscanf(dosya_kisi,"%[^,]s",ekle->adres);
        fgetc(dosya_kisi);
        fscanf(dosya_kisi,"%[^,]s",ekle->telefon);
        fgetc(dosya_kisi);
        fscanf(dosya_kisi,"%[^,]s",ekle->dogum_tarihi);
        fgetc(dosya_kisi);
        fscanf(dosya_kisi,"%[^\n]s",ekle->mail);
        fgetc(dosya_kisi);

        char kontrol;
        char kontrol1;
        do{
            struct ehliyet_bilgi *ekle_ehliyet=malloc(sizeof(struct ehliyet_bilgi));
            ekle_ehliyet->sonraki=NULL;
            fscanf(dosya_ehliyet,"%[^,]s",ekle_ehliyet->tip);
            fgetc(dosya_ehliyet);
            kontrol=fgetc(dosya_ehliyet);
            gsiz_ehliyet_ekle(&(ekle->ehliyet),&(ekle_ehliyet));
        }while(kontrol=='+');
        fgetc(dosya_ehliyet);

        do{
            struct okul_bilgi *ekle_okul=malloc(sizeof(struct okul_bilgi));
            ekle_okul->sonraki=NULL;
            fscanf(dosya_okul,"%[^,]s",ekle_okul->adi);
            fgetc(dosya_okul);
            fscanf(dosya_okul,"%[^,]s",ekle_okul->bolum);
            fgetc(dosya_okul);
            fscanf(dosya_okul,"%[^,]s",ekle_okul->tur);
            fgetc(dosya_okul);
            fscanf(dosya_okul,"%d%[^,]",&(ekle_okul->not_ortalamasi),&kontrol1);
            fgetc(dosya_okul);
            fscanf(dosya_okul,"%d%[^,]",&(ekle_okul->baslangic),&kontrol1);
            fgetc(dosya_okul);
            fscanf(dosya_okul,"%d%[^,]",&(ekle_okul->bitis),&kontrol1);
            fgetc(dosya_okul);
            kontrol=fgetc(dosya_okul);
            gsiz_okul_ekle(&(ekle->okul),&(ekle_okul));
        }while(kontrol=='+');
        fgetc(dosya_okul);

        do{
            struct dil_bilgi *ekle_dil=malloc(sizeof(struct dil_bilgi));
            ekle_dil->sonraki=NULL;
            fscanf(dosya_dil,"%[^,]s",ekle_dil->adi);
            fgetc(dosya_dil);
            kontrol=fgetc(dosya_dil);
            gsiz_dil_ekle(&(ekle->dil),&(ekle_dil));
        }while(kontrol=='+');
        fgetc(dosya_dil);

        do{
            struct is_bilgi *ekle_is=malloc(sizeof(struct is_bilgi));
            ekle_is->sonraki=NULL;
            fscanf(dosya_is,"%[^,]s",ekle_is->adi);
            fgetc(dosya_is);
            fscanf(dosya_is,"%[^,]s",ekle_is->adres);
            fgetc(dosya_is);
            fscanf(dosya_is,"%[^,]s",ekle_is->pozisyon);
            fgetc(dosya_is);
            fscanf(dosya_is,"%f%[^,]",&(ekle_is->sure),&kontrol1);
            fgetc(dosya_is);
            kontrol=fgetc(dosya_is);
            gsiz_is_ekle(&(ekle->is),&(ekle_is));
        }while(kontrol=='+');
        fgetc(dosya_is);

        kisi_ekle(&(*kok),&ekle);
    say++;
    c=fgetc(dosya_kisi);
    }while(c!=EOF);

fclose(dosya_ehliyet);
fclose(dosya_kisi);
fclose(dosya_okul);
fclose(dosya_is);
fclose(dosya_dil);
}
void gsiz_okul_ekle(struct okul_bilgi **okul,struct okul_bilgi **ekle){

    struct okul_bilgi *okul_son=NULL;
    struct okul_bilgi *e;
    for(e=(*okul);e!=NULL;e=e->sonraki){
        if(e->sonraki==NULL){
            okul_son=e;
        }
    }
    (*ekle)->sonraki=NULL;

    if(*okul==NULL) {
        *okul=*ekle;
    } else if(*okul!=NULL) {
        okul_son->sonraki=(*ekle);
    }
}
void gsiz_ehliyet_ekle(struct ehliyet_bilgi **ehliyet,struct ehliyet_bilgi **ekle){
    struct ehliyet_bilgi *ehliyet_son=NULL;
    struct ehliyet_bilgi *e;
    for(e=(*ehliyet);e!=NULL;e=e->sonraki){
        if(e->sonraki==NULL){
            ehliyet_son=e;
        }
    }

    (*ekle)->sonraki=NULL;

    if(*ehliyet==NULL){
        *ehliyet=*ekle;
    }
    else if(*ehliyet!=NULL){
        ehliyet_son->sonraki=(*ekle);
    }

}

void gsiz_dil_ekle(struct dil_bilgi **dil,struct dil_bilgi **ekle){

    struct dil_bilgi *dil_son=NULL;
    struct dil_bilgi *e;
    for(e=(*dil);e!=NULL;e=e->sonraki){
        if(e->sonraki==NULL){
            dil_son=e;
        }
    }

    (*ekle)->sonraki=NULL;

    if(*dil==NULL){
        *dil=*ekle;
    }
    else if(*dil!=NULL){
        dil_son->sonraki=(*ekle);
    }
}
void gsiz_is_ekle(struct is_bilgi **is,struct is_bilgi **ekle){
    struct is_bilgi *is_son=NULL;
    struct is_bilgi *e;
    for(e=(*is);e!=NULL;e=e->sonraki){
        if(e->sonraki==NULL){
            is_son=e;
        }
    }

(*ekle)->sonraki=NULL;
    if(*is==NULL){
         *is=*ekle;
    }
    else if(*is!=NULL){
            is_son->sonraki=(*ekle);
    }
}

int size(struct kisi *kok){
  if (kok==NULL)
    return 0;
  else
    return(size(kok->sol) + 1 + size(kok->sag));
}
void listele(struct kisi *kok){
    if(kok!=NULL){
        listele(kok->sol);
        printf("\n-------------------------------------------------------------------------------------------\n");

        printf("%-20s\t%-15s\t%-11s\t%-5s\t%-20s\t\n",kok->ad,kok->adres,kok->telefon,kok->dogum_tarihi,kok->mail);

        printf("ehliyet:");
        struct ehliyet_bilgi *k;
        for(k=kok->ehliyet;k!=NULL;k=k->sonraki){
            printf("%s,",k->tip);
        }
        printf("\ndil:");
        struct dil_bilgi *c;
        for(c=kok->dil;c!=NULL;c=c->sonraki){
            printf("%s,",c->adi);
        }
        printf("\n\nis:\n");
        struct is_bilgi *e;
        for(e=kok->is;e!=NULL;e=e->sonraki){
            printf("%s\t%s\t%s\t%0.1f\n",e->adi,e->adres,e->pozisyon,e->sure);
        }
        printf("\nokul:\n");
        struct okul_bilgi *b;
        for(b=kok->okul;b!=NULL;b=b->sonraki){
            printf("%s\t%s\t%s\t%d-%d\t%d\n",b->adi,b->tur,b->bolum,b->baslangic,b->bitis,b->not_ortalamasi);

        }
        printf("\n-------------------------------------------------------------------------------------------\n");
        arama_dosya_yaz_calisma_zamani(kok);

        listele(kok->sag);
    }
}
void listele_ad_postorder(struct kisi *kok){
    if(kok!=NULL){
        listele_ad_postorder(kok->sol);
        listele_ad_postorder(kok->sag);
        puts(kok->ad);
    }
}
void listele_ad_inorder(struct kisi *kok){
    if(kok!=NULL){
        listele_ad_inorder(kok->sol);
        puts(kok->ad);
        listele_ad_inorder(kok->sag);
    }
}
void listele_ad_preorder(struct kisi *kok){
    if(kok!=NULL){
        puts(kok->ad);
        listele_ad_preorder(kok->sol);
        listele_ad_preorder(kok->sag);
    }
}

int maksimum_derinlik(struct kisi *kok) { //http://articles.leetcode.com/2010/04/maximum-height-of-binary-tree.html
    if (!kok)
        return 0;
    int sol_boy = maksimum_derinlik(kok->sol);
    int sag_boy = maksimum_derinlik(kok->sag);
    return (sol_boy > sag_boy) ? sol_boy + 1 : sag_boy + 1;
}

void kisi_yaz(const struct kisi *dugum){

        printf("\n------------------------------------------\n");
        printf("%s\t%s\t%s\t%s\t%s\n",dugum->ad,dugum->adres,dugum->telefon,dugum->dogum_tarihi,dugum->mail);
        printf("ehliyet:");
        struct ehliyet_bilgi *k;
        for(k=dugum->ehliyet;k!=NULL;k=k->sonraki){
            printf("%s,",k->tip);
        }
        printf("\ndil:");
        struct dil_bilgi *c;
        for(c=dugum->dil;c!=NULL;c=c->sonraki){
            printf("%s,",c->adi);
        }
        printf("\nis:\n");
        struct is_bilgi *e;
        for(e=dugum->is;e!=NULL;e=e->sonraki){
            printf("%s\t%s\t%s\t%0.1f\n",e->adi,e->adres,e->pozisyon,e->sure);
        }
        printf("\nokul:\n");
        struct okul_bilgi *b;
        for(b=dugum->okul;b!=NULL;b=b->sonraki){
            printf("%s\t%s\t%s\t%d-%d\t%d\n",b->adi,b->tur,b->bolum,b->baslangic,b->bitis,b->not_ortalamasi);
        }
        printf("\n------------------------------------------\n");
    arama_dosya_yaz_calisma_zamani(dugum);
}
void arama_dosya_kopyala(){
    FILE *sonuclar=fopen("arama_sonuc.txt","r");

    if(sonuclar!=NULL){
        FILE *t_aramalar=fopen("tum_aramalar.txt","a");
        if(t_aramalar==NULL){
            printf("arama kopyala dosya acilmadi");
            exit(10);
        }
        char ch_k;
        while( ( ch_k = fgetc(sonuclar) ) != EOF ){
        fputc(ch_k, t_aramalar);
        }
        fprintf(t_aramalar,"\n");

    fclose(sonuclar);
    fclose(t_aramalar);
    }

}

void kayit_kontrol(const int a){
    int kayit;
    FILE *kontrol=fopen("arama_sonuc2.txt","r");
    if(kontrol==NULL){
        printf("\naradik, bulamadik.\n\n");
    }
    else if(kontrol!=NULL){
    fclose(kontrol);

    if(a==1) {
        do {
            fflush(stdin);
            printf("bu kisiler arama dosyasina kaydedilsin mi?\n(1:evet,0:hayir):");
            scanf("%d",&kayit);
        } while(kayit!=0&&kayit!=1);
    }
    if(kayit==0) {
        remove("arama_sonuc2.txt");
    }
    else if(kayit==1){
        arama_dosya_yaz();
        remove("arama_sonuc2.txt");
    }
    }
}
void arama_dosya_yaz(){

    FILE *arama_dosya=fopen("arama_sonuc.txt","w");
    if(arama_dosya==NULL){
        printf("arama dosyasi acilmadi!\n");
        exit(7);
    }
    FILE *arama_dosya2=fopen("arama_sonuc2.txt","r");
    if(arama_dosya2==NULL){
        printf("arama dosyasi acilmadi!\n");
        exit(8);
    }

        char ch_k;
        while( ( ch_k = fgetc(arama_dosya2) ) != EOF ){
        fputc(ch_k, arama_dosya);
        }
        fprintf(arama_dosya,"\n");

    fclose(arama_dosya2);
    fclose(arama_dosya);
}
void arama_istegi(struct aranan_kisi **aranan){
    struct aranan_kisi *ekle=malloc(sizeof(struct aranan_kisi));
    ekle->dil=NULL;
    ekle->ehliyet=NULL;

    fflush(stdin);
    printf("\naramak istediginiz kisinin:\n");
    printf("min deneyimi:");
    scanf("%d",&(ekle->deneyim));

    printf("egitim turu:");
    fgetsss(20,ekle->egitim);

    fflush(stdin);
    printf("maksimum yasi:");
    scanf("%d",&(ekle->maks_yasi));

    ehliyet_ekle(&(ekle->ehliyet));
    dil_ekle(&(ekle->dil));

    int giris_secimi;
    do{
        fflush(stdin);
        printf("0:\tbitir\n1:\tehliyet ekle\n2:\tdil ekle\nsecim:");
        scanf("%d",&giris_secimi);
        switch(giris_secimi){
            case 0:{ giris_secimi=0;
                break;
            }
            case 1:{ ehliyet_ekle(&(ekle->ehliyet));
                    system("cls");
                break;
            }
            case 2:{ dil_ekle(&(ekle->dil));
                     system("cls");
                break;
            }
            default:{ printf("lutfen gecerli secenek giriniz.");
                break;
            }
        }
    }while(giris_secimi!=0);


    *aranan=ekle;

}
void arama_istegi_kontrol(const struct kisi *kok,const struct aranan_kisi *aranan){
    if(kok!=NULL){
        arama_istegi_kontrol(kok->sol,aranan);
        int bayrak=0;
        int bayrak2=0;
        int bayrak3=0;
        int bayrak5=0;
        int bayrak6=0;
        struct is_bilgi *e;
        int toplam=0;
        for(e=kok->is;e!=NULL;e=e->sonraki){
            toplam=toplam + e->sure;
        }
        if(toplam>=aranan->deneyim){
            bayrak=1;
        }
        int say2=0;
        struct ehliyet_bilgi *i;
        struct ehliyet_bilgi *j;

        for(j=aranan->ehliyet;j!=NULL;j=j->sonraki){
            say2++;
            for(i=kok->ehliyet;i!=NULL;i=i->sonraki){
                if(strcmp(i->tip,j->tip)==0){
                    bayrak2++;
                }

            }
        }
        struct okul_bilgi *f;

        for(f=kok->okul;f!=NULL;f=f->sonraki){
            if(strcmp(f->tur,aranan->egitim)==0){
                bayrak3=1;
                break;
            }
        }

        struct dil_bilgi *x;
        struct dil_bilgi *y;
        int say5=0;
        for(y=aranan->dil;y!=NULL;y=y->sonraki){
            say5++;
            for(x=kok->dil;x!=NULL;x=x->sonraki){
                if(strcmp(x->adi,y->adi)==0){
                    bayrak5++;
                }
            }
        }
        if(aranan->maks_yasi>= 2015-atoi(kok->dogum_tarihi)){
            bayrak6=1;
        }

        if(bayrak==1&&bayrak2==say2&&bayrak3==1&&bayrak5==say5&&bayrak6==1){
            kisi_yaz(kok);
        }

        arama_istegi_kontrol(kok->sag,aranan);
    }
}
void arama_dosya_yaz_calisma_zamani(const struct kisi *dugum){

    FILE *arama_dosya=fopen("arama_sonuc2.txt","a");
    if(arama_dosya==NULL){
        printf("arama dosyasi acilmadi!\n");
        exit(7);
    }
    fprintf(arama_dosya,"%s,%s,%s,%s,%s\n",dugum->ad,dugum->adres,dugum->telefon,dugum->dogum_tarihi,dugum->mail);
            fprintf(arama_dosya,"ehliyet:");
            struct ehliyet_bilgi *c;
            for(c=dugum->ehliyet; c!=NULL; c=c->sonraki) {
                fprintf(arama_dosya,"%s,",c->tip);
            }
            fprintf(arama_dosya,"\n");
            fprintf(arama_dosya,"dil:");
              struct dil_bilgi *e;
                for(e=dugum->dil ; e!=NULL ; e=e->sonraki){
                    fprintf(arama_dosya,"%s,",e->adi);
                }
            fprintf(arama_dosya,"\n\n");
            fprintf(arama_dosya,"okul:\n");
                struct okul_bilgi *b;
                for(b=dugum->okul;b!=NULL;b=b->sonraki){
                        fprintf(arama_dosya,"%s,%s,%s,%d,%d,%d\n",b->adi,b->bolum,b->tur,b->not_ortalamasi,b->baslangic,b->bitis);
                }
            fprintf(arama_dosya,"\n");
            fprintf(arama_dosya,"is:\n");

            struct is_bilgi *a;
            for(a=dugum->is; a!=NULL; a=a->sonraki) {
                    fprintf(arama_dosya,"%s,%s,%s,%0.1f\n",a->adi,a->adres,a->pozisyon,a->sure);
            }
        fprintf(arama_dosya,"\n------------------------------------------\n");

    fclose(arama_dosya);
}

void ad_ara(struct kisi *kok,const char aranan_ad[30]){
static int a=0;

    if(kok!=NULL){
        if(strcmp(kok->ad,aranan_ad)==0){
            kisi_yaz(kok);
            a++;
        }else if(strcmp(kok->ad,aranan_ad)>0){
                ad_ara(kok->sol,aranan_ad);
        }else{
                ad_ara(kok->sag,aranan_ad);
        }
    }
    else{
        if(a==0){
            printf("kisi bulunamadi\n\n");
        }
    }

}
void ing_bilen_ara(struct kisi *kok){
    if(kok!=NULL){
        ing_bilen_ara(kok->sol);
        struct dil_bilgi *e;
        for(e=kok->dil;e!=NULL;e=e->sonraki){
            if(strcmp(e->adi,"ingilizce")==0){
                kisi_yaz(kok);
            }
        }
        ing_bilen_ara(kok->sag);
    }
}
void fazla_dil_bilen_ara(struct kisi *kok){
    if(kok!=NULL){
        fazla_dil_bilen_ara(kok->sol);
        struct dil_bilgi *e;
        int sayac=0;
        for(e=kok->dil;e!=NULL;e=e->sonraki){
            sayac++;
        }
        if(sayac>=2){
            kisi_yaz(kok);
        }
        fazla_dil_bilen_ara(kok->sag);
    }
}
void deneyim_ara(struct kisi *kok,const int deneyim){

    if(kok!=NULL){
        deneyim_ara(kok->sol,deneyim);
        struct is_bilgi *e;
        int toplam=0;
        for(e=kok->is;e!=NULL;e=e->sonraki){
            toplam=toplam + e->sure;
        }
        if(deneyim<=toplam){
            kisi_yaz(kok); //
        }
        deneyim_ara(kok->sag,deneyim);
    }
}
void yas_kisi_ara(struct kisi *kok,const int maks){
    if(kok!=NULL){
        yas_kisi_ara(kok->sol,maks);
        if(maks>=(2015-atoi(kok->dogum_tarihi))){
            kisi_yaz(kok); //
        }
        yas_kisi_ara(kok->sag,maks);
    }
}

void ehliyet_ara(struct kisi *kok,const char tip[5]){

    if(kok!=NULL){
        ehliyet_ara(kok->sol,tip);
        struct ehliyet_bilgi *e;
        for(e=kok->ehliyet;e!=NULL;e=e->sonraki){
            if(strcmp(e->tip,tip)==0){
                kisi_yaz(kok);
            }
        }
        ehliyet_ara(kok->sag,tip);
    }
}

void en_az_lisans_kisi_ara(struct kisi *kok){
     if(kok!=NULL){
        en_az_lisans_kisi_ara(kok->sol);
        struct okul_bilgi *e;
        for(e=kok->okul;e!=NULL;e=e->sonraki){
            if((strcmp(e->tur,"lisans")==0)||(strcmp(e->tur,"doktora")==0)||(strcmp(e->tur,"yuksek lisans")==0)||(strcmp(e->tur,"yüksek lisans")==0)){
                kisi_yaz(kok);
                break;
            }
        }
        en_az_lisans_kisi_ara(kok->sag);
    }
}
void deneyimsiz_ara(struct kisi *kok){

    if(kok!=NULL){
        deneyimsiz_ara(kok->sol);
        if(kok->is==NULL||kok->is->sure==0){
            kisi_yaz(kok);
        }
        deneyimsiz_ara(kok->sag);
    }
}
