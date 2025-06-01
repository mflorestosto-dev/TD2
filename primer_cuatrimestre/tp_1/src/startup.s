/*
_table_isr:
Contiene los punteros a los handlers. Cada vector de excepción carga la dirección de su handler correspondiente.

copy_loop:
Copia la tabla de vectores a la posición 0 (donde el procesador espera que estén los vectores). 
Esto es típico en sistemas ARM antiguos donde la tabla debe estar en la dirección 0x00000000.

idle:
Un bucle infinito para “esperar” o “dormir”.
 */

.extern	_reset_vector
.extern undef_handler
.extern swi_handler
.extern prefetch_abort
.extern data_abort
.extern reserved_handler
.extern irq_handler
.extern fiq_handler
.global _start

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
literal_undef_handler 		: .word undef_handler
literal_swi_handler 		: .word swi_handler
literal_prefetch_abort 		: .word prefetch_abort
literal_data_abort 			: .word data_abort
literal_reserved_handler 	: .word reserved_handler
literal_irq_handler 		: .word irq_handler
literal_fiq_handler 		: .word fiq_handler


_start:

    // Modo FIQ
    MSR cpsr_c, (0x11 | 0xC0)  // Modo FIQ, deshabilitar IRQ/FIQ
    LDR SP, =stack_top_fiq

    // Modo IRQ
    MSR cpsr_c, (0x12 | 0xC0)  // Modo IRQ, deshabilitar IRQ/FIQ
    LDR SP, =stack_top_irq

    // Modo Supervisor
    MSR cpsr_c, (0x13 | 0xC0)  // Modo SVC, deshabilitar IRQ/FIQ
    LDR SP, =stack_top_svc

    // Modo Abort
    MSR cpsr_c, (0x17 | 0xC0)  // Modo Abort, deshabilitar IRQ/FIQ
    LDR SP, =stack_top_abt

    // Modo Undefined
    MSR cpsr_c, (0x1B| 0xC0)  // Modo Undefined, deshabilitar IRQ/FIQ
    LDR SP, =stack_top_und

    // Modo System (generalmente se usa como User con privilegios)
    MSR cpsr_c, (0x1F | 0xC0)  // Modo System, deshabilitar IRQ/FIQ
    LDR SP, =stack_top_sys

    LDR r0, = #0             //Direccion destino
    LDR r1, = _table_isr        // Dirección destino
    LDR r2, = _start         // Dirección final

copy_loop:
    LDR r3, [r1], #4
    STR r3, [r0], #4
	CMP r1, r2
    BNE copy_loop

    // --------------------------------------------------------
    // Llamar a la inicialización del sistema en C (board_init)
    // --------------------------------------------------------
    BL board_init               // Llamada a la rutina en C

    // --------------------------------------------------------
    // Habilitar interrupciones (IRQ) globalmente
    // --------------------------------------------------------
    MRS r0, cpsr
    BIC r0, r0, #0x80           // Limpiamos bit I (habilitamos IRQ)
    MSR cpsr_c, r0

idle:
    WFI
	B idle
