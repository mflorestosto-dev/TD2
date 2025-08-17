/*
Este archivo define los manejadores de excepciones (handlers). 
Cada sección (b .) significa que cuando ocurre esa excepción, el procesador 
se queda en un bucle infinito (esperando que el usuario lo depure).
 */


.global	_reset_vector
.global handler_UND
.global handler_SWI
.global handler_prefetch_abort
.global handler_data_abort
.global handler_reserved
.global handler_IRQ
.global handler_FIQ


.extern kernel_handler_irq
.extern kernel_handler_svc

.section .handlers, "ax"

handler_UND:       
    b .

handler_SWI:         
    // 1. Guardamos el contexto de la tarea que llamó a la syscall
    //    (registros R0-R12, LR) en la pila.
    //    Esto es necesario para que podamos restaurar el contexto después de manejar la syscall.
    //    El LR se ajusta para que no vuelva a la instrucción de llamada a la syscall,
    //    sino que vuelva al punto correcto después de manejar la syscall.
    SUB LR, LR, #4
    PUSH {R0-R12, LR}

    // 2. Llamamos al manejador en C, pasándole un puntero al contexto
    //    guardado (el stack pointer actual)
    MOV R0, SP
    BL kernel_handler_svc

    // 3. Restauramos el contexto y volvemos a la tarea
    LDMFD sp!, {R0-R12, PC}^

handler_prefetch_abort:      
    b .

handler_data_abort:          
    b .

handler_reserved:    
    b .

handler_IRQ:         

    //Entra la excepcion, se guardan Link register,los registros del momento en donde se esta, el SP y el SPSR
    SUB LR, LR, #4
    PUSH {R0-R12, LR}
    MOV R7, SP
    MRS R8, SPSR
    PUSH {R7, R8}

    // Manejo de excepcion
    MOV R0, SP
    BL kernel_handler_irq

    //Se vuelve todo como estaba antes, se recontruye la pila
    MOV SP,R0
    POP {R7,R8}
    MSR SPSR, R8
    MOV SP, R7
    LDMFD sp!, {R0-R12, PC}^

handler_FIQ:         
    b .

.end