#include <ctype.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "TAS_lock.h"

#define NB_ACTIONS 6400

typedef struct {
    pthread_t thread;
    int remaining_actions;
    int thread_id;
} ThreadInfo;

void work();
void *execute_action(void *i);
void test_TAS_lock(ThreadInfo *thread);

LockTAS *lock;
ThreadInfo *threads;

int main(int argc, char *argv[]) {
    int nb_threads;

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

    if (init_TAS(&lock) != 0) {
        printf("Error, no memory available.");
        exit(EXIT_FAILURE);
    }

    threads = malloc(sizeof(ThreadInfo) * nb_threads);

    for (int i = 0; i < nb_threads; i++) {
        pthread_create(&(threads[i].thread), NULL, execute_action,
                       &(threads[i].thread_id));
        threads[i].remaining_actions = NB_ACTIONS / nb_threads;
        threads[i].thread_id = i;
    }

    for (int i = 0; i < nb_threads; i++) {
        pthread_join(threads[i].thread, NULL);
    }

    free(threads);
    exit(EXIT_SUCCESS);
}

void work() {
    while (rand() > RAND_MAX / 10000) {
    }
}

void *execute_action(void *id) {
    int id_int = *((int *)id);
    ThreadInfo current_thread = threads[id_int];

    test_TAS_lock(&current_thread);

    return NULL;
}

void test_TAS_lock(ThreadInfo *thread) {
    while (true) {
        lock_TAS(&lock);

        if (thread->remaining_actions == 0) {
            unlock_TAS(&lock);
            break;
        }

        work();
        thread->remaining_actions--;
        unlock_TAS(&lock);
    }
}