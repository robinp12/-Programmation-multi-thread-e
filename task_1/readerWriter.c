#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

sem_t db;
int readcount = 0;
int writecount = 0;
int writing = 640;
int reading = 2560;

pthread_mutex_t mutex_readcount;
pthread_mutex_t mutex_writecount;
pthread_mutex_t mutex_writedb;
pthread_mutex_t mutex_readdb;
pthread_mutex_t z;

sem_t wsem;
sem_t rsem;

// Fonction pour retourner les erreurs
void print_error(int err, char *msg) {
    fprintf(stderr, "'%s' returned error %d with message '%s'\n", msg, err,
            strerror(errno));
    exit(EXIT_FAILURE);
}

void write_database() { printf("write database\n"); }

void read_database() { printf("read database\n"); }

void process_data() {
    while (rand() > RAND_MAX / 10000) {
        // processing data
    }
}

void prepare_data() {
    while (rand() > RAND_MAX / 10000) {
        // preparing data
    }
}

void *reader(void *args) {
    while (true) {
        pthread_mutex_lock(&z);
            sem_wait(&rsem);
            pthread_mutex_lock(&mutex_readcount);

            if (reading <= 0) {
                pthread_mutex_unlock(&mutex_readcount);
                sem_post(&rsem);
                pthread_mutex_unlock(&z);

                break;
            } else {
                reading--;
            }

            // exclusion mutuelle, readercount
            readcount = readcount + 1;
            if (readcount == 1) {
                // arrivée du premier reader
                sem_wait(&wsem);
            }

            pthread_mutex_unlock(&mutex_readcount);
            sem_post(&rsem);  // libération du prochain reader
        pthread_mutex_unlock(&z);
        read_database();
        pthread_mutex_lock(&mutex_readcount);

        // exclusion mutuelle, readcount
        readcount = readcount - 1;
        if (readcount == 0) {
            // départ du dernier reader
            sem_post(&wsem);
        }

        pthread_mutex_unlock(&mutex_readcount);
        process_data();
    }

    return NULL;
}

void *writer(void *args) {
    while (true) {
        prepare_data();
        pthread_mutex_lock(&mutex_writecount);

        if (writing <= 0) {
            pthread_mutex_unlock(&mutex_writecount);
            break;
        } else {
            writing--;
        }

        // section critique - writecount
        writecount = writecount + 1;
        if (writecount == 1) {
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
        writecount = writecount - 1;
        if (writecount == 0) {
            // départ du dernier writer
            sem_post(&rsem);
        }

        pthread_mutex_unlock(&mutex_writecount);
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
    err = pthread_mutex_init(&mutex_readcount, NULL);
    if (err != 0) print_error(err, "mutex_init readcount");
    err = pthread_mutex_init(&mutex_writecount, NULL);
    if (err != 0) print_error(err, "mutex_init writecount");
    err = pthread_mutex_init(&mutex_readdb, NULL);
    if (err != 0) print_error(err, "mutex_init readdb");
    err = pthread_mutex_init(&mutex_writedb, NULL);
    if (err != 0) print_error(err, "mutex_init writedb");
    err = pthread_mutex_init(&z, NULL);
    if (err != 0) print_error(err, "mutex_init z");

    // Initialisation des semaphores read / write
    err = sem_init(&wsem, 0, 1);
    if (err != 0) print_error(err, "sem_init write");
    err = sem_init(&rsem, 0, 1);
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

    // Destruction des mutex
    err = pthread_mutex_destroy(&mutex_readcount);
    if (err != 0) {
        print_error(err, "mutex_destroy mutex_readcount");
    }
    err = pthread_mutex_destroy(&mutex_writecount);
    if (err != 0) {
        print_error(err, "mutex_destroy mutex_writecount");
    }
    err = pthread_mutex_destroy(&mutex_readdb);
    if (err != 0) {
        print_error(err, "mutex_destroy mutex_readdb");
    }
    err = pthread_mutex_destroy(&mutex_writedb);
    if (err != 0) {
        print_error(err, "mutex_destroy mutex_writedb");
    }
    err = pthread_mutex_destroy(&z);
    if (err != 0) {
        print_error(err, "mutex_destroy z");
    }

    // Destruction des semaphores
    err = sem_destroy(&wsem);
    if (err != 0) print_error(err, "sem_destroy write");
    err = sem_destroy(&rsem);
    if (err != 0) print_error(err, "sem_destroy read");

    exit(EXIT_SUCCESS);
}
