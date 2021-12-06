#ifndef TAS_LOCK_HEADER
#define TAS_LOCK_HEADER

#include <stdlib.h>

typedef struct {
    int state;
} LockTAS;

int init_TAS(LockTAS *lock) {
    lock->state = 0;
    return 0;
}

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

void unlock_TAS(LockTAS *lock) {
    asm("movl $0, %%eax\n"   // %eax = 0
        "xchgl %%eax, %0\n"  // swap %eax and the first operand
        : "=m"(lock->state)  // output operands
        : "m"(lock->state)   // input operands
        : "%eax");           // modified registers
}

#endif
