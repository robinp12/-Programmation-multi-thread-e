#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

typedef struct {
    long int val;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
} prim_sem_t;

int prim_sem_init(prim_sem_t *sem, int pshared, unsigned int value){
    sem = malloc(sizeof(prim_sem_t));
    if (sem == NULL) {
        return -1;
    }
    sem->val = value;
    return 0;
};
int prim_sem_destroy(prim_sem_t *sem){
    free(sem);
    sem=NULL;
    return 0;
};
int prim_sem_wait(prim_sem_t *sem){
    --sem->val;
    if(sem->val<0)
    {
        // Place this thread in s.queue;
        // This thread is blocked;
    }
    return 0;
};
int prim_sem_post(prim_sem_t *sem){
    ++sem->val;
    if(sem->val<=0)
    {
        // Remove one thread T from s.queue;
        // Mark thread T as ready to run;
    }
    return 0;
};

#endif
