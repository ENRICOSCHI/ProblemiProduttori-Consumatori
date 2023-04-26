#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE]; // dimensione del buffer

/*Nell'inizializzazione metti e togli sono uguali*/
int metti = 0; // indice di scrittura nel buffer
int togli = 0;  // indice di lettura dal buffer

sem_t empty; // semaforo contatore vuoto
sem_t full;  // semaforo contatore pieno
pthread_mutex_t mutex; // mutex per garantire accesso esclusivo al buffer

void* producer(int item) {
    while(1) {
        sem_wait(&empty); //buffer vuoto
        pthread_mutex_lock(&mutex);
        buffer[metti] = item;
        printf("Il PRODUTTORE ha prodotto l'oggetto: %d nell'indice: %d\n", item, metti);
        metti = (metti + 1) % BUFFER_SIZE;
        item++;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);//buffer pieno
    }
}

void* consumer() {
    while(1) {
        sem_wait(&full);//buffer pieno
        pthread_mutex_lock(&mutex);
        int item = buffer[togli];
        printf("Il CONSUMATORE ha consumato: %d dall'indice:'%d\n", item, togli);
        togli = (togli + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);//buffer vuoto
    }
}

int main() {
pthread_t prod_thread, cons_thread;
int item = 1;
sem_init(&empty, 0, BUFFER_SIZE); // inizializza semaforo empty al valore BUFFER_SIZE
sem_init(&full, 0, 0); // inizializza semaforo full al valore 0
pthread_mutex_init(&mutex, NULL); // inizializza il mutex
pthread_create(&prod_thread, NULL, producer, NULL); // crea il thread produttore
pthread_create(&cons_thread, NULL, consumer, NULL); // crea il thread consumatore
pthread_join(prod_thread, NULL); // attesa terminazione del thread produttore
pthread_join(cons_thread, NULL); // attesa terminazione del thread consumatore
return 0;
}
