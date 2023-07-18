#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

const int NUM_SEDIE = 3;
const int STUDENTI = 8;

sem_t semaforoStudenti;
sem_t semaforoTeaching;

pthread_mutex_t mutex;

bool dorme = false;
int successivo = 0;
int studentiInAttesa = 0;
int sedie[NUM_SEDIE];
int pos = 0;

void *threadTeachingAssistant(void*);

int main(int argc, char const *argv[]){
	int totStudenti;
	cout<<"Studenti in arrivo..."<<endl;
	sleep(3);
	if(argc > 1){
		totStudenti = stoi(argv[1]);
		cout<<"Caricamento completato degli studenti inseriti "<<endl;
		sleep(1);
	} else{
		totStudenti = STUDENTI;
		cout<<"Caricamento completato degli studenti di default"<<endl;
		sleep(1);
	}
	cout<<"Programma in esecuzione..."<<endl;
	sleep(1);
	int identificatoriStudenti[totStudenti];
	pthread_t studenti[totStudenti];
	pthread_t teaching_assistant;
	sem_init(&semaforoStudenti, 0, 0);
	sem_init(&semaforoTeaching, 0, 1);
	pthread_mutex_init(&mutex, NULL);
	pthread_create (&teaching_assistant, NULL, threadTeachingAssistant, NULL);

	
}

void *threadTeachingAssistant(void *){
	while(true){
		if(studentiInAttesa > 0){
			dorme = false;
			sem_wait(&semaforoStudenti);
			pthread_mutex_lock(&mutex);
			int tempo = rand() % 8;
			cout<<"Lo studente : "<<sedie[successivo]<<" ora e' entrato nello studio.\n";
			studentiInAttesa--;
			cout<<"Lo studente ha aspettato "<<studentiInAttesa<<" studente/i prima di entrare."<<endl;
			sedie[successivo] = 0;
			successivo = (successivo + 1) % NUM_SEDIE;
			sleep(tempo);
			pthread_mutex_unlock(&mutex);
			sem_post(&semaforoTeaching);
			} else if(!dorme){
				cout<<"Non ci sono studenti che hanno bisogno di aiuto. L'assistente puo' riposare"<<endl;
				dorme = true;
			}
	}
}