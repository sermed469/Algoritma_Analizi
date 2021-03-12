#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX 1000

int HornerKey(char *);//Horner metodunu uygulayan fonksiyon
int strtoASC2(char);//Büyük-küçük harf farkýný ortadan kaldýran fonksiyon
int hash(unsigned long int,int);//Double hashing yapan fonksiyon
int compare(char *,char *);//Küçük büyük harf farký olmaksýzýn iki kelimeyi karþýlaþtýran fonksiyon
void sozlukHashOlustur(char **,FILE *);//sözlük için hash tablosu olþturan fonksiyon
int *KelimeAra(char *,char **);//Hash tablosunda ilgili kelimeyi arayan fonksiyon
char* Levenstein(char **,char **, char *);//Levenstein algoritmasý ile dinamik programlamayla sözlükteki kelimelerden yanlýþ yazýlan kelime için öneride bulunan fonksiyon
int min(int,int,int);//Levenstein algoritmasýnda harflerin farklý olduðu kýsýmda minimum cost deðerini hesaplayan fonksiyon

int main(){
	
	char cumle[MAX] = {'\0'};//Kullanýcý tarafýndan girilen cümlenin saklanacaðý string
	char correct[MAX] = {'\0'};//Cümlenin doðru halinin saklanacaðý string
	char kelime[MAX][MAX];//cümledeki her bir kelimenin saklanacaðý string dizisi
	char **sozluk;//Sözlükteki kelimelerin saklanacaðý hash tablosu
	char **hatali;//Hatalý yazýlmýþ kelimelerin saklanacaðý hash tablosu
	char temp[MAX] ={'\0'};//Kelimelerin geçici olarak saklandýðý deðiþken
	char *word;//Hatalý kelimenin doðru halinin saklanacaðý deðiþken
	int i;
	int j;
	int k;
	int sayac = 0;//correct[] string deðiþkeni için indis olarak kullanýlacak deðiþken
	int count = 0;//temp[] string deðiþkeni için indis olarak kullanýlacak deðiþken
	int *index;//Kelimelerin hash tablosundaki adreslerini ve kelimenin hash tablosunda bulunup bulunmadýðýný içeren deðiþken
	int control;
	FILE *fp = fopen("smallDictionary.txt", "r"); 
	
	sozluk = (char **)calloc(MAX,sizeof(char*));//sözlük Hash tablosu için hafýzada yer açýlmasý
	for (i = 0; i < MAX; i++){
		sozluk[i] = (char *)calloc(MAX,sizeof(char));
		strcpy(sozluk[i],"\0");
	}

	hatali = (char **)calloc(MAX,sizeof(char*));//hatali kelimelerin Hash tablosu için hafýzada yer açýlmasý
	for (i = 0; i < MAX; i++){
		hatali[i] = (char *)calloc(MAX,sizeof(char));
		strcpy(hatali[i],"\0");
	}
	word = (char *)calloc(MAX,sizeof(char));
	index = (int *)calloc(2,sizeof(int));
	
	sozlukHashOlustur(sozluk,fp);//sözlükteki kelimeler için hash tablosu oluþturulmasý
	fclose(fp);
	
	printf("Enter the sentence: ");
	gets(cumle);
	
	while(cumle[0] != '0'){//Kullanýcý 0 tuþuna basmadýðý sürece kullanýcýdan cümle alýnýr
		
		for(i = 0; i < MAX; i++){//temp[],correct[] ve kelime[][] stringlerinde önceden kalan deðerlerin temizlenmesi
			temp[i] = '\0';
			correct[i] = '\0';
			for(j = 0; j < MAX; j++)
				kelime[i][j] = '\0';
		}
		
		i = 0; 
		k = 0; 
		j = 0;
		while(cumle[i] != '\0'){//cümlenin sonuna gelinmediði sürece
			
			kelime[j][k] = cumle[i];//cümledeki her kelime kelime[][] string dizisine atanýr
			i++;
			k++;
			
			if((cumle[i] == ' ') || (cumle[i] == '\0')){//boþluða gelindðinde bir sonraki kelime alýnýr
				kelime[j][k] = '\0';
				j++;
				i++;
				k = 0;
			}	
		}

		sayac = 0;
		for(i = 0; i < j; i++){//cümleden alýnan kelimelerin tek tek kontrol edilmesi
		
			index = KelimeAra(kelime[i],sozluk);//kelimenin sözlükte aranmasý
			
			if(index[0]){//Eðer kelime sözlükte varsa doðru yazýlmýþtýr

				strcpy(temp,sozluk[index[1]]);//kelime,geçici olarak temp[] dizisinde saklanýr
			}
			else{//Eðer kelime sözlükte yoksa
			
				index = KelimeAra(kelime[i],hatali);//Hatalý kelimelerin bulunduðu hash tablosunda aranýr
				
				if (index[0]){//Eðer hatalý kelimelerin hash tablsounda varsa
			
					k = 0;
					while(hatali[index[1]][k] != ' ')//Kelimenin doðru haline eriþmek için boþluk karakteri görene kadar ilerlenir
						k++;
					
					k++;
					count = 0;
					while((hatali[index[1]][k] != ' ')){//Kelimenin doðru hali temp[] dizisine aktarýlýr
						
						temp[count] = hatali[index[1]][k]; 
						count++;
						k++;
					}
					temp[count] = ' ';
				}
				else{//Eðer kelime hatalý hash tablosunda da yoksa

					word = Levenstein(sozluk,hatali,kelime[i]);//Levenstein algoritmasý ile bu kelimeye 
					strcpy(temp,word);//Seçilen kelime temp[] dizisine aktarýlýr
					temp[strlen(word)] = ' ';
				}
			}

			for(k = 0; k < strlen(temp); k++){//temp[] dizisindeki kelime doðru cümlenin olacaðý correct[] dizisine eklenir
				
				correct[sayac] = temp[k];
				sayac++;
			}
			
			for(k = 0; k < MAX; k++){//word[] ve temp[] dizisindeki önceki kelimeye ait harflerin temizlenmesi
	
				word[k] = '\0';
				temp[k] = '\0';
			}
			
		}
		
		printf("CORRECT: %s\n",correct);//kullanýcý tarafýndn girilen cümlenin doðru halinin ekrana yazdýrýlmasý

		printf("Enter the sentence:(If you end press 0 and Enter) ");
		for(i = 0; i < MAX; i++)//cumle[] dizisindeki önceki cümleye ait harflerin temizlenmesi
			cumle[i] = '\0';
	
		gets(cumle);//Kullanýcýdan cümlenin alýnmasý

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
	
	while((ch = (char)getc(f)) != EOF){//Dosyanýn sonuna gelinmediði sürece
		
		k = 0;
		while(ch != ' '){//Dokümandaki kelimeler boþluk ile ayrýldýðý için her bir kelime boþluk karakteri ile kontrol edilir
			temp[k] = ch;//Dokümandaki kelime harf harf alýnarak temp[] dizisine aktarýlýr
			k++;
			ch = (char)getc(f);
			
		}
		temp[k--] = '\0';
		i = 0;
		
		key = HornerKey(temp);//Kelime Horner metod ile sayýsal deðerinin hesaplanmasý
		h = hash(key,i);//key deðerinden faydalanarak kelimenin hash deðerinin hesaplanmasý
		
		if(!strcmp(sozluk[h],"\0")){//Eðer hesaplanan hash adresi boþsa
			
			strcpy(sozluk[h],temp);//Bu hash adresinin olduðu yere ilgili kelime yazýlýr
			sozluk[h][strlen(temp)] = ' ';
		}
		else{//Eðer hesaplanan hash adresi boþ deðilse
				
			while((i < MAX) && (strcmp(temp,sozluk[h])) && (strcmp(sozluk[h],"\0"))){//Aranan kelime bulunmadýkça ve hesaplanan hash adresinde bir kelime olduðu sürece döngü devam eder
			
				i++;
				h = hash(key,i);//Yeni hash deðeri hesaplanýr
			}

			strcpy(sozluk[h],temp);//Bulunan boþ adrese kelimenin eklenemesi
			sozluk[h][strlen(temp)] = ' ';
		}
	}
}

int *KelimeAra(char str[], char **sozluk){
	
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

	key = HornerKey(str);//Horner metodu ile kelimenin sayýsal deðerinin hesaplanmasý
	h = hash(key,i);//key deðerinden faydalanarak kelimenin hash deðerinin hesaplanmasý

	if(!strcmp(sozluk[h],"\0")){//Hesaplanan hash deðeri boþsa
		strcpy(temp,sozluk[h]);
	}
	else{//hesaplanan hash deðerinde bir kelime varsa
	
		while((sozluk[h][k] != ' ')){//kelimenin temp[] dizisine atanmasý
		
			temp[k] = sozluk[h][k];
			k++;
		}
	}

	while((i < MAX) && (!compare(temp,str)) && (strcmp(temp,"\0"))){//Aranan kelime bulunmadýkça ve hesaplanan hash adresinde bir kelime olduðu sürece döngü devam eder
		for(j = 0; j < MAX; j++)//temp dizisindeki eski deðerin temizlenmesi
			temp[j] = '\0';
		i++;
		h = hash(key,i);//Yeni hash deðeri hesaplanýr
		
		if(!strcmp(sozluk[h],"\0")){//Yeni hash adresi boþsa
			strcpy(temp,sozluk[h]);
		}
		else{//Yeni hash adresi boþ deðilse
			
			k = 0;
			while(sozluk[h][k] != ' '){//Yeni hash adresindeki kelime temp dizisine aktarýlýr
		
				temp[k] = sozluk[h][k];
				k++;
			}	
		}
	}

	if (compare(str,temp)){//Eðer aranan kelime daha önceden hash tablosunda varsa

		control[0] = 1;
	}
	else{
		
		control[0] = 0;
	}
	control[1] = h;//Kelimenin hash tablosunda ekleneceði deðerin atanmasý
	return control;
}

char* Levenstein(char **sozluk, char **hatali, char str[]){
	
	char temp[MAX];
	char *str1[MAX];//uzaklýðý bir olan kelimelerin tutulacaðý deðiþken
	char *str2[MAX];//uzaklýðý iki olan kelimelerin tutulacaðý deðiþken
	char *kelime;
	char tmp[MAX];
	char t[MAX];//Hatalý yazýlmýþ kelimenin küçük harfle yazýlmýþ halinin saklanacaðý deðiþken
	int **dist;//Kelimeler arasýndaki uzaklýðýn hesaplanacaðý matris
	char hatalikelime[MAX] = {'\0'};
	int h;
	int k;
	int i;
	int j;
	int len1;//Birinici kelimenin uzunluðu
	int len2;//Ýkinci kelimenin uzunluðu
	int count1 = 0;//uzaklýðý bir olan kelime sayýsýný tutacak olan deðiþken
	int count2 = 0;//uzaklýðý iki olan kelime sayýsýný tutacak olan deðiþken
	int *index;
	int control;
	
	for(i = 0; i < strlen(str); i++){//Hatalý yazýlmýþ kelimedeki bütün harflerin küçük harfe çevrilmesi
		
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
	
	while(h < MAX){//sözlükteki bütün kelimelerin kontrol edilmesi
		
		if(strcmp(sozluk[h],"\0")){//Hash tablosunda ilgili adres boþ deðilse
			
			for(j = 0; j < 50; j++)//temp dizisindeki eski deðerin temizlenmesi
			temp[j] = '\0';
		
			k = 0;
			while(sozluk[h][k] != ' '){//sözlükteki kelimenin temp[] dizisine alýnmasý
			
				temp[k] = sozluk[h][k];
				k++;
			}
			temp[k]  = '\0';
		
			len1 = strlen(temp);//Sözlükteki kelimenin uzunluðu
			len2 = strlen(str);//hatalý yazýlan kelimenin uzunluðu
	
			dist = (int **)calloc(len1+1, sizeof(int *));
	
			for(i = 0; i <= len1; i++)
				dist[i] = (int *)calloc(len2+1, sizeof(int));

			for (i = 0; i <= len1; i++) dist[i][0] = i;//ilk sütunun doldurulmasý
			for (j = 0; j <= len2; j++) dist[0][j] = j;//ilk satýrýn doldurulmasý
			
			i = 1; k = 0;
			while((i <= len1) && (k != 3)){

				j = 1;
				while((j <= len2) && (k != 3)){
		
					if(temp[i-1] == t[j-1])//Eðer karþýlaþtýrýlan harfler eþitse sol çaprazýndaki deðer alýnýr
						dist[i][j] = dist[i-1][j-1];
					else//Eðer karþýlaþtýrýlan harfler farklýysa en küçük cost deðerine sahip olan sayý alýnýr  
						dist[i][j] = min(dist[i-1][j],dist[i][j-1],dist[i-1][j-1]) + 1;
	
					if(dist[i][i + len2 - len1] == 3)//Eðer köþegendeki deðer 3 ise uzaklýk hesaplamayý bitir
						k = 3;
					j++;
				}
				i++;
			}
			
			if(k != 3){//Eðer uzaklýk 1 ya da 2 ise
				
				if(dist[len1][len2] == 1){//Eðer uzaklýk 1 ise
					strcpy(str1[count1],temp);
					count1++;
				}
				else if(dist[len1][len2] == 2){//Eðer uzaklýk iki ise
					strcpy(str2[count2],temp);
					count2++;
				}
			}
		}
		
		h++;
	}
	
	if(count1){//Eðer uzaklýðý 1 olan en az bir kelime varsa 
		
		printf("%s is not in Dictionary. Did you mean: ",str);
		
		for(i = 0; i < count1-1; i++)
			printf("%s or ", str1[i]);
		printf("%s", str1[i]);		
	}
	else if(count2){//Eðer uzaklýðý 1 olan kelime yok ve uzaklýðý 2 olan en az bir kelime varsa
		
		printf("%s is not in Dictionary. Did you mean: ",str);
		
		for(i = 0; i < count2-1; i++)
			printf("%s or ", str2[i]);
		printf("%s", str2[i]);
	}
	else{//Eðer uzaklýðý 1 veya 2 olan herhangi bir kelime yoksa
		
		printf("There is not word that matches with %s ", str);
		return str;
	}
	printf("\n");
	
	control = 0;
	while(control != 1){//Kullanýcýnýn girdiði kelimeninin önerilenler listesinde olup olmadýðýnýn kontrolü

		scanf("%s",kelime);//Önerilen kelimelerden seçilecek olan kelimenin kullanýcýdan alýnmasý  
		gets(tmp);//'\n' karakterini alýr
		
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

	index = KelimeAra(str,hatali);//hatalý kelimenin hatalý kelimelerin olduðu hash tablosunda aranarak olmasý gereken hash adresinin hesaplanmasý
	
	for(i = 0; i < strlen(str); i++)//Hash adresine eklenecek hatalý kelime ve doðru hali için öncelikle hatalý kelime eklenir
		hatalikelime[i] = str[i];
	
	hatalikelime[i] = ' ';//Araya bir boþluk koyulur
	
	k = 0;
	for(i = strlen(str)+1; i < strlen(str) + 1 + strlen(kelime); i++){//Kullanýcý tarafýndan seçilen kelime yanýna yazýlýr
		
		hatalikelime[i] = kelime[k];
		k++;
	}
	hatalikelime[i] = ' ';
	strcpy(hatali[index[1]],hatalikelime);//Hatalý kelime ve kullanýcý tarafýndan seçilen doðru halinin hash adresinde saklanmasý
	return kelime;
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

int min(int num1, int num2, int num3){
	
	int min;
	
	if (num1 < num2){ // Ýlk sayý ikinci sayýdan küçükse
		
		min = num1;
		
		if (num3 < num1)// üçüncü sayý da birinci sayýdan küçükse
			min = num3;// en küçük sayý üçüncü sayýdýr
	}
	else{//ilk sayý ikinci sayýdan küçük deðilse
		
		min = num2;
		
		if (num3 < num2) //üçüncü sayý ikinci sayýdan küçükse 
			min = num3;// en küçük sayý üçüncü sayýdýr
	}
	
	return min;
}
