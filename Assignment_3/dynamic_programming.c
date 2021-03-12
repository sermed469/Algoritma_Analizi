#include <stdio.h>
#include <stdlib.h>

int main(){
	
	int macsayisi;
	double p;
	double **matris;//A'n�n ve B'nin kalan ma�lar� �zerinden olas�l�k hesab� yap�laca�� i�in matris kullan�l�r
	int i;
	int j;
	int n;
	
	printf("Mac sayisini giriniz: ");
	scanf("%d", &macsayisi);
	
	printf("A takiminin bir maci kazanma olasiligini giriniz: ");
	scanf("%lf", &p);
	
	n = (macsayisi / 2) + 1;//Seriyi kazanabilmek i�in Serideki ma�lar�n yar�s�ndan bir fazla ma� kazanmal�
	
	matris = (double **)calloc(n+1,sizeof(double *));//Matris i�in dinamik olarak haf�zada yer a��lmas�
	for(i = 0; i <= n; i++)
		matris[i] = (double *)calloc(n+1,sizeof(double *));
		
	matris[0][0] = -1.0;//Hem A'n�n hem de B'nin de seriyi kazanmak i�in 0 ma�� kalamaz. �kisnden biri kazanmal�
	
	for(i = 1; i <= n; i++)//Sat�r k�sm� A'n�n kalan ma�lar� olsun 
		matris[i][0] = 0.0;//B'n�n kazanmak i�in 0 ma�� kald�ysa A tak�m� bu seriyi kazanamaz. Bu durUmda ihtimal 0'd�r
	
	for(j = 1; j <= n; j++)//S�tun k�sm� A'n�n kalan ma�lar� olsun
		matris[0][j] = 1.0;//A'n�n kazanmak i�in 0 ma�� kald�ysa A tak�m� bu seriyi kazanm��t�r. Bu durumda ihtimal 1'dir.
		
	for(i = 1; i <= n; i++){
		for(j = 1; j <= n; j++){
			matris[i][j] = matris[i-1][j]*p + matris[i][j-1]*(1-p);//Rek�rans ba��nt�s� ile ilgili olas�l���n hesaplanmas�
		}	
	}
	
	printf("A takiminin seriyi kazanma ihtimali: %f", matris[n][n]); //Matrisin sol alt h�cresindeki de�er A ve B tak�mlar� herhangi bir ma� yapmad��� durumda A'n�n kazanma ihtimalidir
	
	return 0;
}


