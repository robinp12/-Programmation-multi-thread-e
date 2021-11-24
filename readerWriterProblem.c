#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

sem_t db;
int readcount = 0;
int writecount = 0;
pthread_mutex_t mutex_readcount;
pthread_mutex_t mutex_writecount;
pthread_mutex_t mutex_writedb;
pthread_mutex_t mutex_readdb;
pthread_mutex_t z;

sem_t wsem;
sem_t rsem;

// Custom message d'erreur
void error(int err, char *msg) {
  fprintf(stderr,"%s a retourné %d message d'erreur : %s\n",msg,err);
  exit(EXIT_FAILURE);
}

int write_database(){
        pthread_mutex_lock(&mutex_writedb);

        pthread_mutex_unlock(&mutex_writedb);
}

int read_database(){
        pthread_mutex_lock(&mutex_readdb);

        pthread_mutex_unlock(&mutex_readdb);
}
void *reader(){
    while(true){
        pthread_mutex_lock(&z);
            sem_wait(&rsem);
            pthread_mutex_lock(&mutex_readcount);
                // exclusion mutuelle, readercount
                readcount=readcount+1;
                if (readcount==1) {
                    // arrivée du premier reader
                    sem_wait(&wsem);
                }
            pthread_mutex_unlock(&mutex_readcount);
            sem_post(&rsem); // libération du prochain reader
        pthread_mutex_unlock(&z);
        read_database();
        pthread_mutex_lock(&mutex_readcount);
            // exclusion mutuelle, readcount
            readcount=readcount-1;
            if(readcount==0) {
                // départ du dernier reader
                sem_post(&wsem);
            }
        pthread_mutex_unlock(&mutex_readcount);
    }
}
void *writer(){
    while(true){

        pthread_mutex_lock(&mutex_writecount);
            // section critique - writecount
            writecount=writecount+1;
            if(writecount==1) {
                // premier writer arrive
                sem_wait(&rsem);
            }
        pthread_mutex_unlock(&mutex_writecount);
        sem_wait(&wsem);
            // section critique, un seul writer à la fois
            write_database();
        sem_post(&wsem);
        pthread_mutex_lock(&mutex_writecount);
            // section critique - writecount
            writecount=writecount-1;
            if(writecount==0) {
                // départ du dernier writer
                sem_post(&rsem);
            }
        pthread_mutex_unlock(&mutex_writecount);
    }
}

int main (int argc, char *argv[]){
    
    int err;
    int reader_thread;
    int writer_thread;
    if(argc != 3){
        printf("\n 2 arguments requis (int) \n");
        return 0;
    }
    reader_thread = atoi(argv[1]);
    writer_thread = atoi(argv[2]);
    pthread_t thread_read[reader_thread];
    pthread_t thread_write[writer_thread];

    err = pthread_mutex_init(&mutex_readcount,NULL);
    if(err != 0) error(err,"mutex_init readcount");
    err = pthread_mutex_init(&mutex_writecount,NULL);
    if(err != 0) error(err,"mutex_init writecount");
    err = pthread_mutex_init(&mutex_readdb,NULL);
    if(err != 0) error(err,"mutex_init readdb");
    err = pthread_mutex_init(&mutex_writedb,NULL);
    if(err != 0) error(err,"mutex_init writedb");
    err = pthread_mutex_init(&z,NULL);
    if(err != 0) error(err,"mutex_init z");

    err = sem_init(&wsem,0,1);
    if(err != 0) error(err,"sem_init write");
    err = sem_init(&rsem,0,1);
    if(err != 0) error(err,"sem_init read");

    for(int i = 0; i < reader_thread; i++) {
        err = pthread_create(&thread_read[i],NULL,reader,NULL);
        if(err != 0) error(err,"pthread_create reader");
    }
    for(int i = 0; i < writer_thread; i++) {
        err = pthread_create(&thread_write[i],NULL,writer,NULL);
        if(err != 0) error(err,"pthread_create writer");
    }

    for(int i = 0; i < reader_thread; i++) {
        err = pthread_join(thread_read[i],NULL);
        if(err != 0) error(err,"pthread_join reader");
    }
    for(int i = 0; i < writer_thread; i++) {
        err = pthread_join(thread_write[i],NULL);
        if(err != 0) error(err,"pthread_create writer");
    }

    err = pthread_mutex_destroy(&mutex_readcount); 
    if(err != 0){error(err, "mutex_destroy mutex_readcount");}
    err = pthread_mutex_destroy(&mutex_writecount); 
    if(err != 0){error(err, "mutex_destroy mutex_writecount");}
    err = pthread_mutex_destroy(&mutex_readdb); 
    if(err != 0){error(err, "mutex_destroy mutex_readdb");}
    err = pthread_mutex_destroy(&mutex_writedb); 
    if(err != 0){error(err, "mutex_destroy mutex_writedb");}
    err = pthread_mutex_destroy(&z); 
    if(err != 0){error(err, "mutex_destroy z");}

    err = sem_destroy(&wsem);
    if(err != 0) error(err,"sem_destroy write");
    err = sem_destroy(&rsem);
    if(err != 0) error(err,"sem_destroy read");

    return(EXIT_SUCCESS);
}
