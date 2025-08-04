#include "../../inc/syscalls.h"

// Wrapper para imprimir un string
void syscall_print_string(const char *str) {
    __asm volatile (
        "mov r0, %[msg]\n"   // Argumento (puntero al string) en R0
        "swi %[id]\n"         // Instrucción SWI con el número de syscall
        :
        : [msg] "r" (str), [id] "i" (SYSCALL_PRINT_STRING)
        : "r0"
    );
}

// Wrapper para imprimir un entero
void syscall_print_uint(uint32_t n) {
    __asm volatile (
        "mov r0, %[num]\n"   // Argumento (número) en R0
        "swi %[id]\n"         // Instrucción SWI con el número de syscall
        :
        : [num] "r" (n), [id] "i" (SYSCALL_PRINT_UINT)
        : "r0"
    );
}