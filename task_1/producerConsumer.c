#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_NB_ELEMENTS 1024
#define BUFFER_SIZE 8

pthread_mutex_t mutex;

sem_t empty;
sem_t full;

int buffer[BUFFER_SIZE];

int nb_produced_elements = 0;
int nb_consumed_elements = 0;

// Fonction pour retourner les erreurs
void print_error(int err, char *msg) {
    fprintf(stderr, "'%s' returned error %d with message '%s'\n", msg, err,
            strerror(errno));
    exit(EXIT_FAILURE);
}

void work() {
    while (rand() > RAND_MAX / 10000) {}
}

void produce() {
    printf("producing\n");
    buffer[nb_produced_elements % BUFFER_SIZE] = rand();
    nb_produced_elements++;
}

void consume() {
    printf("consuming\n");
    buffer[nb_consumed_elements % BUFFER_SIZE] = 0;
    nb_consumed_elements++;
}

void *producer() {
    while (nb_produced_elements < MAX_NB_ELEMENTS) {
        work();
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
            if(nb_produced_elements < MAX_NB_ELEMENTS) produce();
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

void *consumer() {
    while (nb_consumed_elements < MAX_NB_ELEMENTS) {
        work();
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

            if (nb_consumed_elements < MAX_NB_ELEMENTS) consume();
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}



int main(int argc, char *argv[]) {
    int nb_producers;
    int nb_consumers;
    int err;

    // Verification des arguments
    if (argc != 3) {
        printf("Error, expected two arguments, but received %d.", (argc - 1));
        exit(EXIT_FAILURE);
    }

    if (isdigit(*argv[1])) {
        nb_producers = atoi(argv[1]);
    } else {
        printf("Error, the first argument should be an integer.");
        exit(EXIT_FAILURE);
    }

    if (isdigit(*argv[2])) {
        nb_consumers = atoi(argv[2]);
    } else {
        printf("Error, the second argument should be an integer.");
        exit(EXIT_FAILURE);
    }

    if (nb_producers < 1) {
        printf("Error, the first argument should be a positive integer.");
        exit(EXIT_FAILURE);
    }

    if (nb_consumers < 1) {
        printf("Error, the second argument should be a positive integer.");
        exit(EXIT_FAILURE);
    }

    pthread_t producers[nb_producers];
    pthread_t consumers[nb_consumers];

    // Initialisation du mutex
    err = pthread_mutex_init(&mutex, NULL);
    if (err != 0) print_error(err, "mutex_init");

    // Initialisation des semaphores
    err = sem_init(&empty, 0, BUFFER_SIZE);
    if (err != 0) print_error(err, "sem_init empty");
    err = sem_init(&full, 0, 0);
    if (err != 0) print_error(err, "sem_init full");

    // Creation des threads producteur
    for (int i = 0; i < nb_producers; i++) {
        err = pthread_create(&producers[i], NULL, producer, NULL);
        if (err != 0) print_error(err, "pthread_create producer");
    }

    // Creation des threads consomateurs
    for (int i = 0; i < nb_consumers; i++) {
        err = pthread_create(&consumers[i], NULL, consumer, NULL);
        if (err != 0) print_error(err, "pthread_create consumer");
    }

    for (int i = 0; i < nb_producers; i++) {
        err = pthread_join(producers[i], NULL);
        if (err != 0) print_error(err, "pthread_create producers");
    }

    for (int i = 0; i < nb_consumers; i++) {
        err = pthread_join(consumers[i], NULL);
        if (err != 0) print_error(err, "pthread_create consumers");
    }

    // Destruction des mutex
    err = pthread_mutex_destroy(&mutex);
    if (err != 0) {
        print_error(err, "mutex_destroy mutex");
    }

    // Destruction des semaphores
    err = sem_destroy(&empty);
    if (err != 0) print_error(err, "sem_destroy empty");
    err = sem_destroy(&full);
    if (err != 0) print_error(err, "sem_destroy full");

    exit(EXIT_SUCCESS);
}