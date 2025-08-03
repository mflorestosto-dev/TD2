/**
 * Archivo: low_level_cpu_access.S
 * Función: funciones de bajo nivel de acceso al control del CPU
 * Autores: Ferreyro
 **/

.global _irq_enable
.global _irq_disable
.global _WRITE_8
.global _WRITE_16
.global _WRITE_32
.global _READ_8
.global _READ_16
.global _READ_32
.global _READ_CPSR_REGS

.section .kernel_text
/*
 Habilita interrupciones
 */
.align 4             // Alineado a 4 Bytes
_irq_enable:
    MRS R0, CPSR
    BIC R0, #0x80
    MSR CPSR, R0
    BX LR

/*
 Deshabilita interrupciones
 */
.align 4             // Alineado a 4 Bytes
_irq_disable:
    MRS R0, CPSR
    ORR R0, #0x40
    MSR CPSR, R0
    BX LR

.align 4             // Alineado a 4 Bytes
_READ_CPSR_REGS:
    DSB
    MRS R0, CPSR
    BX LR

/*
    Funciones de escritura y lecutra de 8, 16 y 32 bits
    Para las funciones _WRITE_X, desde C es: _WRITE_X(DIRECCION, VALOR) 
 */
.align 4             // Alineado a 4 Bytes
_WRITE_8:
    STRB R1, [R0]       // STR Byte el contenido de R0 en la posición R1
    BX LR

_WRITE_16:
    STRH R1, [R0]       // STR HalfWord el contenido de R0 en la posición R1
    BX LR

_WRITE_32:
    STR R1, [R0]       // STR Word el contenido de R0 en la posición R1
    BX LR

_READ_8:
    LDRB R0, [R0]
    BX LR

_READ_16:
    LDRH R0, [R0]
    BX LR

_READ_32:
    LDR R0, [R0]
    BX LR
