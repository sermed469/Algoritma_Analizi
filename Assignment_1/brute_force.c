#include <stdio.h>
#include <math.h>
#define MAX 1000

void EnYakinBul(int*,int);

int main(){
	
	int N;//Dizinin eleman say�s�
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
	int min;//Elemanlar aras�ndaki fark�n tutulaca�� de�i�ken
	int sayi1;//De�er olarak en yak�n elemanlardan birincisini tutacak olan de�i�ken
	int sayi2;//De�er olarak en yak�n elemanlardan ikincisini tutacak olan de�i�ken
	
	min = MAX;//min de�i�kenine ba�lang�� de�eri olarak b�y�k bir say� atan�r
	for (i = 0; i < N; i++){//Dizinin herbir eleman� ile kendisinden sonraki elemanlar aras�ndaki fark bulunarak bu fark�n en k���k olup olmad��� kontrol edilir
		
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
