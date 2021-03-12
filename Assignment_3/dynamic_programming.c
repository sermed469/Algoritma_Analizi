#include <stdio.h>
#include <stdlib.h>

int main(){
	
	int macsayisi;
	double p;
	double **matris;//A'nýn ve B'nin kalan maçlarý üzerinden olasýlýk hesabý yapýlacaðý için matris kullanýlýr
	int i;
	int j;
	int n;
	
	printf("Mac sayisini giriniz: ");
	scanf("%d", &macsayisi);
	
	printf("A takiminin bir maci kazanma olasiligini giriniz: ");
	scanf("%lf", &p);
	
	n = (macsayisi / 2) + 1;//Seriyi kazanabilmek için Serideki maçlarýn yarýsýndan bir fazla maç kazanmalý
	
	matris = (double **)calloc(n+1,sizeof(double *));//Matris için dinamik olarak hafýzada yer açýlmasý
	for(i = 0; i <= n; i++)
		matris[i] = (double *)calloc(n+1,sizeof(double *));
		
	matris[0][0] = -1.0;//Hem A'nýn hem de B'nin de seriyi kazanmak için 0 maçý kalamaz. Ýkisnden biri kazanmalý
	
	for(i = 1; i <= n; i++)//Satýr kýsmý A'nýn kalan maçlarý olsun 
		matris[i][0] = 0.0;//B'nÝn kazanmak için 0 maçý kaldýysa A takýmý bu seriyi kazanamaz. Bu durUmda ihtimal 0'dýr
	
	for(j = 1; j <= n; j++)//Sütun kýsmý A'nýn kalan maçlarý olsun
		matris[0][j] = 1.0;//A'nýn kazanmak için 0 maçý kaldýysa A takýmý bu seriyi kazanmýþtýr. Bu durumda ihtimal 1'dir.
		
	for(i = 1; i <= n; i++){
		for(j = 1; j <= n; j++){
			matris[i][j] = matris[i-1][j]*p + matris[i][j-1]*(1-p);//Rekürans baðýntýsý ile ilgili olasýlýðýn hesaplanmasý
		}	
	}
	
	printf("A takiminin seriyi kazanma ihtimali: %f", matris[n][n]); //Matrisin sol alt hücresindeki deðer A ve B takýmlarý herhangi bir maç yapmadýðý durumda A'nýn kazanma ihtimalidir
	
	return 0;
}


