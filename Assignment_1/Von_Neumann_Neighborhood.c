#include <stdio.h>
#include <stdlib.h>
#define MAX 1000

int *VonNeumannNeighborhood(int**,int);//Von Neumann's Neighborhood kuralýna göre matris oluþturan fonksiyon
void printmatrix(int**,int);//Matris elemanlarýný ekrana yazdýran fonksiyon
int **MatrisYerAc(int);//Dynamic Memory Allocation ile matrise hafýzada yer açan fonksiyon

int main() {

	int N;//Kullanýcý tarafýndan girilecek deðerin saklandýðý deðiþken
	int i;
	int j;
	int **matris;
	int matrisboyutu;//Kullanýcý tarafýndan girilen deðere göre oluþturulan matrisin boyutu
	int *sayac;//Her satýrda kaç siyah hücre olduðunu tutan dizi

	printf("N degerini giriniz: ");
	scanf("%d", &N);
	
	matrisboyutu = 2*N+3;//Kullanýcý tarafýndan girilen N deðerine göre matris boyutunun hesaplanmasý
	
	sayac = (int *)calloc(matrisboyutu, sizeof(int));
	
	matris = MatrisYerAc(matrisboyutu);
		
	sayac = VonNeumannNeighborhood(matris,N);
	
	printf("\n");
	printmatrix(matris,matrisboyutu);
	printf("\n");
	
	for (i = 0; i < matrisboyutu; i++)
		printf("%d. satirdaki siyah kare sayisi: %d\n", i, sayac[i]);
	
	printf("\nToplam siyah hucre sayisi: %d", (2*N*(N+1)) + 1 );//Matriste bulunan toplam siyah hücre sayýsýnýn ekrana yazdýrýlmasý 
	
	return 0; 	
}

int **MatrisYerAc(int size){
	
	int i;
	int **temp;//Geçici matris
	
	temp = (int **) calloc(size, sizeof(int*));
	
	for(i = 0; i < size; i++)
		temp[i] = (int *) calloc(size, sizeof(int));
		
	return temp;
}

int *VonNeumannNeighborhood(int **matris, int N){
	
	int i;
	int j;
	int *count;//Her satýrdaki siyah hücre sayýsýný tutacak olan dizi
	int matrisboyutu;
	
	matrisboyutu = 2*N+3;//N deðerine göre matrisboyutunun hesaplanmasý
	
	count = (int *)calloc(matrisboyutu, sizeof(int));
/* “Von Neumann’s Neighborhood” kuralýna göre matristeki siyah hücrelerin satýr numarasý ile matrisin ortadaki elemanýn satýr numarasý 
arasýndaki farkýn  mutlak deðeri ile sütun numaralarý arasýndaki farkýn mutlak deðerinin toplamý N deðerine küçük eþit olmalýdýr*/
	for (i = 0; i < matrisboyutu; i++)//Bu kuralý saðlayan matris elemanlarýnýn deðerinin 1 yapýlmasý
		for (j = 0; j < matrisboyutu; j++)	
			if (abs(i-N-1)+abs(j-N-1) <= N){
				matris[i][j] = 1;
				count[i]++;//Her bir satýrdaki siyah hücre sayýsýnýn hesaplanmasý
			}
	
	return count;
}

void printmatrix(int **matris, int size){
	
	int i;
	int j;
	
	for (i = 0; i < size; i++){
		for (j = 0; j < size; j++){
			printf("%d ", matris[i][j]);
		}
		printf("\n");
	}
}
