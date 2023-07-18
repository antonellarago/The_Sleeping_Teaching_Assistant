#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

////// COSTANTI E VARIABILI GLOBALI //////

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

////// PROTOTIPI //////

void *threadTeachingAssistant(void*);
void *threadStudente(void* IDStudente);

int main(int argc, char const *argv[]){
	int totStudenti;
	cout<<"\tStudenti in arrivo..."<<endl;
	sleep(3);
	if(argc > 1){
		totStudenti = stoi(argv[1]);
		cout<<"\tCaricamento completato degli studenti inseriti."<<endl;
		sleep(1);
	} else{
		totStudenti = STUDENTI;
		cout<<"\tCaricamento completato di "<< STUDENTI <<" studenti."<<endl;
		sleep(1);
	}
	cout<<"\n ---------------- Programma in esecuzione ----------------\n "<<endl;
	sleep(1);
	int identificatoriStudenti[totStudenti];
	pthread_t studenti[totStudenti];
	pthread_t teaching_assistant;
	sem_init(&semaforoStudenti, 0, 0);
	sem_init(&semaforoTeaching, 0, 1);
	pthread_mutex_init(&mutex, NULL);
	pthread_create (&teaching_assistant, NULL, threadTeachingAssistant, NULL);
	for (int i = 0; i < totStudenti; ++i){
		identificatoriStudenti[i] = i + 1;
		pthread_create(&studenti[i], NULL, threadStudente, (void*)&identificatoriStudenti[i]);
	}
	pthread_join(teaching_assistant, NULL);
	for (int i = 0; i < totStudenti; ++i){
		pthread_join(studenti[i], NULL);
	}
	return 0;
}

void *threadTeachingAssistant(void *){
	while(true){
		if(studentiInAttesa > 0){
			dorme = false;
			sem_wait(&semaforoStudenti);
			pthread_mutex_lock(&mutex);
			int tempo = rand() % 8;
			cout<<" * * * Lo studente "<<sedie[successivo]<<" ora e' entrato nello studio. * * *"<< endl;
			studentiInAttesa--;
			cout<<"Lo studente ha aspettato "<<studentiInAttesa<<" studente/i prima di entrare."<<endl;
			sedie[successivo] = 0;
			successivo = (successivo + 1) % NUM_SEDIE;
			sleep(tempo);
			pthread_mutex_unlock(&mutex);
			sem_post(&semaforoTeaching);
		} else if(!dorme){
			cout<<"\n (-_-)zzZ \t Non ci sono studenti che hanno bisogno di aiuto. L'assistente puo' riposare \t (-_-)zzZ \n"<<endl;
			dorme = true;
		}
	}
}

void *threadStudente(void* IDStudente){
	int id = *(int*)IDStudente;
	while(true){
		//cout<<"Lo studente "<< id <<" sta programmando "<<endl;
		int tempo = rand() % 5;
		sleep(tempo);
		pthread_mutex_lock(&mutex);
		if(studentiInAttesa < NUM_SEDIE){
			sedie[pos] = id;
			studentiInAttesa++;
			pos = (pos + 1) % NUM_SEDIE;
			cout<<"Lo studente " << id << " ha bisogno di aiuto e sta aspettando in sala di attesa \n";
			cout<<"---> In totale ci sono " << studentiInAttesa << " studenti in attesa <--- \n";
			pthread_mutex_unlock(&mutex);
			sem_post(&semaforoStudenti);
			sem_wait(&semaforoTeaching);
		} else {
			pthread_mutex_unlock(&mutex);
			cout<<"*** OPS, sono terminati i posti in sala di attesa. Lo studente " << id << " tornerà piu' tardi (>_<) ***\n";
		}
	}
}
