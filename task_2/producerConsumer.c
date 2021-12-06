#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include "semaphore.h"
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_NB_ELEMENTS 1024
#define BUFFER_SIZE 8

LockTAS mutex;

Semaphore empty;
Semaphore full;

int buffer[BUFFER_SIZE];

int nb_produced_elements = 0;
int nb_consumed_elements = 0;

// Fonction pour retourner les erreurs
void print_error(int err, char *msg) {
    fprintf(stderr, "'%s' returned error %d with message '%s'\n", msg, err,
            strerror(errno));
    exit(EXIT_FAILURE);
}

// Fonction qui simule le travail effectué pour produire/consommer un élément
void work() {
    while (rand() > RAND_MAX / 10000) {}
}

// Fonction appelée par le producteur pour produire un nouvel élément
void produce() {
    printf("producing\n");
    buffer[nb_produced_elements % BUFFER_SIZE] = rand();
    nb_produced_elements++;
}

// Fonction appelée par le consommateur pour consommer un élément
void consume() {
    printf("consuming\n");
    buffer[nb_consumed_elements % BUFFER_SIZE] = 0;
    nb_consumed_elements++;
}

// Fonction qui sera exécutée par les threads producteurs
void *producer() {
    while (nb_produced_elements < MAX_NB_ELEMENTS) {
        work();
        semaphore_wait(&empty);
        lock_TAS(&mutex);
            // les producteurs ont produit le nombre maximal d'éléments
            if(nb_produced_elements < MAX_NB_ELEMENTS) produce();
        unlock_TAS(&mutex);
        semaphore_post(&full);
    }
}

// Fonction qui sera exécutée par les threads consommateurs
void *consumer() {
    while (nb_consumed_elements < MAX_NB_ELEMENTS) {
        work();
        semaphore_wait(&full);
        lock_TAS(&mutex);
             // les consommateurs ont consommé le nombre maximal d'éléments
            if (nb_consumed_elements < MAX_NB_ELEMENTS) consume();
        unlock_TAS(&mutex);
        semaphore_post(&empty);
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
    err = init_TAS(&mutex);
    if (err != 0) print_error(err, "mutex_init");

    // Initialisation des semaphores
    err = semaphore_init(&empty, BUFFER_SIZE);
    if (err != 0) print_error(err, "sem_init empty");
    err = semaphore_init(&full, 0);
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

    exit(EXIT_SUCCESS);
}