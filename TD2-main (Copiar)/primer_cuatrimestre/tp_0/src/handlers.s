/*
Este archivo define los manejadores de excepciones (handlers). 
Cada sección (b .) significa que cuando ocurre esa excepción, el procesador 
se queda en un bucle infinito (esperando que el usuario lo depure).
 */


.global undef_handler
.global swi_handler
.global prefetch_abort
.global data_abort
.global reserved_handler
.global irq_handler
.global fiq_handler

.section .handlers, "ax"

undef_handler:       
    b . 

swi_handler:         
    b .

prefetch_abort:      
    b .

data_abort:          
    b .

reserved_handler:    
    b .

irq_handler:         
    @MOV R0, #0x04
    b .

fiq_handler:         
    b .