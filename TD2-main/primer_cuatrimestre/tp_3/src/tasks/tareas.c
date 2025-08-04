#include "stdint.h"
#include "../../inc/tareas.h"
#include "../../inc/syscalls.h" // Para usar las syscalls


__attribute__((section(".tarea_idle_text"))) void tarea_idle(void){
    while(1){
        // La tarea IDLE pone al procesador en modo de bajo consumo hasta que ocurra una interrupción.
        asm("WFI"); // Wait For Interrupt
    }
}

/**
 * @brief Tarea 1: Sucesión de Fibonacci.
 *
 * Calcula los números de la sucesión de Fibonacci hasta un límite definido (MAX_FIBO).
 * En un sistema real, cada número calculado se imprimiría a través de una llamada a sistema (syscall).
 */
__attribute__((section(".tarea_1_text"))) void tarea_1(void){
    uint32_t a = 0, b = 1;
    
    syscall_print_string("Tarea 1: Fibonacci\n");

    for (int i = 0; i < 10; i++) {
        syscall_print_uint(a);
        syscall_print_string(" ");
        
        uint32_t next = a + b;
        a = b;
        b = next;
    }
    
    // La tarea termina su trabajo y se duerme
    while(1){
        asm("WFI");
    }
}

/**
 * @brief Tarea 2: Conjetura de Collatz.
 *
 * Imprime la secuencia de Collatz para un número inicial usando syscalls.
 * Si n es par -> n = n / 2
 * Si n es impar -> n = 3 * n + 1
 */
__attribute__((section(".tarea_2_text"))) void tarea_2(void){
    // Número inicial para la secuencia (puedes cambiarlo).
    uint32_t n = 27; 

    // Imprimimos un título para la tarea.
    syscall_print_string("Tarea 2: Conjetura de Collatz para n=27\n");

    // Imprimimos el número inicial.
    syscall_print_uint(n);

    while (n > 1) {
        if (n % 2 == 0) {
            // Es par
            n = n / 2;
        } else {
            // Es impar
            n = 3 * n + 1;
        }
        // Imprimimos una flecha y el nuevo número de la secuencia.
        syscall_print_string(" -> ");
        syscall_print_uint(n);
    }
    
    // Imprimimos un salto de línea al final.
    syscall_print_string("\n");

    // La secuencia ha llegado a 1. La tarea entra en modo de espera.
    while(1){
        asm("WFI");
    }
}

/*
 * @brief Tarea 3: Factorización en números primos.
 *
 * Calcula e imprime los factores primos de un número fijo usando syscalls.
 */
__attribute__((section(".tarea_3_text"))) void tarea_3(void){
    // Número que queremos factorizar (puedes cambiarlo).
    uint32_t numero = 9975;
    uint32_t n = numero;
    uint32_t divisor = 2;

    // Imprimimos un título para la tarea.
    syscall_print_string("Tarea 3: Factores primos de ");
    syscall_print_uint(numero);
    syscall_print_string(":\n");

    while (n > 1) {
        if (n % divisor == 0) {
            // 'divisor' es un factor primo. Lo imprimimos.
            syscall_print_uint(divisor);
            syscall_print_string(" ");

            // Dividimos el número por el factor encontrado.
            n = n / divisor;
        } else {
            // Pasamos al siguiente posible divisor.
            divisor++;
        }
    }

    // Imprimimos un salto de línea al final.
    syscall_print_string("\n");

    // La factorización ha terminado. La tarea entra en modo de espera.
    while(1){
        asm("WFI");
    }
}