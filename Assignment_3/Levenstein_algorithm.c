#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX 1000

int HornerKey(char *);//Horner metodunu uygulayan fonksiyon
int strtoASC2(char);//B�y�k-k���k harf fark�n� ortadan kald�ran fonksiyon
int hash(unsigned long int,int);//Double hashing yapan fonksiyon
int compare(char *,char *);//K���k b�y�k harf fark� olmaks�z�n iki kelimeyi kar��la�t�ran fonksiyon
void sozlukHashOlustur(char **,FILE *);//s�zl�k i�in hash tablosu ol�turan fonksiyon
int *KelimeAra(char *,char **);//Hash tablosunda ilgili kelimeyi arayan fonksiyon
char* Levenstein(char **,char **, char *);//Levenstein algoritmas� ile dinamik programlamayla s�zl�kteki kelimelerden yanl�� yaz�lan kelime i�in �neride bulunan fonksiyon
int min(int,int,int);//Levenstein algoritmas�nda harflerin farkl� oldu�u k�s�mda minimum cost de�erini hesaplayan fonksiyon

int main(){
	
	char cumle[MAX] = {'\0'};//Kullan�c� taraf�ndan girilen c�mlenin saklanaca�� string
	char correct[MAX] = {'\0'};//C�mlenin do�ru halinin saklanaca�� string
	char kelime[MAX][MAX];//c�mledeki her bir kelimenin saklanaca�� string dizisi
	char **sozluk;//S�zl�kteki kelimelerin saklanaca�� hash tablosu
	char **hatali;//Hatal� yaz�lm�� kelimelerin saklanaca�� hash tablosu
	char temp[MAX] ={'\0'};//Kelimelerin ge�ici olarak sakland��� de�i�ken
	char *word;//Hatal� kelimenin do�ru halinin saklanaca�� de�i�ken
	int i;
	int j;
	int k;
	int sayac = 0;//correct[] string de�i�keni i�in indis olarak kullan�lacak de�i�ken
	int count = 0;//temp[] string de�i�keni i�in indis olarak kullan�lacak de�i�ken
	int *index;//Kelimelerin hash tablosundaki adreslerini ve kelimenin hash tablosunda bulunup bulunmad���n� i�eren de�i�ken
	int control;
	FILE *fp = fopen("smallDictionary.txt", "r"); 
	
	sozluk = (char **)calloc(MAX,sizeof(char*));//s�zl�k Hash tablosu i�in haf�zada yer a��lmas�
	for (i = 0; i < MAX; i++){
		sozluk[i] = (char *)calloc(MAX,sizeof(char));
		strcpy(sozluk[i],"\0");
	}

	hatali = (char **)calloc(MAX,sizeof(char*));//hatali kelimelerin Hash tablosu i�in haf�zada yer a��lmas�
	for (i = 0; i < MAX; i++){
		hatali[i] = (char *)calloc(MAX,sizeof(char));
		strcpy(hatali[i],"\0");
	}
	word = (char *)calloc(MAX,sizeof(char));
	index = (int *)calloc(2,sizeof(int));
	
	sozlukHashOlustur(sozluk,fp);//s�zl�kteki kelimeler i�in hash tablosu olu�turulmas�
	fclose(fp);
	
	printf("Enter the sentence: ");
	gets(cumle);
	
	while(cumle[0] != '0'){//Kullan�c� 0 tu�una basmad��� s�rece kullan�c�dan c�mle al�n�r
		
		for(i = 0; i < MAX; i++){//temp[],correct[] ve kelime[][] stringlerinde �nceden kalan de�erlerin temizlenmesi
			temp[i] = '\0';
			correct[i] = '\0';
			for(j = 0; j < MAX; j++)
				kelime[i][j] = '\0';
		}
		
		i = 0; 
		k = 0; 
		j = 0;
		while(cumle[i] != '\0'){//c�mlenin sonuna gelinmedi�i s�rece
			
			kelime[j][k] = cumle[i];//c�mledeki her kelime kelime[][] string dizisine atan�r
			i++;
			k++;
			
			if((cumle[i] == ' ') || (cumle[i] == '\0')){//bo�lu�a gelind�inde bir sonraki kelime al�n�r
				kelime[j][k] = '\0';
				j++;
				i++;
				k = 0;
			}	
		}

		sayac = 0;
		for(i = 0; i < j; i++){//c�mleden al�nan kelimelerin tek tek kontrol edilmesi
		
			index = KelimeAra(kelime[i],sozluk);//kelimenin s�zl�kte aranmas�
			
			if(index[0]){//E�er kelime s�zl�kte varsa do�ru yaz�lm��t�r

				strcpy(temp,sozluk[index[1]]);//kelime,ge�ici olarak temp[] dizisinde saklan�r
			}
			else{//E�er kelime s�zl�kte yoksa
			
				index = KelimeAra(kelime[i],hatali);//Hatal� kelimelerin bulundu�u hash tablosunda aran�r
				
				if (index[0]){//E�er hatal� kelimelerin hash tablsounda varsa
			
					k = 0;
					while(hatali[index[1]][k] != ' ')//Kelimenin do�ru haline eri�mek i�in bo�luk karakteri g�rene kadar ilerlenir
						k++;
					
					k++;
					count = 0;
					while((hatali[index[1]][k] != ' ')){//Kelimenin do�ru hali temp[] dizisine aktar�l�r
						
						temp[count] = hatali[index[1]][k]; 
						count++;
						k++;
					}
					temp[count] = ' ';
				}
				else{//E�er kelime hatal� hash tablosunda da yoksa

					word = Levenstein(sozluk,hatali,kelime[i]);//Levenstein algoritmas� ile bu kelimeye 
					strcpy(temp,word);//Se�ilen kelime temp[] dizisine aktar�l�r
					temp[strlen(word)] = ' ';
				}
			}

			for(k = 0; k < strlen(temp); k++){//temp[] dizisindeki kelime do�ru c�mlenin olaca�� correct[] dizisine eklenir
				
				correct[sayac] = temp[k];
				sayac++;
			}
			
			for(k = 0; k < MAX; k++){//word[] ve temp[] dizisindeki �nceki kelimeye ait harflerin temizlenmesi
	
				word[k] = '\0';
				temp[k] = '\0';
			}
			
		}
		
		printf("CORRECT: %s\n",correct);//kullan�c� taraf�ndn girilen c�mlenin do�ru halinin ekrana yazd�r�lmas�

		printf("Enter the sentence:(If you end press 0 and Enter) ");
		for(i = 0; i < MAX; i++)//cumle[] dizisindeki �nceki c�mleye ait harflerin temizlenmesi
			cumle[i] = '\0';
	
		gets(cumle);//Kullan�c�dan c�mlenin al�nmas�

	}
	
	return 0;
}

void sozlukHashOlustur(char **sozluk, FILE *f){
	
	char temp[MAX] = {'\0'};
	char ch;
	int k;
	int key;
	int h;
	int i;
	int sayac = 0;
	
	i = 0;
	
	while((ch = (char)getc(f)) != EOF){//Dosyan�n sonuna gelinmedi�i s�rece
		
		k = 0;
		while(ch != ' '){//Dok�mandaki kelimeler bo�luk ile ayr�ld��� i�in her bir kelime bo�luk karakteri ile kontrol edilir
			temp[k] = ch;//Dok�mandaki kelime harf harf al�narak temp[] dizisine aktar�l�r
			k++;
			ch = (char)getc(f);
			
		}
		temp[k--] = '\0';
		i = 0;
		
		key = HornerKey(temp);//Kelime Horner metod ile say�sal de�erinin hesaplanmas�
		h = hash(key,i);//key de�erinden faydalanarak kelimenin hash de�erinin hesaplanmas�
		
		if(!strcmp(sozluk[h],"\0")){//E�er hesaplanan hash adresi bo�sa
			
			strcpy(sozluk[h],temp);//Bu hash adresinin oldu�u yere ilgili kelime yaz�l�r
			sozluk[h][strlen(temp)] = ' ';
		}
		else{//E�er hesaplanan hash adresi bo� de�ilse
				
			while((i < MAX) && (strcmp(temp,sozluk[h])) && (strcmp(sozluk[h],"\0"))){//Aranan kelime bulunmad�k�a ve hesaplanan hash adresinde bir kelime oldu�u s�rece d�ng� devam eder
			
				i++;
				h = hash(key,i);//Yeni hash de�eri hesaplan�r
			}

			strcpy(sozluk[h],temp);//Bulunan bo� adrese kelimenin eklenemesi
			sozluk[h][strlen(temp)] = ' ';
		}
	}
}

int *KelimeAra(char str[], char **sozluk){
	
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

	key = HornerKey(str);//Horner metodu ile kelimenin say�sal de�erinin hesaplanmas�
	h = hash(key,i);//key de�erinden faydalanarak kelimenin hash de�erinin hesaplanmas�

	if(!strcmp(sozluk[h],"\0")){//Hesaplanan hash de�eri bo�sa
		strcpy(temp,sozluk[h]);
	}
	else{//hesaplanan hash de�erinde bir kelime varsa
	
		while((sozluk[h][k] != ' ')){//kelimenin temp[] dizisine atanmas�
		
			temp[k] = sozluk[h][k];
			k++;
		}
	}

	while((i < MAX) && (!compare(temp,str)) && (strcmp(temp,"\0"))){//Aranan kelime bulunmad�k�a ve hesaplanan hash adresinde bir kelime oldu�u s�rece d�ng� devam eder
		for(j = 0; j < MAX; j++)//temp dizisindeki eski de�erin temizlenmesi
			temp[j] = '\0';
		i++;
		h = hash(key,i);//Yeni hash de�eri hesaplan�r
		
		if(!strcmp(sozluk[h],"\0")){//Yeni hash adresi bo�sa
			strcpy(temp,sozluk[h]);
		}
		else{//Yeni hash adresi bo� de�ilse
			
			k = 0;
			while(sozluk[h][k] != ' '){//Yeni hash adresindeki kelime temp dizisine aktar�l�r
		
				temp[k] = sozluk[h][k];
				k++;
			}	
		}
	}

	if (compare(str,temp)){//E�er aranan kelime daha �nceden hash tablosunda varsa

		control[0] = 1;
	}
	else{
		
		control[0] = 0;
	}
	control[1] = h;//Kelimenin hash tablosunda eklenece�i de�erin atanmas�
	return control;
}

char* Levenstein(char **sozluk, char **hatali, char str[]){
	
	char temp[MAX];
	char *str1[MAX];//uzakl��� bir olan kelimelerin tutulaca�� de�i�ken
	char *str2[MAX];//uzakl��� iki olan kelimelerin tutulaca�� de�i�ken
	char *kelime;
	char tmp[MAX];
	char t[MAX];//Hatal� yaz�lm�� kelimenin k���k harfle yaz�lm�� halinin saklanaca�� de�i�ken
	int **dist;//Kelimeler aras�ndaki uzakl���n hesaplanaca�� matris
	char hatalikelime[MAX] = {'\0'};
	int h;
	int k;
	int i;
	int j;
	int len1;//Birinici kelimenin uzunlu�u
	int len2;//�kinci kelimenin uzunlu�u
	int count1 = 0;//uzakl��� bir olan kelime say�s�n� tutacak olan de�i�ken
	int count2 = 0;//uzakl��� iki olan kelime say�s�n� tutacak olan de�i�ken
	int *index;
	int control;
	
	for(i = 0; i < strlen(str); i++){//Hatal� yaz�lm�� kelimedeki b�t�n harflerin k���k harfe �evrilmesi
		
		if(str[i] <= 'Z')
			t[i] = str[i] + 'a' - 'A';
		else
			t[i] = str[i];
	}
	
	h = 0;
	index = (int *)calloc(2,sizeof(int));
	kelime = (char *)calloc(MAX,sizeof(char));
	
	for(i = 0; i < MAX; i++){
		
		str1[i] = (char *)calloc(MAX,sizeof(char));
		str2[i] = (char *)calloc(MAX,sizeof(char));
	}
	
	while(h < MAX){//s�zl�kteki b�t�n kelimelerin kontrol edilmesi
		
		if(strcmp(sozluk[h],"\0")){//Hash tablosunda ilgili adres bo� de�ilse
			
			for(j = 0; j < 50; j++)//temp dizisindeki eski de�erin temizlenmesi
			temp[j] = '\0';
		
			k = 0;
			while(sozluk[h][k] != ' '){//s�zl�kteki kelimenin temp[] dizisine al�nmas�
			
				temp[k] = sozluk[h][k];
				k++;
			}
			temp[k]  = '\0';
		
			len1 = strlen(temp);//S�zl�kteki kelimenin uzunlu�u
			len2 = strlen(str);//hatal� yaz�lan kelimenin uzunlu�u
	
			dist = (int **)calloc(len1+1, sizeof(int *));
	
			for(i = 0; i <= len1; i++)
				dist[i] = (int *)calloc(len2+1, sizeof(int));

			for (i = 0; i <= len1; i++) dist[i][0] = i;//ilk s�tunun doldurulmas�
			for (j = 0; j <= len2; j++) dist[0][j] = j;//ilk sat�r�n doldurulmas�
			
			i = 1; k = 0;
			while((i <= len1) && (k != 3)){

				j = 1;
				while((j <= len2) && (k != 3)){
		
					if(temp[i-1] == t[j-1])//E�er kar��la�t�r�lan harfler e�itse sol �apraz�ndaki de�er al�n�r
						dist[i][j] = dist[i-1][j-1];
					else//E�er kar��la�t�r�lan harfler farkl�ysa en k���k cost de�erine sahip olan say� al�n�r  
						dist[i][j] = min(dist[i-1][j],dist[i][j-1],dist[i-1][j-1]) + 1;
	
					if(dist[i][i + len2 - len1] == 3)//E�er k��egendeki de�er 3 ise uzakl�k hesaplamay� bitir
						k = 3;
					j++;
				}
				i++;
			}
			
			if(k != 3){//E�er uzakl�k 1 ya da 2 ise
				
				if(dist[len1][len2] == 1){//E�er uzakl�k 1 ise
					strcpy(str1[count1],temp);
					count1++;
				}
				else if(dist[len1][len2] == 2){//E�er uzakl�k iki ise
					strcpy(str2[count2],temp);
					count2++;
				}
			}
		}
		
		h++;
	}
	
	if(count1){//E�er uzakl��� 1 olan en az bir kelime varsa 
		
		printf("%s is not in Dictionary. Did you mean: ",str);
		
		for(i = 0; i < count1-1; i++)
			printf("%s or ", str1[i]);
		printf("%s", str1[i]);		
	}
	else if(count2){//E�er uzakl��� 1 olan kelime yok ve uzakl��� 2 olan en az bir kelime varsa
		
		printf("%s is not in Dictionary. Did you mean: ",str);
		
		for(i = 0; i < count2-1; i++)
			printf("%s or ", str2[i]);
		printf("%s", str2[i]);
	}
	else{//E�er uzakl��� 1 veya 2 olan herhangi bir kelime yoksa
		
		printf("There is not word that matches with %s ", str);
		return str;
	}
	printf("\n");
	
	control = 0;
	while(control != 1){//Kullan�c�n�n girdi�i kelimeninin �nerilenler listesinde olup olmad���n�n kontrol�

		scanf("%s",kelime);//�nerilen kelimelerden se�ilecek olan kelimenin kullan�c�dan al�nmas�  
		gets(tmp);//'\n' karakterini al�r
		
		if(count1){
			
			for(i = 0; i < count1; i++)
				if(!strcmp(str1[i],kelime))
					control = 1;
		}						
		else if(count2){
				
			for(i = 0; i < count2; i++)
				if(!strcmp(str2[i],kelime))
					control = 1;	
		}		
	}

	index = KelimeAra(str,hatali);//hatal� kelimenin hatal� kelimelerin oldu�u hash tablosunda aranarak olmas� gereken hash adresinin hesaplanmas�
	
	for(i = 0; i < strlen(str); i++)//Hash adresine eklenecek hatal� kelime ve do�ru hali i�in �ncelikle hatal� kelime eklenir
		hatalikelime[i] = str[i];
	
	hatalikelime[i] = ' ';//Araya bir bo�luk koyulur
	
	k = 0;
	for(i = strlen(str)+1; i < strlen(str) + 1 + strlen(kelime); i++){//Kullan�c� taraf�ndan se�ilen kelime yan�na yaz�l�r
		
		hatalikelime[i] = kelime[k];
		k++;
	}
	hatalikelime[i] = ' ';
	strcpy(hatali[index[1]],hatalikelime);//Hatal� kelime ve kullan�c� taraf�ndan se�ilen do�ru halinin hash adresinde saklanmas�
	return kelime;
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

int min(int num1, int num2, int num3){
	
	int min;
	
	if (num1 < num2){ // �lk say� ikinci say�dan k���kse
		
		min = num1;
		
		if (num3 < num1)// ���nc� say� da birinci say�dan k���kse
			min = num3;// en k���k say� ���nc� say�d�r
	}
	else{//ilk say� ikinci say�dan k���k de�ilse
		
		min = num2;
		
		if (num3 < num2) //���nc� say� ikinci say�dan k���kse 
			min = num3;// en k���k say� ���nc� say�d�r
	}
	
	return min;
}
