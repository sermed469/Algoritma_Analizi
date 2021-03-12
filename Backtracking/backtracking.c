#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void MatrisDonustur(char ***, int **, int, char (*)[8]);//Renk matrisinindeki renkleri sayýsal karþýlaklarýna dönüþtürüp matris oluþturan fonksiyon
int RenkBul(char *, char (*)[8]);//renklerin saklandýðý dizide renk aramasý yapan fonksiyon
int BackTrack(int **, int, int, char (*)[8]);//Geri Ýzleme yöntemi ile Renk matrisini her sütunda ayný renkten bþr bulunacak þekilde rekürsif olarak düzenleyen fonksiyon
void SagaKaydir(int **, int, int);//Renk matrisindeki ilgili satýrý bir adým saða kaydýran fonksiyon
int Kontrol(int **, int, int, char (*)[8]);//Ýlgili satýrdaki her bir sütunda bulunan renkleri kendisinin üstünde bulunan renkler ile karþýlaþtýran fonksiyon

int main(){
	
	int N;//Renk Sayisi
	int i;//Döngü deðiþkeni
	int j;//Döngü deðiþkeni
	int **M;//Kullanýcý tarafýndan girilen renk matrisindeki renklerin sayýsal karþýlýklarýnýn tutulduðu matris
	char renk[8][8] = {{"Mavi"},{"Kirmizi"},{"Yesil"},{"Turuncu"},{"Mor"},{"Sari"},{"Siyah"},{"Beyaz"}};//Renklerin tutulduðu matris
	char ***matris;//Kullanýcý tarafýndan girilecek renk matrisi
	
	printf("Giriceginiz renkleri asagidaki yazilan formatta giriniz.\n");
	for (i = 0; i < 8; i++)
		printf("%s ", renk[i]);
		
	do{	
		printf("\n\nRenk sayisini giriniz: ");
		scanf("%d", &N);
	
	}while(N < 3 || N > 8 );//Renk sayýsý minimum 3 maksimum 8 olmalý
	
	matris = (char ***)calloc(N,sizeof(char **));//Renk Matrisi için dinamik olarak hafýzada yer açýlmasý
	M = (int **)calloc(N, sizeof(int *));//Renk matrisinin sayýsal karþýlýðýnýn bulunacaðý matris için yer açýlmasý
	
	for(i = 0; i < N; i++){
		
		matris[i] = (char **)calloc(N,sizeof(char*));
		M[i] = (int *)calloc(N, sizeof(int));
	}
				
	for (i = 0; i < N; i++)
		for(j = 0; j < N; j++)
			matris[i][j] = (char *)calloc(8,sizeof(char));//Renk matrisinde en uzun renk 8 harfli olduðu için 8 karakterlik yer açýlýr
	
	printf("Renk matrisini giriniz: \n");
	for (i = 0; i < N; i++)
		for(j = 0; j < N; j++)
			scanf("%s", matris[i][j]);

	MatrisDonustur(matris,M,N,renk);

	if(BackTrack(M,0,N,renk)){//Eðer BackTrack() fonksiyonu 1 deðerini döndürürse istenen matris oluþturulmuþtur
		
		printf("\n\nDuzenlenmis Renk Matrisi:\n\n");
		
		for (i = 0; i < N; i++){
		
			for(j = 0; j < N; j++){
			
				printf("%s\t", renk[M[i][j]]);
			}
		
			printf("\n");
		}
	}else{//Eðer BackTrack() fonksiyonu 0 deðerini döndürürse 
		
		printf("\nMATRIS OLUSTURULAMADI\n");
	}

	return 0;
}

void MatrisDonustur(char ***matris, int **M, int N, char renk[][8]){
	
	int i;
	int j;
	
	for(i = 0; i < N; i++){//Kullanýcý tarafýndan girilen matristeki renkler için sayýsal deðerler renk[][] String dizisindeki her bir rengin indisi olarak belirlenir
		
		for(j = 0; j < N; j++){
			
			M[i][j] = RenkBul(matris[i][j],renk);//Ýlgili renk renklerin bulunduðu dizide aranýr ve indis deðeri döndürülür
		}
	}
}

int RenkBul(char color[], char renk[][8]){

	int i;
	
	for(i = 0; i < 8; i++){//Aranan Renk dizide bulunana kadar karþýlaþtýrýlýr
		
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
		for(s1 = 0; s1 < N; s1++){//Matriste bulunan satýrlarýn üstteki satýrlara göre düzgün yerleþtiði zaman ekrana yazdýrýlmasý
			for(s2 = 0; s2 < N; s2++){
				printf("%s\t", renk[M[s1][s2]]);
			}
			printf("\n");
		}
	}
	
	if(i == N){//Eðer Renk matrisi her sütundaki renkler farklý olacak þekilde düzenlendiyse
		
		return 1;
	}
	else{
		
		for(j = 0; j < N; j++){//Ýlgili satýrýn saða kaydýrýldýðýnda her bir kombinasyon için
			
			if(Kontrol(M,i,N,renk)){//Renk matrisinde ilgili satýrdan yukarýya doðru kontrol yapýlýr
					
				if(BackTrack(M,i+1,N,renk)){//Eðer matriste o satýra kadar olan kýsým kurala uydunsa bir sonraki satýra geçilir
						
					return;                   				
				}
			}
			else{//Eðer ilgili satýrda ayný sütunda ayný renk birden fazla kez yer alýyorsa
				
				SagaKaydir(M,i,N);//Satýrdaki renkleri saða kaydar
			}
		}
	}
	if(i > 0)
		SagaKaydir(M,i-1,N);//Bir önceki satýrdaki renkler 1 birim saða kaydýrýlýr
		
	return 0;
}

int Kontrol(int **M, int i, int N, char renk[][8]){
	
	int j;
	int k;
	
	for(k = 0; k < i; k++){
		
		for(j = 0; j < N; j++){
			
			if(M[i][j] == M[k][j])//Ýlgili Renk kendisinin üstündeki sütun ile ayný renk ise
				return 0;
		}
	}
	//Eðer Kontrol edilen satýrdaki tüm renkler kendilerinin üstünde bulunan sütunlardaki renklerden farklý ise
	return 1;
}

void SagaKaydir(int **M, int i, int N){
	
	int j;
	int temp[N];//Saða kaydýrma iþleminde kullanýlacak geçici dizi
	//Saða kaydýrma iþleminde satýrdaki son eleman en baþa getirilir, diðer elemanlar bir birim saða kaydýrýlýr
	for(j = 0; j < N-1; j++)
		temp[j+1] = M[i][j];//Her bir elemanýn bir birim saða kaydýrlmasý
	
	temp[0] = M[i][N-1];//Satýrdaki son eleman baþa eklenir
	
	for(j = 0; j < N; j++)//Geçici dizide saklanan bir birim saða kaydýrýlmýþ satýr orjinal renk matrisine atanýr
		M[i][j] = temp[j];
}
