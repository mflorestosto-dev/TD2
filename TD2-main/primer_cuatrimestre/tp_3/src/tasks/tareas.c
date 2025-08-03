#include "stdint.h"
#include "../../inc/tareas.h"


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
    // Definimos hasta qué número de la secuencia queremos calcular.
    const int MAX_FIBO_COUNT = 20; 
    uint32_t a = 0, b = 1;

    // Primeros dos números de la secuencia
    // Aquí se haría la llamada a sistema (syscall) para imprimir 'a' (0)
    // Aquí se haría la llamada a sistema (syscall) para imprimir 'b' (1)

    for (int i = 2; i < MAX_FIBO_COUNT; i++) {
        uint32_t next = a + b;
        // Aquí se haría la llamada a sistema (syscall) para imprimir el valor de 'next'
        a = b;
        b = next;
    }

    // La tarea ya completó su cálculo principal.
    // Entra en un bucle infinito para no finalizar y ceder el control.
    while(1){
        // Podríamos llamar a WFI para dormir la tarea si no hay más que hacer.
        asm("WFI");
    }
}

/**
 * @brief Tarea 2: Conjetura de Collatz.
 *
 * Aplica la secuencia de Collatz a un número inicial hasta llegar a 1.
 * Si n es par -> n = n / 2
 * Si n es impar -> n = 3 * n + 1
 * Cada paso de la secuencia se imprimiría por una syscall.
 */
__attribute__((section(".tarea_2_text"))) void tarea_2(void){
    // Número inicial para la secuencia (puedes cambiarlo).
    uint32_t n = 27; 

    // Imprimir el número inicial
    // Aquí se haría la llamada a sistema (syscall) para imprimir el valor inicial de 'n'.

    while (n > 1) {
        if (n % 2 == 0) {
            // Es par
            n = n / 2;
        } else {
            // Es impar
            n = 3 * n + 1;
        }
        // Aquí se haría la llamada a sistema (syscall) para imprimir el nuevo valor de 'n'.
    }

    // La secuencia ha llegado a 1. La tarea entra en modo de espera.
    while(1){
        asm("WFI");
    }
}

/**
 * @brief Tarea 3: Factorización en números primos.
 *
 * Calcula los factores primos de un número fijo.
 * Cada factor primo encontrado se imprimiría mediante una syscall.
 */
__attribute__((section(".tarea_3_text"))) void tarea_3(void){
    // Número que queremos factorizar (puedes cambiarlo).
    uint32_t numero = 9975; // Por ejemplo: 3 * 3 * 5 * 5 * 5 * 7
    uint32_t n = numero;
    uint32_t divisor = 2;

    // Aquí se haría una syscall para imprimir un mensaje inicial, ej: "Factores de 9975:"

    while (n > 1) {
        if (n % divisor == 0) {
            // 'divisor' es un factor primo.
            // Aquí se haría la llamada a sistema (syscall) para imprimir el 'divisor'.
            n = n / divisor;
        } else {
            // Pasamos al siguiente posible divisor.
            divisor++;
        }
    }

    // La factorización ha terminado. La tarea entra en modo de espera.
    while(1){
        asm("WFI");
    }
}