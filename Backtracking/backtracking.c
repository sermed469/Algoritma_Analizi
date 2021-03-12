#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void MatrisDonustur(char ***, int **, int, char (*)[8]);//Renk matrisinindeki renkleri say�sal kar��laklar�na d�n��t�r�p matris olu�turan fonksiyon
int RenkBul(char *, char (*)[8]);//renklerin sakland��� dizide renk aramas� yapan fonksiyon
int BackTrack(int **, int, int, char (*)[8]);//Geri �zleme y�ntemi ile Renk matrisini her s�tunda ayn� renkten b�r bulunacak �ekilde rek�rsif olarak d�zenleyen fonksiyon
void SagaKaydir(int **, int, int);//Renk matrisindeki ilgili sat�r� bir ad�m sa�a kayd�ran fonksiyon
int Kontrol(int **, int, int, char (*)[8]);//�lgili sat�rdaki her bir s�tunda bulunan renkleri kendisinin �st�nde bulunan renkler ile kar��la�t�ran fonksiyon

int main(){
	
	int N;//Renk Sayisi
	int i;//D�ng� de�i�keni
	int j;//D�ng� de�i�keni
	int **M;//Kullan�c� taraf�ndan girilen renk matrisindeki renklerin say�sal kar��l�klar�n�n tutuldu�u matris
	char renk[8][8] = {{"Mavi"},{"Kirmizi"},{"Yesil"},{"Turuncu"},{"Mor"},{"Sari"},{"Siyah"},{"Beyaz"}};//Renklerin tutuldu�u matris
	char ***matris;//Kullan�c� taraf�ndan girilecek renk matrisi
	
	printf("Giriceginiz renkleri asagidaki yazilan formatta giriniz.\n");
	for (i = 0; i < 8; i++)
		printf("%s ", renk[i]);
		
	do{	
		printf("\n\nRenk sayisini giriniz: ");
		scanf("%d", &N);
	
	}while(N < 3 || N > 8 );//Renk say�s� minimum 3 maksimum 8 olmal�
	
	matris = (char ***)calloc(N,sizeof(char **));//Renk Matrisi i�in dinamik olarak haf�zada yer a��lmas�
	M = (int **)calloc(N, sizeof(int *));//Renk matrisinin say�sal kar��l���n�n bulunaca�� matris i�in yer a��lmas�
	
	for(i = 0; i < N; i++){
		
		matris[i] = (char **)calloc(N,sizeof(char*));
		M[i] = (int *)calloc(N, sizeof(int));
	}
				
	for (i = 0; i < N; i++)
		for(j = 0; j < N; j++)
			matris[i][j] = (char *)calloc(8,sizeof(char));//Renk matrisinde en uzun renk 8 harfli oldu�u i�in 8 karakterlik yer a��l�r
	
	printf("Renk matrisini giriniz: \n");
	for (i = 0; i < N; i++)
		for(j = 0; j < N; j++)
			scanf("%s", matris[i][j]);

	MatrisDonustur(matris,M,N,renk);

	if(BackTrack(M,0,N,renk)){//E�er BackTrack() fonksiyonu 1 de�erini d�nd�r�rse istenen matris olu�turulmu�tur
		
		printf("\n\nDuzenlenmis Renk Matrisi:\n\n");
		
		for (i = 0; i < N; i++){
		
			for(j = 0; j < N; j++){
			
				printf("%s\t", renk[M[i][j]]);
			}
		
			printf("\n");
		}
	}else{//E�er BackTrack() fonksiyonu 0 de�erini d�nd�r�rse 
		
		printf("\nMATRIS OLUSTURULAMADI\n");
	}

	return 0;
}

void MatrisDonustur(char ***matris, int **M, int N, char renk[][8]){
	
	int i;
	int j;
	
	for(i = 0; i < N; i++){//Kullan�c� taraf�ndan girilen matristeki renkler i�in say�sal de�erler renk[][] String dizisindeki her bir rengin indisi olarak belirlenir
		
		for(j = 0; j < N; j++){
			
			M[i][j] = RenkBul(matris[i][j],renk);//�lgili renk renklerin bulundu�u dizide aran�r ve indis de�eri d�nd�r�l�r
		}
	}
}

int RenkBul(char color[], char renk[][8]){

	int i;
	
	for(i = 0; i < 8; i++){//Aranan Renk dizide bulunana kadar kar��la�t�r�l�r
		
		if(!strcmp(color,renk[i])){
			
			return i;
		}
	}
}

int BackTrack(int **M, int i, int N, char renk[][8]){
	
	int j;
	int s1;
	int s2;
	
	if(i > 1){
		
		printf("\n%d. satir duzenlendi\n");	
		for(s1 = 0; s1 < N; s1++){//Matriste bulunan sat�rlar�n �stteki sat�rlara g�re d�zg�n yerle�ti�i zaman ekrana yazd�r�lmas�
			for(s2 = 0; s2 < N; s2++){
				printf("%s\t", renk[M[s1][s2]]);
			}
			printf("\n");
		}
	}
	
	if(i == N){//E�er Renk matrisi her s�tundaki renkler farkl� olacak �ekilde d�zenlendiyse
		
		return 1;
	}
	else{
		
		for(j = 0; j < N; j++){//�lgili sat�r�n sa�a kayd�r�ld���nda her bir kombinasyon i�in
			
			if(Kontrol(M,i,N,renk)){//Renk matrisinde ilgili sat�rdan yukar�ya do�ru kontrol yap�l�r
					
				if(BackTrack(M,i+1,N,renk)){//E�er matriste o sat�ra kadar olan k�s�m kurala uydunsa bir sonraki sat�ra ge�ilir
						
					return;                   				
				}
			}
			else{//E�er ilgili sat�rda ayn� s�tunda ayn� renk birden fazla kez yer al�yorsa
				
				SagaKaydir(M,i,N);//Sat�rdaki renkleri sa�a kaydar
			}
		}
	}
	if(i > 0)
		SagaKaydir(M,i-1,N);//Bir �nceki sat�rdaki renkler 1 birim sa�a kayd�r�l�r
		
	return 0;
}

int Kontrol(int **M, int i, int N, char renk[][8]){
	
	int j;
	int k;
	
	for(k = 0; k < i; k++){
		
		for(j = 0; j < N; j++){
			
			if(M[i][j] == M[k][j])//�lgili Renk kendisinin �st�ndeki s�tun ile ayn� renk ise
				return 0;
		}
	}
	//E�er Kontrol edilen sat�rdaki t�m renkler kendilerinin �st�nde bulunan s�tunlardaki renklerden farkl� ise
	return 1;
}

void SagaKaydir(int **M, int i, int N){
	
	int j;
	int temp[N];//Sa�a kayd�rma i�leminde kullan�lacak ge�ici dizi
	//Sa�a kayd�rma i�leminde sat�rdaki son eleman en ba�a getirilir, di�er elemanlar bir birim sa�a kayd�r�l�r
	for(j = 0; j < N-1; j++)
		temp[j+1] = M[i][j];//Her bir eleman�n bir birim sa�a kayd�rlmas�
	
	temp[0] = M[i][N-1];//Sat�rdaki son eleman ba�a eklenir
	
	for(j = 0; j < N; j++)//Ge�ici dizide saklanan bir birim sa�a kayd�r�lm�� sat�r orjinal renk matrisine atan�r
		M[i][j] = temp[j];
}
