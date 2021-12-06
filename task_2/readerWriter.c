#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include "semaphore.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Semaphore db;
int readcount = 0;
int writecount = 0;
int writing = 640;
int reading = 2560;

LockTAS mutex_readcount;
LockTAS mutex_writecount;
LockTAS mutex_writedb;
LockTAS mutex_readdb;
LockTAS z;

Semaphore wsem;
Semaphore rsem;

// Fonction pour retourner les erreurs
void print_error(int err, char *msg) {
    fprintf(stderr, "'%s' returned error %d with message '%s'\n", msg, err,
            strerror(errno));
    exit(EXIT_FAILURE);
}

void write_database() {
    printf("write database\n");
}

void read_database() {
    printf("read database\n");
}

// Fonction appelée par les lecteurs, qui simule une lecture de données
void process_data() {
    while (rand() > RAND_MAX / 10000) {
        // processing data
    }
}

// Fonction appelée par les écrivains, qui simule une création de données
void prepare_data() {
    while (rand() > RAND_MAX / 10000) {
        // preparing data
    }
}

// Fonction appelée par les threads lecteurs
void *reader(void *args) {
    while (true) {
        lock_TAS(&z);
        semaphore_wait(&rsem);
        lock_TAS(&mutex_readcount);

        if (reading <= 0) {
            unlock_TAS(&mutex_readcount);
            semaphore_post(&rsem);
            unlock_TAS(&z);

            break;
        } else {
            reading--;
        }

        // exclusion mutuelle, readercount
        readcount = readcount + 1;
        if (readcount == 1) {
            // arrivée du premier reader
            semaphore_wait(&wsem);
        }

        unlock_TAS(&mutex_readcount);
        semaphore_post(&rsem);  // libération du prochain reader
        unlock_TAS(&z);
        read_database();
        lock_TAS(&mutex_readcount);

        // exclusion mutuelle, readcount
        readcount = readcount - 1;
        if (readcount == 0) {
            // départ du dernier reader
            semaphore_post(&wsem);
        }

        unlock_TAS(&mutex_readcount);
        process_data();
    }

    return NULL;
}

// Fonction appelée par les threads écrivains
void *writer(void *args) {
    while (true) {
        prepare_data();
        lock_TAS(&mutex_writecount);

        if (writing <= 0) {
            unlock_TAS(&mutex_writecount);
            break;
        } else {
            writing--;
        }

        // section critique - writecount
        writecount = writecount + 1;
        if (writecount == 1) {
            // premier writer arrive
            semaphore_wait(&rsem);
        }

        unlock_TAS(&mutex_writecount);
        semaphore_wait(&wsem);

        // section critique, un seul writer à la fois
        write_database();
        semaphore_post(&wsem);
        lock_TAS(&mutex_writecount);

        // section critique - writecount
        writecount = writecount - 1;
        if (writecount == 0) {
            // départ du dernier writer
            semaphore_post(&rsem);
        }

        unlock_TAS(&mutex_writecount);
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    int err;
    int nb_reader_threads;
    int nb_writer_threads;

    // Verification des arguments
    if (argc != 3) {
        printf("Error, expected two arguments, but received %d.", (argc - 1));
        exit(EXIT_FAILURE);
    }

    if (isdigit(*argv[1])) {
        nb_reader_threads = atoi(argv[1]);
    } else {
        printf("Error, the first argument should be an integer.");
        exit(EXIT_FAILURE);
    }

    if (isdigit(*argv[2])) {
        nb_writer_threads = atoi(argv[2]);
    } else {
        printf("Error, the second argument should be an integer.");
        exit(EXIT_FAILURE);
    }

    if (nb_reader_threads < 1) {
        printf("Error, the first argument should be a positive integer.");
        exit(EXIT_FAILURE);
    }

    if (nb_writer_threads < 1) {
        printf("Error, the second argument should be a positive integer.");
        exit(EXIT_FAILURE);
    }

    pthread_t thread_read[nb_reader_threads];
    pthread_t thread_write[nb_writer_threads];

    // Initialisation des mutex
    err = init_TAS(&mutex_readcount);
    if (err != 0) print_error(err, "mutex_init readcount");
    err = init_TAS(&mutex_writecount);
    if (err != 0) print_error(err, "mutex_init writecount");
    err = init_TAS(&mutex_readdb);
    if (err != 0) print_error(err, "mutex_init readdb");
    err = init_TAS(&mutex_writedb);
    if (err != 0) print_error(err, "mutex_init writedb");
    err = init_TAS(&z);
    if (err != 0) print_error(err, "mutex_init z");

    // Initialisation des semaphores read / write
    err = semaphore_init(&wsem,1);
    if (err != 0) print_error(err, "sem_init write");
    err = semaphore_init(&rsem,1);
    if (err != 0) print_error(err, "sem_init read");

    // Creation des thread reader / writer
    for (int i = 0; i < nb_reader_threads; i++) {
        err = pthread_create(&thread_read[i], NULL, reader, NULL);
        if (err != 0) print_error(err, "pthread_create reader");
    }
    for (int i = 0; i < nb_writer_threads; i++) {
        err = pthread_create(&thread_write[i], NULL, writer, NULL);
        if (err != 0) print_error(err, "pthread_create writer");
    }

    for (int i = 0; i < nb_reader_threads; i++) {
        err = pthread_join(thread_read[i], NULL);
        if (err != 0) print_error(err, "pthread_join reader");
    }
    for (int i = 0; i < nb_writer_threads; i++) {
        err = pthread_join(thread_write[i], NULL);
        if (err != 0) print_error(err, "pthread_create writer");
    }

    exit(EXIT_SUCCESS);
}
