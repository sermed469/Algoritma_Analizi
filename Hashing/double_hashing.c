#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define MAX 200

int HornerKey(char *);//Horner metodunu uygulayan fonksiyon
int strtoASC2(char);//Büyük-küçük harf farkýný ortadan kaldýran fonksiyon
void DokumanEkle(FILE *,FILE *,char *,char **);//Yeni gelen dokümandaki kelimeleri hash tablosuna ekleyen fonksiyon
int *KelimeAra(char *,FILE *,char *,int *,char **);//Hash tablosunda ilgili kelimeyi arayan fonksiyon
int hash(unsigned long int,int);//Double hashing yapan fonksiyon
int compare(char *,char *);
int controlDoc(int *,char *,char **);//Bir kelime ayný dokümanda birçok kez geçiyorsa bunu kontrol eden fonksiyon

int main(){

	int M;//Hash tablosu boyutu
	int i;
	int control;//Doküman ekleme mi yoksa kelime arama mý yapýldýðýný kontrol eden deðiþken
	int adim;//Arama iþleminin kaç adýmda gerçekleþtiðini tutan deðiþken
	char str[MAX];//Aranacak kelimeyi tutacak string
	char d[MAX];//Dokümaný içeren dosya adýný tutan deðiþken
	char kelime[MAX];//
	char **sozluk;//Hash tablosunu tutan deðiþken
	FILE *doc;//doküman dosyasý
	FILE *sz;//hash tablosu dosyasý
	
	sozluk = (char **)calloc(998,sizeof(char*));//Hash tablosu için hafýzada yer açýlmasý
	for (i = 0; i < 998; i++)
		sozluk[i] = (char *)calloc(MAX,sizeof(char));
	
	i = 0;
	M = 997;//tablo boyutunun 1000'e en yakýn asal sayý seçilmesi
	sz = fopen("dictionary.txt", "r+");
	
	while(i<998){//Hash tablosunun tutulduðu dosyadan bilgilerin çekilmesi

		fgets(sozluk[i],MAX,sz);
		i++;
	}
	
	printf("Dokuman eklemek icin 1'e kelime aramak icin 0'a basiniz: ");
	scanf("%d", &control);
	
	if (control == 1){//Eðer doküman eklenecekse
		
		printf("Dokumanin adini giriniz(.txt uzantisi ile birlikte giriniz) ");
		scanf("%s",d);
		doc = fopen(d, "r");
		DokumanEkle(doc,sz,d,sozluk);
		fclose(doc);
	}
	else if(control == 0){//Eðer kelime aranýyorsa
		
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
	
	char temp[MAX] = {'\0'};//kelimenin geçici olarak saklanacaðý dizi
	char c[MAX] = {'\0'};
	char ch;//Dosyadan alýnan karakterlerin saklanacaðý deðiþken
	int *index;//kelimenin daha 
	int key;//Horner metodu ile oluþturulacak key deðeri
	int k;//temp dizisindeki indis
	int count = 0;
	int t;
	int N;//Yeni eklenen kelimelerin sayýsýný tutan deðiþken
	int i;
	int samedoc;//Ayný dokümanda bir kelimenin bir çok kez geçip geçmediðini kontrol eden deðiþken
	double loadfactor;//load factor deðerini tutacak deðiþken
	
	N = 0;
	
	loadfactor = 0;
	for(i = 0; i < 997; i++){//
		
		if(sozluk[i][0] != '\n' )
			loadfactor++;	
	}

	index = (int *)calloc(2,sizeof(int));
	while((ch = (char)getc(doc)) != EOF){//Dokümandan kelimelerin alýnmasý

		samedoc = 1;
		k = 0;
		while(ch != ' '){//Dokümandaki kelimeler boþluk ile ayýrt edilmesi
			temp[k] = ch;
			k++;
			ch = (char)getc(doc);
		}
		temp[k--] = '\0';
		
		index = KelimeAra(temp,sz,c,&t,sozluk);//Ýlgili kelimenin daha önce hash tablosunda olup olmadýðýna bakýlmasý
		if(index[0]){//Eðer hash tablosunda aranan kelime varsa

			k = 0;
			while(sozluk[index[1]][k] != '\n')
				k++;
			samedoc = ControlDoc(index,d,sozluk);//Ayný dokümanda kelime tekrarlanmýþ mý
		}
		else{//Eðer kelime ilk kez eklenecekse
			
			k = 0;
			count = 0;
			while((temp[count] != '\n') && (temp[count])){
				sozluk[index[1]][k] = temp[count];
				count++;
				k++;
			}
			N++;
			printf("Load factor: %f\n", (N + loadfactor) / 997);
			if ((N + loadfactor) / 997 > 0.8){ //load factor 0.8 deðerinden büyük ise
				printf("UYARI!\nLOAD FACTOR 0.8'i GECTI\n");
			  	if ((N + loadfactor) / 997 >= 1.0){//Load factor 1.0 olduðunda ekleme iþlemi biter 
					while(!feof(doc)){
						fgets(temp,MAX,doc);
						puts(temp);//Eklenemeyen kelimeler ekrana yazdýrýlýr
					}
					return;
			    }
			}		
		}
		if(samedoc){//Eðer eklenecek kelime ayný dokümanda deðilse
			
			sozluk[index[1]][k++] = ' ';
			count = 0;
			while(d[count]){//Doküman adý hash tablosunda kelimenin olduðu kýsma yazýlýr
				sozluk[index[1]][k] = d[count];
				k++;
				count++;
			}
			sozluk[index[1]][k] = '\n';	
		}
	}
	fseek(sz,0,SEEK_SET);//Hash tablosunu içeren dosyada file position indicator baþa getirilir
	
	k = 0;
	while(k < 997){//Hash tablosunu içeren sozluk deðiþkenindeki bilgiler ile dosyanýn güncellenmesi
		
		fputs(sozluk[k],sz);
		k++;
	}
	
	loadfactor = (N + loadfactor) / 997;
	sozluk[997][0] = (int)loadfactor + '0';//Load factor'de virgülün solundaki basamak karaktere çevirilir
	sozluk[997][1] = '.';
	sozluk[997][2] = (((int)(loadfactor * 10)) % 10) + '0';//Virgülden sonraki ilk basamaðýn karaktere dönüþtürülmesi
	sozluk[997][3] = (((int)(loadfactor * 100)) % 10) + '0';//Virgülden sonraki ikinci basamaðýn karaktere dönüþtürülmesi
	fputs(sozluk[k],sz);//Load factor deðeri stringe dönüþtürüldükten sonra dosyanýn sonuna eklenir		
}

int *KelimeAra(char str[], FILE *sz, char kelime[], int *sayac, char **sozluk){
	
	unsigned long int key;//key deðerini tutacak deðiþken
	int i;
	int h;//hash deðerini tutacak deðiþken
	int k;
	int j;
	int *control;
	char temp[MAX] = {'\0'};//kelimelerin saklanacaðý dizi
	
	control = (int *)calloc(2,sizeof(int));//Aranan kelimenin hem hash tablosunda olup olmadýðý hem de kelimenin hash deðeri 2 elemanlý int dizisinde tutulur
	
	i = 0;
	k = 0;
	*sayac = 0;//Aranacak kelimenin kaç adýmda arandýðýný tutacak deðiþken
	
	key = HornerKey(str);//Kelime Horner metod ile sayýsal deðerinin hesaplanmasý
	h = hash(key,i);//key deðerinden faydalanarak kelimenin hash deðerinin hesaplanamsý

	if(!strcmp(sozluk[h],"\n")){//Hesaplanan hash deðerinde kelime yoksa
		strcpy(temp,sozluk[h]);
	}
	else{//hesaplanan hash deðerinde bir kelime varsa
	
		while(sozluk[h][k] != ' '){//kelimenin temp string deðiþkenine atanmasý
		
			temp[k] = sozluk[h][k];
			k++;
		}
	}

	while((i < 997) && (!compare(temp,str)) && (strcmp(temp,"\n"))){//Aranan kelime bulunmadýkça ve hesaplanan hash adresinde bir kelime olduðu sürece döngü devam eder
		for(j = 0; j < MAX; j++)//temp dizisindeki eski deðerin temizlenmesi
			temp[j] = '\0';
		i++;
		h = hash(key,i);//Yeni hash deðeri hesaplanýr

		if(!strcmp(sozluk[h],"\n")){//Yeni hash adresi boþsa
			strcpy(temp,sozluk[h]);
		}
		else{//Yeni hash adresi boþ deðilse
			
			k = 0;
			while((sozluk[h][k] != ' ') && (strcmp(sozluk[h],"\n"))){//Yeni hash adresindeki kelime temp dizisine aktarýlýr
		
				temp[k] = sozluk[h][k];
				k++;
			}	
		}
		(*sayac)++;//adým sayýsý artýrýlýr
	}
	if (compare(temp,str)){//Eðer aranan kelime daha önceden hash tablosunda varsa

		strcpy(kelime,sozluk[h]);
		control[0] = 1;
	}
	else{
		
		strcpy(kelime,"Aradiginiz kelime bulunamadi.");
		control[0] = 0;
	}
	control[1] = h;//Kelimenin hash tablosunda ekleneceði deðerin atanmasý
	return control;
}

int hash(unsigned long int k, int i){
	
	int M;//Tablo boyutu
	int h1;//Ýlk hash sonucunu tutacak deðiþken
	int h2;//ikinci hash sonucunu tutacak deðiþken
	int h;//Son hash sonucunu tutacak deðiþken
	
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
	
	if (c >= 'a')//Eðer küçük harf ise
		c = 'A' - 'a' + c;//Büyük harfe çevrilir
	
	return c - 'A' + 1;//Horner metodu için gerekli iþlem yapýlýr
}

int compare(char s1[], char s2[]){
	
	int i;
	int count;//harfler arasýndaki farký tutacak deðiþken
	i = 0;
	count = 0;
	while(i < strlen(s1)){
		count += strtoASC2(s1[i]) - strtoASC2(s2[i]);//Karþýlaþtýrýlan kelimelerin harfleri arasýndaki fark hesaplanýr
		i++;
	}
	if (count == 0) return 1;//Eðer aradaki fark 0 ise ayný kelimelerdir
	return 0;
}

int ControlDoc(int *indis, char *d, char **s){
	
	char temp[MAX];//doküman adýný tutacak dizi
	int k;//hash adresini tutacak deðiþken
	int i;
	int count;//temp dizisinde indis olarak kullanýlacak deðiþken
	k = 0;
	while(s[indis[1]][k] != '\n'){//hash adresi boþ deðilse
		
		count = 0; 
		for(i = 0; i < MAX; i++)//temp dizisindeki eski ifadenin temizlenmesi
			temp[i] = '\0';
			
		while(s[indis[1]][k] != ' ' && s[indis[1]][k] != '\n'){//hash tablosundaki doküman isminin temp dizisinde saklamasý
			temp[count] = s[indis[1]][k];
			k++;
			count++;
		}
		
		if (s[indis[1]][k] == ' ' )
			k++;

		if(!strcmp(temp,d))//hash tablosundaki doküman ismi yeni eklenecek kelimenin olduðu dokümanla ayný ise
			return 0;	
	}
	return 1;
}
