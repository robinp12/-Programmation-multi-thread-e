#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

#include "TAS_lock.h"

typedef struct {
    int state;
    LockTAS *lock;
} Semaphore;

int semaphore_init(Semaphore *sem, int initial_state) {
    sem = malloc(sizeof(Semaphore));
    if (sem == NULL) {
        return -1;
    }

    sem->state = initial_state;
    init_TAS(sem->lock);
    return 0;
}

int semaphore_wait(Semaphore *sem) {
    lock_TAS(sem->lock);
    --sem->state;
    unlock_TAS(sem->lock);

    if (sem->state < 0) {
        // Place this thread in s.queue;
        // This thread is blocked;
    }
    return 0;
}

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

int semaphore_destroy(Semaphore *sem) {
    free(sem->lock);
    return 0;
}

#endif
