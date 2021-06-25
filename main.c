#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

#define buffersize 5

pthread_mutex_t mutex;
pthread_t tBerber[3],tMusteri[6];
sem_t full, empty;
int counter;
int buffer[buffersize];

void initialize(){
	pthread_mutex_init(&mutex,NULL);
	sem_init(&full,1,0);
	sem_init(&empty,1,buffersize);
	counter = 0;
}

void write(int koltuk){
	buffer[counter++] = koltuk;
}

void * berber(void * param)
{
	int koltuk;
	koltuk = rand();

	sem_wait(&empty); //Buffer doluysa bekletiyoruz yoksa bir ki�i al�p bo� yer say�s�ndan 1 d���rt�yoruz
	pthread_mutex_lock(&mutex); //Kritik b�lgeyi kilitliyoruz
	
	printf("\nBerber koltugu bosta musteri oturdu, koltuk: %d\n",counter+1);
	
	write(koltuk); //Buffer'a item ekleniyor
	
	pthread_mutex_unlock(&mutex); 
	
	sem_post(&full); //Bekleyen m��teri varsa uyand�r yoksa bufferdaki dolu yer say�s�n� 1 artt�r
}

int read(){
	return(buffer[--counter]);
}

void * musteri (void * param){
	int koltuk;
	
	sem_wait(&full); //Buffer bo� ise bekle yoksa dolu yer say�s�n� 1 azalt
	
	pthread_mutex_lock(&mutex); //Kritik b�lgeyi kilitliyoruz
	
	koltuk=read(); //Bufferdan bo�ta olan koltu�u oku
	
	printf("\nMusterinin trasi sona erdi koltuk bosa cikti, koltuk: %d\n",counter+1);
	printf("\nBerber yeni musteri geleseye kadar uyuyor..\n");
	sleep(3);
	
	pthread_mutex_unlock(&mutex);//Kritik b�lge kilidini a��yoruz s�radaki i�in
	
	sem_post(&empty); 	//Bekleyen berber varsa uyand�r yoksa bufferdaki bo� yer say�s�n� 1 artt�r
}

int main() {
	
	int musteriSayisi,i;
	initialize();
		
	printf("Henuz musteri gelmedi berber uyuyor..\n");
	sleep(3);	
	
	printf("\nBerber sayisi => 2\n");	
	
	printf("\nMusteri sayisini gir: ");
	scanf("%d",&musteriSayisi);
	
	if(musteriSayisi == 0)
		printf("Hic musteri yok, berber tum gun uyudu...");
	else{
		for(i=0; i<2; i++)
		pthread_create(&tBerber[i],NULL,berber,NULL); //2 adet berber olu�turuyoruz.
		
	for(i=0; i<musteriSayisi; i++)
		pthread_create(&tMusteri[i],NULL,musteri,NULL); //Girilen say� kadar m��teri olu�turuyoruz.
		
	for(i=0; i<2; i++)
		pthread_join(tBerber[i],NULL);   //Berber threadlerini join ediyoruz
		
	for(i=0; i<musteriSayisi; i++)
		pthread_join(tMusteri[i],NULL);   //M��teri threadlerini join ediyoruz.
	
	sleep(5);
	printf("\nTum musterilerin trasi bitti, dukkan kapandi..");
	}
		
	
	exit(0);

	return 0;
}
