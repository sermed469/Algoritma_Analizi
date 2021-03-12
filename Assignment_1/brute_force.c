#include <stdio.h>
#include <math.h>
#define MAX 1000

void EnYakinBul(int*,int);

int main(){
	
	int N;//Dizinin eleman sayýsý
	int i;
	int dizi[MAX];//dizi
	
	printf("Dizinin eleman sayisini giriniz: ");
	scanf("%d", &N);
	
	for (i = 0; i < N; i++){
		
		printf("Dizinin %d. elemanini giriniz: ", i+1);
		scanf("%d", &dizi[i]);
	}
	
	EnYakinBul(dizi,N);
	
	return 0;
}

void EnYakinBul(int dizi[], int N){
	
	int i;
	int j;
	int min;//Elemanlar arasýndaki farkýn tutulacaðý deðiþken
	int sayi1;//Deðer olarak en yakýn elemanlardan birincisini tutacak olan deðiþken
	int sayi2;//Deðer olarak en yakýn elemanlardan ikincisini tutacak olan deðiþken
	
	min = MAX;//min deðiþkenine baþlangýç deðeri olarak büyük bir sayý atanýr
	for (i = 0; i < N; i++){//Dizinin herbir elemaný ile kendisinden sonraki elemanlar arasýndaki fark bulunarak bu farkýn en küçük olup olmadýðý kontrol edilir
		
		for (j = i+1; j < N; j++ ){
			
			if (abs(dizi[i] - dizi[j]) < min){
				
				sayi1 = dizi[i];
				sayi2 = dizi[j];
				min = abs(dizi[i] - dizi[j]);
			}
		}
	}
	
	printf("Dizide birbirine en yakin iki deger: %d %d", sayi1, sayi2);
}
