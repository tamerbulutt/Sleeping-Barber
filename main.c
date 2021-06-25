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

	sem_wait(&empty); //Buffer doluysa bekletiyoruz yoksa bir kiþi alýp boþ yer sayýsýndan 1 düþürtüyoruz
	pthread_mutex_lock(&mutex); //Kritik bölgeyi kilitliyoruz
	
	printf("\nBerber koltugu bosta musteri oturdu, koltuk: %d\n",counter+1);
	
	write(koltuk); //Buffer'a item ekleniyor
	
	pthread_mutex_unlock(&mutex); 
	
	sem_post(&full); //Bekleyen müþteri varsa uyandýr yoksa bufferdaki dolu yer sayýsýný 1 arttýr
}

int read(){
	return(buffer[--counter]);
}

void * musteri (void * param){
	int koltuk;
	
	sem_wait(&full); //Buffer boþ ise bekle yoksa dolu yer sayýsýný 1 azalt
	
	pthread_mutex_lock(&mutex); //Kritik bölgeyi kilitliyoruz
	
	koltuk=read(); //Bufferdan boþta olan koltuðu oku
	
	printf("\nMusterinin trasi sona erdi koltuk bosa cikti, koltuk: %d\n",counter+1);
	printf("\nBerber yeni musteri geleseye kadar uyuyor..\n");
	sleep(3);
	
	pthread_mutex_unlock(&mutex);//Kritik bölge kilidini açýyoruz sýradaki için
	
	sem_post(&empty); 	//Bekleyen berber varsa uyandýr yoksa bufferdaki boþ yer sayýsýný 1 arttýr
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
		pthread_create(&tBerber[i],NULL,berber,NULL); //2 adet berber oluþturuyoruz.
		
	for(i=0; i<musteriSayisi; i++)
		pthread_create(&tMusteri[i],NULL,musteri,NULL); //Girilen sayý kadar müþteri oluþturuyoruz.
		
	for(i=0; i<2; i++)
		pthread_join(tBerber[i],NULL);   //Berber threadlerini join ediyoruz
		
	for(i=0; i<musteriSayisi; i++)
		pthread_join(tMusteri[i],NULL);   //Müþteri threadlerini join ediyoruz.
	
	sleep(5);
	printf("\nTum musterilerin trasi bitti, dukkan kapandi..");
	}
		
	
	exit(0);

	return 0;
}
