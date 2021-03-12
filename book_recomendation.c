#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX 10000

void Similarity(int **, int **, double **, int, int, int); //NU okuyucularýnýn U okuyucularý ile aralarýndaki benzerliði hesaplayan fonksiyon
int ortakkitapbul(int **, int **, int, int, int, int *); //NU ve U okuyucularýnýn her ikisinin de okuduðu kitaplarý bulan fonksiyon
void EnBenzerBul(double **, char *, int, int, int *); //Ýstenilen okuyucuya en benzer k okuyucuyu bulan fonksiyon
void KitapOner(int *, char*, double **, int **, int **, int, int, int, char **,int); //Okuyucuya kitap öneren fonksiyon

int main(){
	
	FILE *cs; //csv dosyasýndaki verilerin alýnmasý için kullanýlacak File pointer
	char c; //Dosyadan okunan her karakterin saklandýðý deðiþken
	char str[MAX] = {'\0'}; //Dosyadaki verilerin saklanacaðý string
	char temp[100] = {'\0'};
	int i;
	int j;
	int k;
	int l;
	int kitapsayisi; //kitap sayýsýný tutacak olan deðiþken
	int **U; //U okuyucularýný ve her bir kitaba verdikleri puaný gösteren matris
	int **NU; //NU okuyucularýný ve her bir kitaba verdikleri puaný gösteren matris
	int u; //U okuyucularýnýn sayýsý
	int nu; //NU okuyucularýnýn sayýsý
	int ucount; //U okuyucularýna eriþim için kullanýlacak indis
	int nucount; //NU okuyucularýna eriþim için kullanýlacak indis	
	int kitap; //kitaplara eriþim için kullanýlacak indis
	double **sim; //NU ve U okuyucularý arasýndaki benzerlik deðerlerinin tutulacaðý matris
	char **kitaplar; //kitaplarýn tutulacaðý string dizisi
	char kullanici[4]; //Kitap önerilecek okuyucunun adýnýn saklanacaðý deðiþken
	int sayi; //En çok benzer kaç kiþinin saklandýðý deðiþken
	int *benzer; //Ýlgili okuyucuya en çok benzer k kiþininin tutulacaðý dizi
	
	cs = fopen("RecomendationDataSet.csv", "r");
	
	i = 0;
	kitapsayisi = 0;
	while(!feof(cs) && c != '\n'){//Ýlk satýrdaki kitap isimlerinin okunmasý
		
		c = getc(cs);
		str[i] = c;
		i++;
		
		if ( c == ';') //kitaplar noktalý virgül ile ayrýldýðý için kitap sayisi hesaplanýr
			kitapsayisi++;
	}
	
	kitaplar = (char **)calloc(kitapsayisi,sizeof(char *));//Dosyadaki kitap isimlerinin saklanmasý için hafýzada dinamik olarak yer açýlmasý
	for (i = 0; i < kitapsayisi; i++)
		kitaplar[i] = (char *)calloc(100,sizeof(char));
	
	i = 0;
	j = 0;
	k = 0;
	
	while(str[i] != ';'){//Ýlk satýrda yazan USERS kelimesinin atlanmasý
		
		i++;
	}
	i++;
	while(str[i] != '\n'){
			
		if (str[i] == ';'){
			
			strcpy(kitaplar[j],temp);//Kitap ismi kitaplar[] dizisine aktarýlýr
			j++;
			k = 0;
			i++;
			
			for(l = 0; l < 100; l++){//temp dizisindeki eski deðerin temizlenmesi
				temp[l] = '\0';
			}
		}
		temp[k] = str[i]; //Kitaptaki harflerin tek tek temp[] dizisine atanmasý
		k++;
		i++;
	}
	strcpy(kitaplar[j],temp);//Son kitabýn isminin kitaplar[] dizisine atanmasý

	i = 0;
	u = 1;
	nu = 0;
	while(!feof(cs)){//Dosyadaki 1. satýrdan sonraki bilgilerin str[] dizisine atanmasý
		
		c = getc(cs);
		
		if(c == 'U' && str[i-1] == '\n')//Yeni satýra geçildiðinde
			u++; //U okuyucularýnýn sayýlmasý
		if(c == 'N' && str[i-1] == '\n')//Yeni satýra geçildiðinde
			nu++; //NU okuyucularýnýn sayýlmasý
		
		str[i] = c;
		i++;
		
	}
	
	U = (int **)calloc(u, sizeof(int *)); //U okuyucularý ve kitaplara verdikleri oylarýn saklanacaðý U matrisi için hafýzada yer açýlmasý
	for (i = 0; i < u; i++)
		U[i] = (int *)calloc(kitapsayisi, sizeof(int));
	
	NU = (int **)calloc(nu, sizeof(int *)); //NU okuyucularý ve kitaplara verdikleri oylarýn saklanacaðý U matrisi için hafýzada yer açýlmasý
	for (i = 0; i < nu; i++)
		NU[i] = (int *)calloc(kitapsayisi, sizeof(int));
		
	sim = (double **)calloc(nu, sizeof(double *)); //NU ve U okuyucularý arasýndaki benzerlik deðerlerinin tutulacaðý sim matrisi için hafýzada yer açýlmasý
	for (i = 0; i < nu; i++)
		sim[i] = (double *)calloc(u, sizeof(double));
	
	i = 0;
	kitap = 0;
	ucount = 0;
	while(str[i] != 'N'){ //U okuyucularý için
		
		if(str[i] == '\n'){//Bir sonraki satýra geçildiyse
			kitap = 0;//kitap indisini sýfýrla
			ucount++;//U okuyucularý için indisi 1 artýr
		}
		
		if(str[i] == ';'){//Bir sonraki kitaba geçildiyse
			
			if(str[i+1] != ' ' && str[i+1] != ';' && str[i+1] != '\n'){//Eðer okuyucunun okuduðu bir kitap ise
				
				U[ucount][kitap] = str[i+1] - '0';//okuyucunun verdiði puan sayýsal formata çevrilir
			}
			else{//Okumadýðý kitap için puan 0 yapýlýr
				
				U[ucount][kitap] = 0;
			}
			
			kitap++;//Bir sonraki kitaba geçilir
		}
		
		i++;
	}
	
	kitap = 0;
	nucount = 0;
	while(str[i] != '\0'){//NU okuyucularý için
		
		if(str[i] == '\n'){
			kitap = 0;
			nucount++;
		}
		
		if(str[i] == ';'){//Bir sonraki kitaba geçildiyse
		
			if(str[i+1] != ' ' && str[i+1] != ';' && str[i+1] != '\n'){//Eðer okuyucunun okuduðu bir kitap ise
				
				NU[nucount][kitap] = str[i+1] - '0';//okuyucunun verdiði puan sayýsal formata çevrilir
			}
			else{//Okumadýðý kitap için puan 0 yapýlýr
				
				NU[nucount][kitap] = 0;
			}
			
			kitap++;//Bir sonraki kitaba geçilir
		}
		
		i++;
	}
	
	Similarity(U,NU,sim,u,nu,kitapsayisi);//U ve NU okuyucularý arasýndaki benzerlik deðerleri hesaplanýr
			
	printf("Kitap onerisi yapilacak kullanicinin adini giriniz: ");
	scanf("%s", &kullanici);
	printf("Benzer kullanici sayisini giriniz: ");
	scanf("%d", &sayi);
	
	benzer = (int *)calloc(sayi,sizeof(int));
	
	EnBenzerBul(sim,kullanici,sayi,u,benzer);//En benzer okuyucularýn bulunmasý
	k = kullanici[2] - '0' - 1;
	for(i = 0; i < sayi; i++)//En benzer okuyucularýn gösterilmesi
		printf("U%d : %.3f\n", benzer[i] + 1, sim[k][benzer[i]]);
		
	KitapOner(benzer,kullanici,sim,U,NU,u,nu,sayi,kitaplar,kitapsayisi);
	
	return 0;
}

void Similarity(int **U, int **NU, double **sim, int u, int nu, int kitapsayisi){
	
	int i;
	int j;
	int *ortak; //ortak okunan kitaplarýn saklanacaðý dizi
	int k;
	int sayac;
	double ra;//NU okuyucusunun okuduðu kitaplara verdiði puan ortalamasý
	double rb;//U okuyucusunun okuduðu kitaplara verdiði puan ortalamasý
	int suma;// NU okuyucusunun verdiði puanlarýn toplamýný tutacak deðiþken
	int sumb;// U okuyucusunun verdiði puanlarýn toplamýný tutacak deðiþken
	int count;// Nu ve U okuyucusunun ortak okuduðu kitaplarýn sayýsý
	double sum1;
	double sum2;
	double sum3;
	
	ortak = (int *)calloc(kitapsayisi, sizeof(int));

	for (i = 0; i < nu; i++){//Her bir NU okuyucusu U okuyucularý ile karþýlaþtýrýlýr
		
		for (j = 0; j < u; j++){
			
			count = 0;
			suma = 0;
			sumb = 0;
			sum1 = 0;
			sum2 = 0;
			sum3 = 0;
			
			for (sayac = 0; sayac < kitapsayisi; sayac++) 
				ortak[i] = -1;
			
			count = ortakkitapbul(U,NU,i,j,kitapsayisi,ortak); //U ve NU okuyucularýnýn ortak okuduðu kitaplarýn bulunmasý
			
			k = 0;
			while(k < count){//NU ve U okuyucularýnýn ortak okuduklarý kitaplara verdikleri oylarýn toplanmasý
				
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
			
			sim[i][j] = sum1 / (sqrt(sum2) * sqrt(sum3)); //Ýlgili NU okuyucusunun U okuyucusu ile aralarýndaki benzerliðin hesaplanarak matrise atanmasý
		}
	}
}

int ortakkitapbul(int **U, int **NU, int i, int j, int kitapsayisi, int *ortak){
	
	int k;
	int count;//Ortak okunan kitap sayýsý
	
	count = 0;
	for (k = 0; k < kitapsayisi; k++){//Her bir kitap için 
		
		if (NU[i][k] != 0 && U[j][k] != 0){//Eðer NU ve U okuyucusunun ikisi de ilgili kitabý okumuþsa
			
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
	double max; //maksimum benzerlik deðerini tutacak deðiþken
	int indeks;
	double U[u]; //NU okuyucusunun U okuyucularý ile aralarýndaki benzerlik deðerlerinin saklanacaðý dizi
	double temp;
	
	k = kullanici[2] - '0';//NU1 için 1 sayýsý alýnýr
	k = k - 1;//1 numaralý okuyucu 0 indisinden baþlar

	for(i = 0; i < u; i++)//Ýlgili NU okuyucusunun bütün U okuyucularý ile aralarýndaki benzerlek deðerlerin U[] dizisine atanmasý
		U[i] = sim[k][i];

	for (i = 0; i < sayi; i++){//En benzer k kiþi bulunurken öncelikle en büyük benzerlik bulunur daha sonra en büyük ikinci benzerlik bulunur... en büyük k. benzerlik bulunur
		
		max = -1.0;
		for(j = 0; j < u; j++){//En büyük benzerlik deðerinin bulunmasý
			
			if(U[j] > max){
				
				max = U[j];
				indeks = j;
			}
		}
		
		benzer[i] = indeks;//Ýlgili U okuyucusunun indisi benzer[] dizisine atanýr
		U[indeks] = -1.0;
	}
	
}

void KitapOner(int *benzer,char kullanici[], double **sim,int **U,int **NU,int u,int nu,int sayi,char **kitaplar,int kitapsayisi){
	
	int *okunmayan;//NU okuyucusu tarafýndan okunmayan kitaplarýn saklanacaðý dizi
	int k;
	int i;
	int j;
	int l;
	int counta_okunmayan;//NU okuyucusunun okumadýðý kitaplarýn sayýsýnýn tutulduðu deðiþken
	int countb_okunmayan;//U okuyucusunun okumadýðý kitaplarýn sayýsýnýn tutulduðu deðiþken
	int maxindex;
	double *pred;
	double ra;//NU okuyucusunun okuduðu kitaplara verdiði puan ortalamasý
	double rb;//U okuyucusunun okuduðu kitaplara verdiði puan ortalamasý
	double sum_a;//NU okuyucusunun verdiði puanlarýn toplamýný tutacak deðiþken
	double pay; //pred hesabý yapýlýrken payýn tutulacaðý deðiþken
	double payda;// pred hesabý yapýlýrken paydanýn tutulacaðý deðiþken
	double sum_b;//U okuyucusunun verdiði puanlarýn toplamýný tutacak deðiþken
	
	counta_okunmayan = 0;
	k = kullanici[2] - '0' - 1;
	okunmayan = (int *)calloc(10,sizeof(int));

	sum_a = 0;
	for(i = 0; i < kitapsayisi; i++){
		
		if(NU[k][i] == 0){//Okumadýðý kitaplara 0 puaný verildiði için 
			
			okunmayan[counta_okunmayan] = i;//Okunmayan kitabýn indisi okunmayan[] dizisine aktarýlýr
			counta_okunmayan++;
 		}
 		
 		sum_a += NU[k][i];//Okunan kitaplara verilen puanlar toplanýr
	}
	
	pred = (double *)calloc(counta_okunmayan,sizeof(double));
	
	ra = (double)sum_a / (kitapsayisi - counta_okunmayan);

	for(i = 0; i < counta_okunmayan; i++){//Okunmayan her bir kitap için
		
		pay = 0;
		payda = 0;
		
		for (j = 0; j < sayi; j++){//En benzer k okuyucu
			
			sum_b = 0;
			countb_okunmayan = 0;
			for(l = 0; l < kitapsayisi; l++){//Ýlgili U okuyucusunun okuduðu kitaplara verdiði oylarýn toplanmasý
				
				sum_b += U[benzer[j]][l];
				
				if (U[benzer[j]][l] == 0){//U okuyucusunun okunmadýðý kitap sayýsýnýn hesaplanmasý
					
					countb_okunmayan++;
				}
			}
			rb = (double)sum_b / (kitapsayisi - countb_okunmayan);
			
			pay += sim[k][benzer[j]] * (U[benzer[j]][okunmayan[i]] - rb);
			payda += sim[k][benzer[j]];
		}
		
		pred[i] = ra + (pay / payda); //Ýlgili kitap için hesaplanan pred deðeri pred[] dizisine atanýr
	}
	printf("\n");
	for (i = 0; i < counta_okunmayan; i++){//Okunmayan her kitap için pred deðerleri ekrana yazdýrýlýr
		
		printf("%s : %.4f\n", kitaplar[okunmayan[i]],pred[i]);
	}
	
	maxindex = 0;
	for(i = 1; i < counta_okunmayan; i++)//En büyük pred deðeri bulunur
		if(pred[i] > pred[maxindex])
			maxindex = i;
	
	printf("\nOnerilen kitap : %s\n", kitaplar[okunmayan[maxindex]]);
}
