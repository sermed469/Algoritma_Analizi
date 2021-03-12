#include <stdio.h>
#include <stdlib.h>
#define MAX 1000

void MergeSort(int *, int, int);//Merge Sort algoritmasýný gerçekleþtiren fonksiyon
void Merge(int *, int, int, int);//Sýralanmþ parçalarý sýralý olacak þekilde birleþtiren fonksiyon
void EnYakinBul(int *, int);//Sýralý dizideki birbire deðer olarak en yakýn elemaný bulan fonksiyon

int main(){
	
	int N;//Dizinin eleman sayýsý
	int i;//indis
	int dizi[MAX];//dizi
	
	printf("Dizinin eleman sayisini giriniz: ");
	scanf("%d", &N);
	
	for (i = 0; i < N; i++){
		
		printf("Dizinin %d. elemanini giriniz: ", i+1);
		scanf("%d", &dizi[i]);
	}
	
	MergeSort(dizi, 0, N-1);//Merge Sort algoritmasý ile dizideki birbirine yakýn elemanlarýn yanyana dizilmesi
	
	printf("Dizinin sýralanmýþ hali: ");
	for (i = 0; i < N; i++)
		printf("%d ", dizi[i]);
	
	EnYakinBul(dizi,N);//Sýralanmýþ dizideki elemanlardan en yakýn olanýn bulunmasý
	
	return 0;
}

void EnYakinBul(int dizi[], int n){
	
	int i;//indis
	int sayi1;//En yakýn sayýlardan birincisi
	int sayi2;//En yakýn sayýlardan ikincisi
	int min = MAX;//dizideki elemanlar arasýndaki farkýn tutulduðu deðiken 
	
	for (i = 0; i < n-1; i++){//Ardýþýk elemanlar arasýndaki farkýn bulunarak en düþük olup olmadýðýnýn kontrolü
		
		if(abs(dizi[i]-dizi[i+1]) < min){
			
			sayi1 = dizi[i];
			sayi2 = dizi[i+1];
			min = abs(sayi1 - sayi2);
		}
	}
	printf("\nDizide en yakin degere sahip olan elemanlar: %d %d\n",sayi1, sayi2);
}


void MergeSort(int a[], int start, int end){
	
	int mid;//Dizinin ortadaki indis deðeri
	
	if(start < end){
		
		mid = (start + end) / 2;//Dizideki ortanca indisin elde edilmesi
		MergeSort(a,start,mid);//Dizinin ilk yarýsýnýn merge sort algoritmasýna sokulmasý
		MergeSort(a,mid+1,end);//Dizinin diðer yarýsýnýn merge sort algoritmasýna sokulmasý
		Merge(a,start,mid,end);//Sýralanmýþ dizi parçalarýnýn sýralý bir þekilde birleþtirilmesi
	}
}

void Merge(int a[], int start, int mid, int end){
	
	int i;//Dizinin ilk yarýsýný gezinmek için kullanýlacak indis
	int j;//Dizinin diðer yarýsýný gezinmek için kullanýlacak indis
	int k;//Elemanlarýn sýralý olarak ekleneceði geçici dizi için kullanýlacak indis
	int size_temp;//geçici dizinin boyutu
	int *temp;//geçici dizi
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
	
	while(i <= mid)//Eðer dizinin ilk yarýsýnda eleman kalmýþsa
		temp[k++] = a[i++];
	
	while(j <= end)//Eðer dizinin diðer yarýsýnda eleman kalmýþsa
		temp[k++] = a[j++];
	
	for (i = 0; i < size_temp; i++)//temp dizisinin a dizisine kopyalanmasý
		a[i+start] = temp[i];
}
