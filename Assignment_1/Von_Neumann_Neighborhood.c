#include <stdio.h>
#include <stdlib.h>
#define MAX 1000

int *VonNeumannNeighborhood(int**,int);//Von Neumann's Neighborhood kural�na g�re matris olu�turan fonksiyon
void printmatrix(int**,int);//Matris elemanlar�n� ekrana yazd�ran fonksiyon
int **MatrisYerAc(int);//Dynamic Memory Allocation ile matrise haf�zada yer a�an fonksiyon

int main() {

	int N;//Kullan�c� taraf�ndan girilecek de�erin sakland��� de�i�ken
	int i;
	int j;
	int **matris;
	int matrisboyutu;//Kullan�c� taraf�ndan girilen de�ere g�re olu�turulan matrisin boyutu
	int *sayac;//Her sat�rda ka� siyah h�cre oldu�unu tutan dizi

	printf("N degerini giriniz: ");
	scanf("%d", &N);
	
	matrisboyutu = 2*N+3;//Kullan�c� taraf�ndan girilen N de�erine g�re matris boyutunun hesaplanmas�
	
	sayac = (int *)calloc(matrisboyutu, sizeof(int));
	
	matris = MatrisYerAc(matrisboyutu);
		
	sayac = VonNeumannNeighborhood(matris,N);
	
	printf("\n");
	printmatrix(matris,matrisboyutu);
	printf("\n");
	
	for (i = 0; i < matrisboyutu; i++)
		printf("%d. satirdaki siyah kare sayisi: %d\n", i, sayac[i]);
	
	printf("\nToplam siyah hucre sayisi: %d", (2*N*(N+1)) + 1 );//Matriste bulunan toplam siyah h�cre say�s�n�n ekrana yazd�r�lmas� 
	
	return 0; 	
}

int **MatrisYerAc(int size){
	
	int i;
	int **temp;//Ge�ici matris
	
	temp = (int **) calloc(size, sizeof(int*));
	
	for(i = 0; i < size; i++)
		temp[i] = (int *) calloc(size, sizeof(int));
		
	return temp;
}

int *VonNeumannNeighborhood(int **matris, int N){
	
	int i;
	int j;
	int *count;//Her sat�rdaki siyah h�cre say�s�n� tutacak olan dizi
	int matrisboyutu;
	
	matrisboyutu = 2*N+3;//N de�erine g�re matrisboyutunun hesaplanmas�
	
	count = (int *)calloc(matrisboyutu, sizeof(int));
/* �Von Neumann�s Neighborhood� kural�na g�re matristeki siyah h�crelerin sat�r numaras� ile matrisin ortadaki eleman�n sat�r numaras� 
aras�ndaki fark�n  mutlak de�eri ile s�tun numaralar� aras�ndaki fark�n mutlak de�erinin toplam� N de�erine k���k e�it olmal�d�r*/
	for (i = 0; i < matrisboyutu; i++)//Bu kural� sa�layan matris elemanlar�n�n de�erinin 1 yap�lmas�
		for (j = 0; j < matrisboyutu; j++)	
			if (abs(i-N-1)+abs(j-N-1) <= N){
				matris[i][j] = 1;
				count[i]++;//Her bir sat�rdaki siyah h�cre say�s�n�n hesaplanmas�
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
