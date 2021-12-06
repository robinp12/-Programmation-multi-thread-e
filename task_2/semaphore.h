#include "TAS_lock.h"

// Structure représentant notre implémentation d'un sémaphore
typedef struct Semaphore {
    int state;
    LockTAS lock;
} Semaphore;

// Fonction utilisée pour initialiser un sémaphore
int semaphore_init(Semaphore *sem, int initial_state) {
    sem->state = initial_state;
    init_TAS(&sem->lock);
    return 0;
}

// Fonction qui bloque les threads tant que la ressource protégée
// par le sémaphore est utilisée, et décrémente le sémaphore
int semaphore_wait(Semaphore *sem) {
    while (sem->state <= 0) {
        lock_TAS(&sem->lock);
        sem->state--;
        unlock_TAS(&sem->lock);
    }
    return 0;
}

// Fonction qui rend un des threads bloqués actif et incrémente donc le sémaphore
int semaphore_post(Semaphore *sem) {
    lock_TAS(&sem->lock);
    sem->state++; //Incrémente la valeur du sémaphore
    unlock_TAS(&sem->lock);
    return 0;
}

// Fonction utilisée pour détruire un sémaphore après utilisation
int semaphore_destroy(Semaphore *sem) {
    free(sem);
    return 0;
}