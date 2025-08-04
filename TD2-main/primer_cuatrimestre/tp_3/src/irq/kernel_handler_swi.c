#include <stdint.h>
#include "../../inc/syscalls.h"  // Para tener los números de syscall
#include "../../inc/console_utils.h" // Para usar Printf

#define SYSCALL_PRINT_STRING 0 // Syscall para imprimir un string
#define SYSCALL_PRINT_UINT   1 // Syscall para imprimir un entero sin signo


// El manejador principal de la excepción SWI/SVC
void kernel_handler_svc(uint32_t* sp) {
    
    // 1. Extraemos el número de la syscall de la instrucción SWI
    uint32_t* pc = (uint32_t*) sp[13]; // PC de la tarea guardado en el stack
    uint32_t swi_instruction = *(pc - 1);
    uint32_t svc_number = swi_instruction & 0x00FFFFFF;

    // 2. Ejecutamos la acción llamando a Printf, que es parte del Kernel
    switch (svc_number) {
        case SYSCALL_PRINT_STRING:
        {
            // El argumento (puntero al string) está en el R0 guardado
            const char* message = (const char*) sp[0];
            
            // Usamos la función del kernel para imprimir el string
            Printf("%s", message);
            break;
        }

        case SYSCALL_PRINT_UINT:
        {
            // El argumento (número) está en el R0 guardado
            uint32_t number = sp[0];

            // Usamos la función del kernel para imprimir el número
            // Puedes cambiar "%u" por "%d" (decimal) o "%x" (hexadecimal)
            Printf("%u", number);
            break;
        }

        default:
            // Syscall desconocida, imprimimos un error
            Printf("[Kernel] Error: Syscall desconocida numero %d\n", svc_number);
            break;
    }
}