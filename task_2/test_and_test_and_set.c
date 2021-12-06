#include <ctype.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "TATAS_lock.h"

#define NB_ACTIONS 6400

// Structure représentant un thread et tenant à jour le nombre d'actions qu'il
// lui reste à effectuer
typedef struct {
    pthread_t thread;
    int remaining_actions;
} ThreadInfo;

void work();
void *execute_action(void *i);
void test_TATAS_lock(ThreadInfo *thread);

LockTATAS *lock;
ThreadInfo *threads;

int main(int argc, char *argv[]) {
    int nb_threads;

    // vérification des arguments du programme
    if (argc != 2) {
        printf("Error, expected one argument, but received %d.", (argc - 1));
        exit(EXIT_FAILURE);
    }

    if (isdigit(*argv[1])) {
        nb_threads = atoi(argv[1]);
    } else {
        printf("Error, the first argument should be an integer.");
        exit(EXIT_FAILURE);
    }

    if (nb_threads < 1) {
        printf("Error, the first argument should be a positive integer.");
        exit(EXIT_FAILURE);
    }

    // initialisation du verrou TATAS
    lock = malloc(sizeof(LockTATAS));
    if (init_TATAS(lock) != 0) {
        printf("Error, no memory available.");
        exit(EXIT_FAILURE);
    }

    // initialisation et lancement des threads
    threads = malloc(sizeof(ThreadInfo) * nb_threads);

    for (int i = 0; i < nb_threads; i++) {
        pthread_create(&(threads[i].thread), NULL, execute_action, &i);

        threads[i].remaining_actions = NB_ACTIONS / nb_threads;
    }

    for (int i = 0; i < nb_threads; i++) {
        pthread_join(threads[i].thread, NULL);
    }

    // libération de la mémoire allouée pour le verrou et les threads
    free(lock);
    free(threads);
    exit(EXIT_SUCCESS);
}

// Fonction qui simule la section critique d'un thread
void work() {
    while (rand() > RAND_MAX / 10000) {
    }
}

// Fonction qui sera exécutée par les threads
void *execute_action(void *id) {
    int id_int = *((int *)id);
    ThreadInfo current_thread = threads[id_int];

    test_TATAS_lock(&current_thread);

    return NULL;
}

void test_TATAS_lock(ThreadInfo *thread) {
    while (true) {
        lock_TATAS(lock);

        // le thread a effectué son nombre maximal d'actions
        if (thread->remaining_actions == 0) {
            unlock_TATAS(lock);
            break;
        }

        work();
        thread->remaining_actions--;
        unlock_TATAS(lock);
    }
}
