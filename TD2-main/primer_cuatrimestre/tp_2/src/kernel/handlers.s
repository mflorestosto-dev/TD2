/*
Este archivo define los manejadores de excepciones (handlers). 
Cada sección (b .) significa que cuando ocurre esa excepción, el procesador 
se queda en un bucle infinito (esperando que el usuario lo depure).
 */

.extern kernel_handler_irq

.global handler_UND
.global handler_SWI
.global handler_prefetch_abort
.global handler_data_abort
.global handler_reserved
.global handler_IRQ
.global handler_FIQ

.section .handlers, "ax"

handler_UND:       
    b .

handler_SWI:         
    b .

handler_prefetch_abort:      
    b .

handler_data_abort:          
    b .

handler_reserved:    
    b .

handler_IRQ:         

    //Entra la excepcion, se guardan Link register,los registros del momento en donde se esta, el SP y el SPSR
    SUB LR,LR,#4
    PUSH {R0-R12,LR}
    MOV R7,SP
    MRS R8,SPSR
    PUSH {R7,R8}

    // Manejo de excepcion
    MOV R0,SP
    BL kernel_handler_irq

    //Se vuelve todo como estaba antes, se recontruye la pila
    MOV SP,R0
    POP {R7,R8}
    MSR SPSR,R8
    MOV SP,R7
    LDMFD SP!,{R0-R12, PC}^

handler_FIQ:         
    b .
