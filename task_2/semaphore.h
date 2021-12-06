#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

#include "TAS_lock.h"

// Structure représentant notre implémentation d'un sémaphore
typedef struct {
    int state;
    LockTAS *lock;
} Semaphore;

// Fonction utilisée pour initialiser un sémaphore
int semaphore_init(Semaphore *sem, int initial_state) {
    sem = malloc(sizeof(Semaphore));
    if (sem == NULL) {
        return -1;
    }

    sem->state = initial_state;
    init_TAS(sem->lock);
    return 0;
}

// Fonction qui bloque les threads tant que la ressource protégée
// par le sémaphore est utilisée, et décrémente le sémaphore
int semaphore_wait(Semaphore *sem) {
    while (sem->state == 0) {
        lock_TAS(sem->lock);
        --sem->state;
        unlock_TAS(sem->lock);
    }
    return 0;
}

// Fonction qui rend un des threads bloqués actif et incrémente donc le sémaphore
int semaphore_post(Semaphore *sem) {
    lock_TAS(sem->lock);
    ++sem->state;
    unlock_TAS(sem->lock);

    if (sem->state > 0) {
        // Remove one thread T from s.queue;
        // Mark thread T as ready to run;
    }
    return 0;
}

// Fonction utilisée pour détruire un sémaphore après utilisation
int semaphore_destroy(Semaphore *sem) {
    free(sem->lock);
    return 0;
}

#endif