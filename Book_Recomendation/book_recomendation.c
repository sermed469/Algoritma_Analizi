#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX 10000

void Similarity(int **, int **, double **, int, int, int); //NU okuyucular�n�n U okuyucular� ile aralar�ndaki benzerli�i hesaplayan fonksiyon
int ortakkitapbul(int **, int **, int, int, int, int *); //NU ve U okuyucular�n�n her ikisinin de okudu�u kitaplar� bulan fonksiyon
void EnBenzerBul(double **, char *, int, int, int *); //�stenilen okuyucuya en benzer k okuyucuyu bulan fonksiyon
void KitapOner(int *, char*, double **, int **, int **, int, int, int, char **,int); //Okuyucuya kitap �neren fonksiyon

int main(){
	
	FILE *cs; //csv dosyas�ndaki verilerin al�nmas� i�in kullan�lacak File pointer
	char c; //Dosyadan okunan her karakterin sakland��� de�i�ken
	char str[MAX] = {'\0'}; //Dosyadaki verilerin saklanaca�� string
	char temp[100] = {'\0'};
	int i;
	int j;
	int k;
	int l;
	int kitapsayisi; //kitap say�s�n� tutacak olan de�i�ken
	int **U; //U okuyucular�n� ve her bir kitaba verdikleri puan� g�steren matris
	int **NU; //NU okuyucular�n� ve her bir kitaba verdikleri puan� g�steren matris
	int u; //U okuyucular�n�n say�s�
	int nu; //NU okuyucular�n�n say�s�
	int ucount; //U okuyucular�na eri�im i�in kullan�lacak indis
	int nucount; //NU okuyucular�na eri�im i�in kullan�lacak indis	
	int kitap; //kitaplara eri�im i�in kullan�lacak indis
	double **sim; //NU ve U okuyucular� aras�ndaki benzerlik de�erlerinin tutulaca�� matris
	char **kitaplar; //kitaplar�n tutulaca�� string dizisi
	char kullanici[4]; //Kitap �nerilecek okuyucunun ad�n�n saklanaca�� de�i�ken
	int sayi; //En �ok benzer ka� ki�inin sakland��� de�i�ken
	int *benzer; //�lgili okuyucuya en �ok benzer k ki�ininin tutulaca�� dizi
	
	cs = fopen("RecomendationDataSet.csv", "r");
	
	i = 0;
	kitapsayisi = 0;
	while(!feof(cs) && c != '\n'){//�lk sat�rdaki kitap isimlerinin okunmas�
		
		c = getc(cs);
		str[i] = c;
		i++;
		
		if ( c == ';') //kitaplar noktal� virg�l ile ayr�ld��� i�in kitap sayisi hesaplan�r
			kitapsayisi++;
	}
	
	kitaplar = (char **)calloc(kitapsayisi,sizeof(char *));//Dosyadaki kitap isimlerinin saklanmas� i�in haf�zada dinamik olarak yer a��lmas�
	for (i = 0; i < kitapsayisi; i++)
		kitaplar[i] = (char *)calloc(100,sizeof(char));
	
	i = 0;
	j = 0;
	k = 0;
	
	while(str[i] != ';'){//�lk sat�rda yazan USERS kelimesinin atlanmas�
		
		i++;
	}
	i++;
	while(str[i] != '\n'){
			
		if (str[i] == ';'){
			
			strcpy(kitaplar[j],temp);//Kitap ismi kitaplar[] dizisine aktar�l�r
			j++;
			k = 0;
			i++;
			
			for(l = 0; l < 100; l++){//temp dizisindeki eski de�erin temizlenmesi
				temp[l] = '\0';
			}
		}
		temp[k] = str[i]; //Kitaptaki harflerin tek tek temp[] dizisine atanmas�
		k++;
		i++;
	}
	strcpy(kitaplar[j],temp);//Son kitab�n isminin kitaplar[] dizisine atanmas�

	i = 0;
	u = 1;
	nu = 0;
	while(!feof(cs)){//Dosyadaki 1. sat�rdan sonraki bilgilerin str[] dizisine atanmas�
		
		c = getc(cs);
		
		if(c == 'U' && str[i-1] == '\n')//Yeni sat�ra ge�ildi�inde
			u++; //U okuyucular�n�n say�lmas�
		if(c == 'N' && str[i-1] == '\n')//Yeni sat�ra ge�ildi�inde
			nu++; //NU okuyucular�n�n say�lmas�
		
		str[i] = c;
		i++;
		
	}
	
	U = (int **)calloc(u, sizeof(int *)); //U okuyucular� ve kitaplara verdikleri oylar�n saklanaca�� U matrisi i�in haf�zada yer a��lmas�
	for (i = 0; i < u; i++)
		U[i] = (int *)calloc(kitapsayisi, sizeof(int));
	
	NU = (int **)calloc(nu, sizeof(int *)); //NU okuyucular� ve kitaplara verdikleri oylar�n saklanaca�� U matrisi i�in haf�zada yer a��lmas�
	for (i = 0; i < nu; i++)
		NU[i] = (int *)calloc(kitapsayisi, sizeof(int));
		
	sim = (double **)calloc(nu, sizeof(double *)); //NU ve U okuyucular� aras�ndaki benzerlik de�erlerinin tutulaca�� sim matrisi i�in haf�zada yer a��lmas�
	for (i = 0; i < nu; i++)
		sim[i] = (double *)calloc(u, sizeof(double));
	
	i = 0;
	kitap = 0;
	ucount = 0;
	while(str[i] != 'N'){ //U okuyucular� i�in
		
		if(str[i] == '\n'){//Bir sonraki sat�ra ge�ildiyse
			kitap = 0;//kitap indisini s�f�rla
			ucount++;//U okuyucular� i�in indisi 1 art�r
		}
		
		if(str[i] == ';'){//Bir sonraki kitaba ge�ildiyse
			
			if(str[i+1] != ' ' && str[i+1] != ';' && str[i+1] != '\n'){//E�er okuyucunun okudu�u bir kitap ise
				
				U[ucount][kitap] = str[i+1] - '0';//okuyucunun verdi�i puan say�sal formata �evrilir
			}
			else{//Okumad��� kitap i�in puan 0 yap�l�r
				
				U[ucount][kitap] = 0;
			}
			
			kitap++;//Bir sonraki kitaba ge�ilir
		}
		
		i++;
	}
	
	kitap = 0;
	nucount = 0;
	while(str[i] != '\0'){//NU okuyucular� i�in
		
		if(str[i] == '\n'){
			kitap = 0;
			nucount++;
		}
		
		if(str[i] == ';'){//Bir sonraki kitaba ge�ildiyse
		
			if(str[i+1] != ' ' && str[i+1] != ';' && str[i+1] != '\n'){//E�er okuyucunun okudu�u bir kitap ise
				
				NU[nucount][kitap] = str[i+1] - '0';//okuyucunun verdi�i puan say�sal formata �evrilir
			}
			else{//Okumad��� kitap i�in puan 0 yap�l�r
				
				NU[nucount][kitap] = 0;
			}
			
			kitap++;//Bir sonraki kitaba ge�ilir
		}
		
		i++;
	}
	
	Similarity(U,NU,sim,u,nu,kitapsayisi);//U ve NU okuyucular� aras�ndaki benzerlik de�erleri hesaplan�r
			
	printf("Kitap onerisi yapilacak kullanicinin adini giriniz: ");
	scanf("%s", &kullanici);
	printf("Benzer kullanici sayisini giriniz: ");
	scanf("%d", &sayi);
	
	benzer = (int *)calloc(sayi,sizeof(int));
	
	EnBenzerBul(sim,kullanici,sayi,u,benzer);//En benzer okuyucular�n bulunmas�
	k = kullanici[2] - '0' - 1;
	for(i = 0; i < sayi; i++)//En benzer okuyucular�n g�sterilmesi
		printf("U%d : %.3f\n", benzer[i] + 1, sim[k][benzer[i]]);
		
	KitapOner(benzer,kullanici,sim,U,NU,u,nu,sayi,kitaplar,kitapsayisi);
	
	return 0;
}

void Similarity(int **U, int **NU, double **sim, int u, int nu, int kitapsayisi){
	
	int i;
	int j;
	int *ortak; //ortak okunan kitaplar�n saklanaca�� dizi
	int k;
	int sayac;
	double ra;//NU okuyucusunun okudu�u kitaplara verdi�i puan ortalamas�
	double rb;//U okuyucusunun okudu�u kitaplara verdi�i puan ortalamas�
	int suma;// NU okuyucusunun verdi�i puanlar�n toplam�n� tutacak de�i�ken
	int sumb;// U okuyucusunun verdi�i puanlar�n toplam�n� tutacak de�i�ken
	int count;// Nu ve U okuyucusunun ortak okudu�u kitaplar�n say�s�
	double sum1;
	double sum2;
	double sum3;
	
	ortak = (int *)calloc(kitapsayisi, sizeof(int));

	for (i = 0; i < nu; i++){//Her bir NU okuyucusu U okuyucular� ile kar��la�t�r�l�r
		
		for (j = 0; j < u; j++){
			
			count = 0;
			suma = 0;
			sumb = 0;
			sum1 = 0;
			sum2 = 0;
			sum3 = 0;
			
			for (sayac = 0; sayac < kitapsayisi; sayac++) 
				ortak[i] = -1;
			
			count = ortakkitapbul(U,NU,i,j,kitapsayisi,ortak); //U ve NU okuyucular�n�n ortak okudu�u kitaplar�n bulunmas�
			
			k = 0;
			while(k < count){//NU ve U okuyucular�n�n ortak okuduklar� kitaplara verdikleri oylar�n toplanmas�
				
				suma += NU[i][ortak[k]];
				sumb += U[j][ortak[k]];
				k++;
			}
			
			ra = (double)suma / count; 
			rb = (double)sumb / count;
			
			for (k = 0; k < count; k++){
				
				sum1 += (NU[i][ortak[k]] - ra) * (U[j][ortak[k]] - rb);
				sum2 += pow(NU[i][ortak[k]] - ra, 2);
				sum3 += pow(U[j][ortak[k]] - rb, 2);
			}
			
			sim[i][j] = sum1 / (sqrt(sum2) * sqrt(sum3)); //�lgili NU okuyucusunun U okuyucusu ile aralar�ndaki benzerli�in hesaplanarak matrise atanmas�
		}
	}
}

int ortakkitapbul(int **U, int **NU, int i, int j, int kitapsayisi, int *ortak){
	
	int k;
	int count;//Ortak okunan kitap say�s�
	
	count = 0;
	for (k = 0; k < kitapsayisi; k++){//Her bir kitap i�in 
		
		if (NU[i][k] != 0 && U[j][k] != 0){//E�er NU ve U okuyucusunun ikisi de ilgili kitab� okumu�sa
			
			ortak[count] = k;
			count++;	
		}
	}
	
	return count;
}

void EnBenzerBul(double **sim, char kullanici[], int sayi, int u, int *benzer){

	int k;
	int i;
	int j;
	double max; //maksimum benzerlik de�erini tutacak de�i�ken
	int indeks;
	double U[u]; //NU okuyucusunun U okuyucular� ile aralar�ndaki benzerlik de�erlerinin saklanaca�� dizi
	double temp;
	
	k = kullanici[2] - '0';//NU1 i�in 1 say�s� al�n�r
	k = k - 1;//1 numaral� okuyucu 0 indisinden ba�lar

	for(i = 0; i < u; i++)//�lgili NU okuyucusunun b�t�n U okuyucular� ile aralar�ndaki benzerlek de�erlerin U[] dizisine atanmas�
		U[i] = sim[k][i];

	for (i = 0; i < sayi; i++){//En benzer k ki�i bulunurken �ncelikle en b�y�k benzerlik bulunur daha sonra en b�y�k ikinci benzerlik bulunur... en b�y�k k. benzerlik bulunur
		
		max = -1.0;
		for(j = 0; j < u; j++){//En b�y�k benzerlik de�erinin bulunmas�
			
			if(U[j] > max){
				
				max = U[j];
				indeks = j;
			}
		}
		
		benzer[i] = indeks;//�lgili U okuyucusunun indisi benzer[] dizisine atan�r
		U[indeks] = -1.0;
	}
	
}

void KitapOner(int *benzer,char kullanici[], double **sim,int **U,int **NU,int u,int nu,int sayi,char **kitaplar,int kitapsayisi){
	
	int *okunmayan;//NU okuyucusu taraf�ndan okunmayan kitaplar�n saklanaca�� dizi
	int k;
	int i;
	int j;
	int l;
	int counta_okunmayan;//NU okuyucusunun okumad��� kitaplar�n say�s�n�n tutuldu�u de�i�ken
	int countb_okunmayan;//U okuyucusunun okumad��� kitaplar�n say�s�n�n tutuldu�u de�i�ken
	int maxindex;
	double *pred;
	double ra;//NU okuyucusunun okudu�u kitaplara verdi�i puan ortalamas�
	double rb;//U okuyucusunun okudu�u kitaplara verdi�i puan ortalamas�
	double sum_a;//NU okuyucusunun verdi�i puanlar�n toplam�n� tutacak de�i�ken
	double pay; //pred hesab� yap�l�rken pay�n tutulaca�� de�i�ken
	double payda;// pred hesab� yap�l�rken paydan�n tutulaca�� de�i�ken
	double sum_b;//U okuyucusunun verdi�i puanlar�n toplam�n� tutacak de�i�ken
	
	counta_okunmayan = 0;
	k = kullanici[2] - '0' - 1;
	okunmayan = (int *)calloc(10,sizeof(int));

	sum_a = 0;
	for(i = 0; i < kitapsayisi; i++){
		
		if(NU[k][i] == 0){//Okumad��� kitaplara 0 puan� verildi�i i�in 
			
			okunmayan[counta_okunmayan] = i;//Okunmayan kitab�n indisi okunmayan[] dizisine aktar�l�r
			counta_okunmayan++;
 		}
 		
 		sum_a += NU[k][i];//Okunan kitaplara verilen puanlar toplan�r
	}
	
	pred = (double *)calloc(counta_okunmayan,sizeof(double));
	
	ra = (double)sum_a / (kitapsayisi - counta_okunmayan);

	for(i = 0; i < counta_okunmayan; i++){//Okunmayan her bir kitap i�in
		
		pay = 0;
		payda = 0;
		
		for (j = 0; j < sayi; j++){//En benzer k okuyucu
			
			sum_b = 0;
			countb_okunmayan = 0;
			for(l = 0; l < kitapsayisi; l++){//�lgili U okuyucusunun okudu�u kitaplara verdi�i oylar�n toplanmas�
				
				sum_b += U[benzer[j]][l];
				
				if (U[benzer[j]][l] == 0){//U okuyucusunun okunmad��� kitap say�s�n�n hesaplanmas�
					
					countb_okunmayan++;
				}
			}
			rb = (double)sum_b / (kitapsayisi - countb_okunmayan);
			
			pay += sim[k][benzer[j]] * (U[benzer[j]][okunmayan[i]] - rb);
			payda += sim[k][benzer[j]];
		}
		
		pred[i] = ra + (pay / payda); //�lgili kitap i�in hesaplanan pred de�eri pred[] dizisine atan�r
	}
	printf("\n");
	for (i = 0; i < counta_okunmayan; i++){//Okunmayan her kitap i�in pred de�erleri ekrana yazd�r�l�r
		
		printf("%s : %.4f\n", kitaplar[okunmayan[i]],pred[i]);
	}
	
	maxindex = 0;
	for(i = 1; i < counta_okunmayan; i++)//En b�y�k pred de�eri bulunur
		if(pred[i] > pred[maxindex])
			maxindex = i;
	
	printf("\nOnerilen kitap : %s\n", kitaplar[okunmayan[maxindex]]);
}
