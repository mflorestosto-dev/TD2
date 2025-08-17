/*
_table_isr:
Contiene los punteros a los handlers. Cada vector de excepción carga la dirección de su handler correspondiente.

copy_loop:
Copia la tabla de vectores a la posición 0 (donde el procesador espera que estén los vectores). 

idle:
Un bucle infinito para “esperar” o “dormir”.
 */

.global _start

//Handlers
.extern	_reset_vector
.extern handler_UND
.extern handler_SWI
.extern handler_prefetch_abort
.extern handler_data_abort
.extern handler_reserved
.extern handler_IRQ
.extern handler_FIQ

//Stack pointers de los modos
.extern __stack_top_sys__ 
.extern __stack_top_svc__ 
.extern __stack_top_irq__
.extern __stack_top_fiq__
.extern __stack_top_abt__
.extern __stack_top_und__

.extern board_init

.code 32

.section .start:

_table_isr:
	LDR PC, literal_reset_vector
    LDR PC, literal_undef_handler      // Undefined instruction
    LDR PC, literal_swi_handler        // Software interrupt
    LDR PC, literal_prefetch_abort     // Prefetch abort
    LDR PC, literal_data_abort         // Data abort
    LDR PC, literal_reserved_handler   // Reserved
    LDR PC, literal_irq_handler        // IRQ
    LDR PC, literal_fiq_handler        // FIQ


    literal_reset_vector 		: .word _reset_vector
    literal_undef_handler 		: .word handler_UND
    literal_swi_handler 		: .word handler_SWI
    literal_prefetch_abort 		: .word handler_prefetch_abort
    literal_data_abort 			: .word handler_data_abort
    literal_reserved_handler 	: .word handler_reserved
    literal_irq_handler 		: .word handler_IRQ
    literal_fiq_handler 		: .word handler_FIQ


_start:
    LDR r0, = #0                    // Dirección destino (inicio de vectores)
    LDR r1, = _table_isr            // Origen
    LDR r2, = _start   

_copy_loop:
    LDR r3, [r1], #4
    STR r3, [r0], #4
	CMP r1, r2
    BNE _copy_loop            // Fin de la tabla

    //Constantes para los modos de operacion
    .equ USR_MODE,  0x10 /*User         - CPSR[4:0] = 10000*/
    .equ FIQ_MODE,  0x11 /*FIQ          - CPSR[4:0] = 10001*/
    .equ IRQ_MODE,  0x12 /*IRQ          - CPSR[4:0] = 10010*/
    .equ SVC_MODE,  0x13 /*Supervisor   - CPSR[4:0] = 10011*/
    .equ ABT_MODE,  0x17 /*Abort        - CPSR[4:0] = 10111*/
    .equ UND_MODE,  0x1B /*Undefined    - CPSR[4:0] = 11011*/
    .equ SYS_MODE,  0x1F /*System       - CPSR[4:0] = 11111*/

    .equ MASK,      0xC0 /*Mascara para poner en 1 el bit de la interrupcion a deshabilitar */

_stack_init:
    // Modo FIQ
    //MSR cpsr_c, #(FIQ_MODE | MASK)   // Modo FIQ, deshabilitar IRQ/FIQ
    //LDR SP, = __stack_top_fiq__

    // Modo IRQ
    MSR cpsr_c, #(IRQ_MODE | MASK)  // Modo IRQ, deshabilitar IRQ/FIQ
    LDR SP, = __stack_top_irq__

    // Modo Supervisor
    MSR cpsr_c, #(SVC_MODE | MASK)  // Modo SVC, deshabilitar IRQ/FIQ
    LDR SP, = __stack_top_svc__

    // Modo Abort
    MSR cpsr_c, #(ABT_MODE | MASK)   // Modo Abort, deshabilitar IRQ/FIQ
    LDR SP, = __stack_top_abt__

    // Modo Undefined
    MSR cpsr_c, #(UND_MODE | MASK)   // Modo Undefined, deshabilitar IRQ/FIQ
    LDR SP, = __stack_top_und__

    // Modo System (generalmente se usa como User con privilegios)
    MSR cpsr_c, #(SYS_MODE | MASK)   // Modo System, deshabilitar IRQ/FIQ
    LDR SP, = __stack_top_sys__


// --------------------------------------------------------
// Llamar a la inicialización del sistema en C (board_init)
// --------------------------------------------------------
_board_init:
    BL board_init       // Llamada a la rutina en C

// --------------------------------------------------------
// Habilitar interrupciones (IRQ) globalmente
// --------------------------------------------------------
_irq_enable:
    CPSIE i

_idle:

    // Bucle infinito para "esperar" o "dormir"
    LDR R10,=idle
    MOV LR, PC
    BX R10
    B idle

.end
