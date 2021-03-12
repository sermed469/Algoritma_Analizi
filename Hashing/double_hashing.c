#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define MAX 200

int HornerKey(char *);//Horner metodunu uygulayan fonksiyon
int strtoASC2(char);//B�y�k-k���k harf fark�n� ortadan kald�ran fonksiyon
void DokumanEkle(FILE *,FILE *,char *,char **);//Yeni gelen dok�mandaki kelimeleri hash tablosuna ekleyen fonksiyon
int *KelimeAra(char *,FILE *,char *,int *,char **);//Hash tablosunda ilgili kelimeyi arayan fonksiyon
int hash(unsigned long int,int);//Double hashing yapan fonksiyon
int compare(char *,char *);
int controlDoc(int *,char *,char **);//Bir kelime ayn� dok�manda bir�ok kez ge�iyorsa bunu kontrol eden fonksiyon

int main(){

	int M;//Hash tablosu boyutu
	int i;
	int control;//Dok�man ekleme mi yoksa kelime arama m� yap�ld���n� kontrol eden de�i�ken
	int adim;//Arama i�leminin ka� ad�mda ger�ekle�ti�ini tutan de�i�ken
	char str[MAX];//Aranacak kelimeyi tutacak string
	char d[MAX];//Dok�man� i�eren dosya ad�n� tutan de�i�ken
	char kelime[MAX];//
	char **sozluk;//Hash tablosunu tutan de�i�ken
	FILE *doc;//dok�man dosyas�
	FILE *sz;//hash tablosu dosyas�
	
	sozluk = (char **)calloc(998,sizeof(char*));//Hash tablosu i�in haf�zada yer a��lmas�
	for (i = 0; i < 998; i++)
		sozluk[i] = (char *)calloc(MAX,sizeof(char));
	
	i = 0;
	M = 997;//tablo boyutunun 1000'e en yak�n asal say� se�ilmesi
	sz = fopen("dictionary.txt", "r+");
	
	while(i<998){//Hash tablosunun tutuldu�u dosyadan bilgilerin �ekilmesi

		fgets(sozluk[i],MAX,sz);
		i++;
	}
	
	printf("Dokuman eklemek icin 1'e kelime aramak icin 0'a basiniz: ");
	scanf("%d", &control);
	
	if (control == 1){//E�er dok�man eklenecekse
		
		printf("Dokumanin adini giriniz(.txt uzantisi ile birlikte giriniz) ");
		scanf("%s",d);
		doc = fopen(d, "r");
		DokumanEkle(doc,sz,d,sozluk);
		fclose(doc);
	}
	else if(control == 0){//E�er kelime aran�yorsa
		
		printf("Aranacak kelimeyi giriniz: ");
		scanf("%s",str);
		KelimeAra(str,sz,kelime,&adim,sozluk);
		puts(kelime);
		printf("%d adimda bulundu\n", adim+1);
	}
	
	fclose(sz);
	return 0;
}

void DokumanEkle(FILE *doc,FILE *sz,char d[],char **sozluk){
	
	char temp[MAX] = {'\0'};//kelimenin ge�ici olarak saklanaca�� dizi
	char c[MAX] = {'\0'};
	char ch;//Dosyadan al�nan karakterlerin saklanaca�� de�i�ken
	int *index;//kelimenin daha 
	int key;//Horner metodu ile olu�turulacak key de�eri
	int k;//temp dizisindeki indis
	int count = 0;
	int t;
	int N;//Yeni eklenen kelimelerin say�s�n� tutan de�i�ken
	int i;
	int samedoc;//Ayn� dok�manda bir kelimenin bir �ok kez ge�ip ge�medi�ini kontrol eden de�i�ken
	double loadfactor;//load factor de�erini tutacak de�i�ken
	
	N = 0;
	
	loadfactor = 0;
	for(i = 0; i < 997; i++){//
		
		if(sozluk[i][0] != '\n' )
			loadfactor++;	
	}

	index = (int *)calloc(2,sizeof(int));
	while((ch = (char)getc(doc)) != EOF){//Dok�mandan kelimelerin al�nmas�

		samedoc = 1;
		k = 0;
		while(ch != ' '){//Dok�mandaki kelimeler bo�luk ile ay�rt edilmesi
			temp[k] = ch;
			k++;
			ch = (char)getc(doc);
		}
		temp[k--] = '\0';
		
		index = KelimeAra(temp,sz,c,&t,sozluk);//�lgili kelimenin daha �nce hash tablosunda olup olmad���na bak�lmas�
		if(index[0]){//E�er hash tablosunda aranan kelime varsa

			k = 0;
			while(sozluk[index[1]][k] != '\n')
				k++;
			samedoc = ControlDoc(index,d,sozluk);//Ayn� dok�manda kelime tekrarlanm�� m�
		}
		else{//E�er kelime ilk kez eklenecekse
			
			k = 0;
			count = 0;
			while((temp[count] != '\n') && (temp[count])){
				sozluk[index[1]][k] = temp[count];
				count++;
				k++;
			}
			N++;
			printf("Load factor: %f\n", (N + loadfactor) / 997);
			if ((N + loadfactor) / 997 > 0.8){ //load factor 0.8 de�erinden b�y�k ise
				printf("UYARI!\nLOAD FACTOR 0.8'i GECTI\n");
			  	if ((N + loadfactor) / 997 >= 1.0){//Load factor 1.0 oldu�unda ekleme i�lemi biter 
					while(!feof(doc)){
						fgets(temp,MAX,doc);
						puts(temp);//Eklenemeyen kelimeler ekrana yazd�r�l�r
					}
					return;
			    }
			}		
		}
		if(samedoc){//E�er eklenecek kelime ayn� dok�manda de�ilse
			
			sozluk[index[1]][k++] = ' ';
			count = 0;
			while(d[count]){//Dok�man ad� hash tablosunda kelimenin oldu�u k�sma yaz�l�r
				sozluk[index[1]][k] = d[count];
				k++;
				count++;
			}
			sozluk[index[1]][k] = '\n';	
		}
	}
	fseek(sz,0,SEEK_SET);//Hash tablosunu i�eren dosyada file position indicator ba�a getirilir
	
	k = 0;
	while(k < 997){//Hash tablosunu i�eren sozluk de�i�kenindeki bilgiler ile dosyan�n g�ncellenmesi
		
		fputs(sozluk[k],sz);
		k++;
	}
	
	loadfactor = (N + loadfactor) / 997;
	sozluk[997][0] = (int)loadfactor + '0';//Load factor'de virg�l�n solundaki basamak karaktere �evirilir
	sozluk[997][1] = '.';
	sozluk[997][2] = (((int)(loadfactor * 10)) % 10) + '0';//Virg�lden sonraki ilk basama��n karaktere d�n��t�r�lmesi
	sozluk[997][3] = (((int)(loadfactor * 100)) % 10) + '0';//Virg�lden sonraki ikinci basama��n karaktere d�n��t�r�lmesi
	fputs(sozluk[k],sz);//Load factor de�eri stringe d�n��t�r�ld�kten sonra dosyan�n sonuna eklenir		
}

int *KelimeAra(char str[], FILE *sz, char kelime[], int *sayac, char **sozluk){
	
	unsigned long int key;//key de�erini tutacak de�i�ken
	int i;
	int h;//hash de�erini tutacak de�i�ken
	int k;
	int j;
	int *control;
	char temp[MAX] = {'\0'};//kelimelerin saklanaca�� dizi
	
	control = (int *)calloc(2,sizeof(int));//Aranan kelimenin hem hash tablosunda olup olmad��� hem de kelimenin hash de�eri 2 elemanl� int dizisinde tutulur
	
	i = 0;
	k = 0;
	*sayac = 0;//Aranacak kelimenin ka� ad�mda arand���n� tutacak de�i�ken
	
	key = HornerKey(str);//Kelime Horner metod ile say�sal de�erinin hesaplanmas�
	h = hash(key,i);//key de�erinden faydalanarak kelimenin hash de�erinin hesaplanams�

	if(!strcmp(sozluk[h],"\n")){//Hesaplanan hash de�erinde kelime yoksa
		strcpy(temp,sozluk[h]);
	}
	else{//hesaplanan hash de�erinde bir kelime varsa
	
		while(sozluk[h][k] != ' '){//kelimenin temp string de�i�kenine atanmas�
		
			temp[k] = sozluk[h][k];
			k++;
		}
	}

	while((i < 997) && (!compare(temp,str)) && (strcmp(temp,"\n"))){//Aranan kelime bulunmad�k�a ve hesaplanan hash adresinde bir kelime oldu�u s�rece d�ng� devam eder
		for(j = 0; j < MAX; j++)//temp dizisindeki eski de�erin temizlenmesi
			temp[j] = '\0';
		i++;
		h = hash(key,i);//Yeni hash de�eri hesaplan�r

		if(!strcmp(sozluk[h],"\n")){//Yeni hash adresi bo�sa
			strcpy(temp,sozluk[h]);
		}
		else{//Yeni hash adresi bo� de�ilse
			
			k = 0;
			while((sozluk[h][k] != ' ') && (strcmp(sozluk[h],"\n"))){//Yeni hash adresindeki kelime temp dizisine aktar�l�r
		
				temp[k] = sozluk[h][k];
				k++;
			}	
		}
		(*sayac)++;//ad�m say�s� art�r�l�r
	}
	if (compare(temp,str)){//E�er aranan kelime daha �nceden hash tablosunda varsa

		strcpy(kelime,sozluk[h]);
		control[0] = 1;
	}
	else{
		
		strcpy(kelime,"Aradiginiz kelime bulunamadi.");
		control[0] = 0;
	}
	control[1] = h;//Kelimenin hash tablosunda eklenece�i de�erin atanmas�
	return control;
}

int hash(unsigned long int k, int i){
	
	int M;//Tablo boyutu
	int h1;//�lk hash sonucunu tutacak de�i�ken
	int h2;//ikinci hash sonucunu tutacak de�i�ken
	int h;//Son hash sonucunu tutacak de�i�ken
	
	M = 997;
	
	h1 = k % M;
	h2 = 1 + (k % (M-1));
	 
	return (h1+(h2*i))%M;
}

int HornerKey(char str[]){
	
	int i;
	int R = 31;
	unsigned long int key = 0;
	
	for (i=strlen(str)-1;i>=0;i--)
		key += pow(R,i)*strtoASC2(str[strlen(str)-1-i]);
	
	return key;
}

int strtoASC2(char c){
	
	if (c >= 'a')//E�er k���k harf ise
		c = 'A' - 'a' + c;//B�y�k harfe �evrilir
	
	return c - 'A' + 1;//Horner metodu i�in gerekli i�lem yap�l�r
}

int compare(char s1[], char s2[]){
	
	int i;
	int count;//harfler aras�ndaki fark� tutacak de�i�ken
	i = 0;
	count = 0;
	while(i < strlen(s1)){
		count += strtoASC2(s1[i]) - strtoASC2(s2[i]);//Kar��la�t�r�lan kelimelerin harfleri aras�ndaki fark hesaplan�r
		i++;
	}
	if (count == 0) return 1;//E�er aradaki fark 0 ise ayn� kelimelerdir
	return 0;
}

int ControlDoc(int *indis, char *d, char **s){
	
	char temp[MAX];//dok�man ad�n� tutacak dizi
	int k;//hash adresini tutacak de�i�ken
	int i;
	int count;//temp dizisinde indis olarak kullan�lacak de�i�ken
	k = 0;
	while(s[indis[1]][k] != '\n'){//hash adresi bo� de�ilse
		
		count = 0; 
		for(i = 0; i < MAX; i++)//temp dizisindeki eski ifadenin temizlenmesi
			temp[i] = '\0';
			
		while(s[indis[1]][k] != ' ' && s[indis[1]][k] != '\n'){//hash tablosundaki dok�man isminin temp dizisinde saklamas�
			temp[count] = s[indis[1]][k];
			k++;
			count++;
		}
		
		if (s[indis[1]][k] == ' ' )
			k++;

		if(!strcmp(temp,d))//hash tablosundaki dok�man ismi yeni eklenecek kelimenin oldu�u dok�manla ayn� ise
			return 0;	
	}
	return 1;
}
