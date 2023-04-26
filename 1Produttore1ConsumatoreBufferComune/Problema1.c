#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define VOLTE 5 //numero volte che si alternano scrittura/prelievo
/*
P() = entrata nel semaforo 
V() = uscita dal semaforo
*/
//memoria condivisa
int buffer; //condivisa per scambiare il dato
int conta = 0; //condivsa per contare le ripetizioni
static sem_t mio_sem;//inizializzazione variabile semaforo
//PRODUTTORE 
void *produci(void *arg){
	int avanti = 1; //variabile per il controllo del ciclo
	while(avanti){ 
		sem_wait(&mio_sem);// P()
		/*SEZIONE CRITICA*/
		if(conta<VOLTE){
			if(buffer == 0){ // se il dato è stato consumato
				buffer = conta+1; //aggiorno il dato del consumatore
				printf("T1: scritto %d \n",buffer);
			}			
		}else{
			avanti = 0; // fine ripetizione ciclo
			printf("T1 ha finito \n");
		}
		/*FINE SEZIONE CRITICA*/
		sem_post(&mio_sem);//V()
	}
	pthread_exit(0);
		return NULL;
}
//CONSUMATORE
void *consuma(void *threadid){
	int avanti = 1;//variabile per il controllo del ciclo
	while(avanti){
		sem_wait(&mio_sem);//P()
		/*SEZIONE CRITICA*/
		if(conta<VOLTE){
			if(buffer>0){// se il dato è stato consumato
				conta = buffer;//incremento contatore ripetizione
				buffer = 0;//"consuma" il dato
				printf("T2: letto %d \n",conta);
			}
		}else{
			avanti = 0;//fine ripetizione ciclo
			printf("T2 ha finito \n");
		}
		/*FINE SEZIONE CRITICA*/
		sem_post(&mio_sem);//V()
	}
	pthread_exit(0);
	return NULL;
}

int main(void) {
pthread_t tid1,tid2;
sem_init(&mio_sem,0,1); //falso
if(pthread_create(&tid1,NULL,produci,NULL)<0){
	fprintf(stderr,"errore nella creazione del thread 1\n");
	exit(1);
}
if(pthread_create(&tid2,NULL,consuma,NULL)<0){
		fprintf(stderr,"errore nella creazione del thread 2\n");
	exit(2);
}
pthread_join (tid1,NULL);
pthread_join (tid2,NULL);
}
