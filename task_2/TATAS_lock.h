#ifndef TATAS_LOCK_HEADER
#define TATAS_LOCK_HEADER

#include <stdlib.h>

typedef struct {
    int state;
} LockTATAS;

int init_TATAS(LockTATAS *lock) {
    if (lock == NULL) {
        return -1;
    }

    lock->state = 0;
    return 0;
}

void lock_TATAS(LockTATAS *lock) {
    asm("enter:\n"
        "loop:\n"
        "cmpl $1, %1\n"
        "je enter\n"
        "movl $1, %%eax\n"      // %eax = 1
        "xchgl %%eax, %1\n"     // swap %eax and the second operand
        "testl %%eax, %%eax\n"  // set ZF to true if %eax contains 0
        "jnz enter\n"           // go back to 'enter:' if ZF is false
        : "=m"(lock->state)     // output operands
        : "m"(lock->state)      // input operands
        : "%eax");              // modified registers
}

void unlock_TATAS(LockTATAS *lock) {
    asm("movl $0, %%eax\n"   // %eax = 0
        "xchgl %%eax, %0\n"  // swap %eax and the first operand
        : "=m"(lock->state)  // output operands
        : "m"(lock->state)   // input operands
        : "%eax");           // modified registers
}

#endif
