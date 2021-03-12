#include <stdio.h>
#include <stdlib.h>
#define MAX 1000

void MergeSort(int *, int, int);//Merge Sort algoritmas�n� ger�ekle�tiren fonksiyon
void Merge(int *, int, int, int);//S�ralanm� par�alar� s�ral� olacak �ekilde birle�tiren fonksiyon
void EnYakinBul(int *, int);//S�ral� dizideki birbire de�er olarak en yak�n eleman� bulan fonksiyon

int main(){
	
	int N;//Dizinin eleman say�s�
	int i;//indis
	int dizi[MAX];//dizi
	
	printf("Dizinin eleman sayisini giriniz: ");
	scanf("%d", &N);
	
	for (i = 0; i < N; i++){
		
		printf("Dizinin %d. elemanini giriniz: ", i+1);
		scanf("%d", &dizi[i]);
	}
	
	MergeSort(dizi, 0, N-1);//Merge Sort algoritmas� ile dizideki birbirine yak�n elemanlar�n yanyana dizilmesi
	
	printf("Dizinin s�ralanm�� hali: ");
	for (i = 0; i < N; i++)
		printf("%d ", dizi[i]);
	
	EnYakinBul(dizi,N);//S�ralanm�� dizideki elemanlardan en yak�n olan�n bulunmas�
	
	return 0;
}

void EnYakinBul(int dizi[], int n){
	
	int i;//indis
	int sayi1;//En yak�n say�lardan birincisi
	int sayi2;//En yak�n say�lardan ikincisi
	int min = MAX;//dizideki elemanlar aras�ndaki fark�n tutuldu�u de�iken 
	
	for (i = 0; i < n-1; i++){//Ard���k elemanlar aras�ndaki fark�n bulunarak en d���k olup olmad���n�n kontrol�
		
		if(abs(dizi[i]-dizi[i+1]) < min){
			
			sayi1 = dizi[i];
			sayi2 = dizi[i+1];
			min = abs(sayi1 - sayi2);
		}
	}
	printf("\nDizide en yakin degere sahip olan elemanlar: %d %d\n",sayi1, sayi2);
}


void MergeSort(int a[], int start, int end){
	
	int mid;//Dizinin ortadaki indis de�eri
	
	if(start < end){
		
		mid = (start + end) / 2;//Dizideki ortanca indisin elde edilmesi
		MergeSort(a,start,mid);//Dizinin ilk yar�s�n�n merge sort algoritmas�na sokulmas�
		MergeSort(a,mid+1,end);//Dizinin di�er yar�s�n�n merge sort algoritmas�na sokulmas�
		Merge(a,start,mid,end);//S�ralanm�� dizi par�alar�n�n s�ral� bir �ekilde birle�tirilmesi
	}
}

void Merge(int a[], int start, int mid, int end){
	
	int i;//Dizinin ilk yar�s�n� gezinmek i�in kullan�lacak indis
	int j;//Dizinin di�er yar�s�n� gezinmek i�in kullan�lacak indis
	int k;//Elemanlar�n s�ral� olarak eklenece�i ge�ici dizi i�in kullan�lacak indis
	int size_temp;//ge�ici dizinin boyutu
	int *temp;//ge�ici dizi
	size_temp = end - start + 1;
	temp = (int *)malloc(size_temp * sizeof(int));
	i = start;
	j = mid + 1;
	k = 0;
	
	while((i <= mid) && (j <= end)){
		
		if (a[i] < a[j])
			temp[k++] = a[i++];
		else
			temp[k++] = a[j++];
	}
	
	while(i <= mid)//E�er dizinin ilk yar�s�nda eleman kalm��sa
		temp[k++] = a[i++];
	
	while(j <= end)//E�er dizinin di�er yar�s�nda eleman kalm��sa
		temp[k++] = a[j++];
	
	for (i = 0; i < size_temp; i++)//temp dizisinin a dizisine kopyalanmas�
		a[i+start] = temp[i];
}
