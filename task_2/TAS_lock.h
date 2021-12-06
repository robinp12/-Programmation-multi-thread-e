#ifndef TAS_LOCK_HEADER
#define TAS_LOCK_HEADER

#include <stdlib.h>

// Structure représentant notre implémentation d'un verrou à attente active
// utilisant l'algorithme Test and Set (TAS)
typedef struct {
    int state;
} LockTAS;

// Fonction utilisée pour initialiser un verrou TAS
int init_TAS(LockTAS *lock) {
    lock->state = 0;
    return 0;
}

// Fonction qui protège l'accès à la section critique d'un thread
void lock_TAS(LockTAS *lock) {
    asm("enter:\n"
        "movl $1, %%eax\n"      // %eax = 1
        "xchgl %%eax, %1\n"     // swap %eax and the second operand
        "testl %%eax, %%eax\n"  // set ZF to true if %eax contains 0
        "jnz enter\n"           // go back to 'enter:' if ZF is false
        : "=m"(lock->state)     // output operands
        : "m"(lock->state)      // input operands
        : "%eax");              // modified registers
}

// Fonction qui libère l'accès à la section critique d'un thread
void unlock_TAS(LockTAS *lock) {
    lock->state = 0;
}

#endif
