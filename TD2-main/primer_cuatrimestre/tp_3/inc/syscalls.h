#ifndef SYSCALLS_H_
#define SYSCALLS_H_

#include <stdint.h>

// --- Números de Syscall ---
#define SYSCALL_PRINT_STRING 0 // Syscall para imprimir un string
#define SYSCALL_PRINT_UINT   1 // Syscall para imprimir un entero sin signo

// --- Funciones Wrapper (las que usarán las tareas) ---
void syscall_print_string(const char *str);
void syscall_print_uint(uint32_t n);

#endif /* SYSCALLS_H_ */