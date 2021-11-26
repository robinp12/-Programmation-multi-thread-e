#include <ctype.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_NB_ELEMENTS 1024
#define BUFFER_SIZE 8

void work();
void produce();
void consume();
void producer();
void consumer();

pthread_mutex_t mutex;
sem_t empty;
sem_t full;
int buffer[BUFFER_SIZE];

int nb_produced_elements = 0;
int nb_consumed_elements = 0;

int main(int argc, char *argv[]) {
    int nb_producers;
    int nb_consumers;

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

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    for (int i = 0; i < nb_producers; i++) {
        pthread_create(&producers[i], NULL, (void *)producer, (void *)&i);
    }

    for (int i = 0; i < nb_consumers; i++) {
        pthread_create(&consumers[i], NULL, (void *)consumer, (void *)&i);
    }

    for (int i = 0; i < nb_producers; i++) {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < nb_consumers; i++) {
        pthread_join(consumers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    exit(EXIT_SUCCESS);
}

void work() {
    while (rand() > RAND_MAX / 10000) {
        // doing some very important work
    }
}

void produce() {
    buffer[nb_produced_elements % BUFFER_SIZE] = rand();
    nb_produced_elements++;
}

void consume() {
    buffer[nb_consumed_elements % BUFFER_SIZE] = 0;
    nb_consumed_elements++;
    work();
}

void producer() {
    while (nb_produced_elements < MAX_NB_ELEMENTS) {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        if (nb_produced_elements == MAX_NB_ELEMENTS) {
            pthread_mutex_unlock(&mutex);
            sem_post(&full);
        }

        produce();
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

void consumer() {
    while (nb_consumed_elements < MAX_NB_ELEMENTS) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        if (nb_consumed_elements == MAX_NB_ELEMENTS) {
            pthread_mutex_unlock(&mutex);
            sem_post(&empty);
        }

        consume();
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}
