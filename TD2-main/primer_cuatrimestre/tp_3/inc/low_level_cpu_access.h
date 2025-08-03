/**
 * Archivo: low_level_cpu_access.h
 * Función: contiene las definiciones de los prototipos en C para poder importar los sys utils
 * Autores: Peccia - Ferreyro
 * Referencia: los códigos desarrollados fueron tomados a partir de diferentes referencias y en base al TRM. El objetivo de esta recopilación
 *             fue darle un enfoque didáctico para el alumno, con comentarios pertinentes y referencias a la documentación disponible.
 *             Ref. 1: https://github.com/allexoll/BBB-BareMetal
 *             Ref. 2: https://github.com/auselen/down-to-the-bone
 *             Ref. 3: https://github.com/mvduin/bbb-asm-demo
 *             Ref. 4: Starterware de Texas Instruments
 *             Ref. 5: AM335x ARM Cortex-A8 - Technical Reference Manual 
 *             Ref. 6: ARM Architectural Reference Manual ARMv7-A and ARMv7-R Edition (ARM)
 *             Ref. 7: Instruction Set Assembly Guide for ARMv7 and earlier ARM Architectures (Version 2.0) - Ref. Guide (ISAG)
 **/

#ifndef __LOW_LEVE_ACCESS_H
#define __LOW_LEVE_ACCESS_H

#include <stddef.h>
#include <stdint.h>

extern void _irq_enable(void);
extern void _irq_disable(void);
extern void _WRITE_8(uint32_t , uint8_t );
extern void _WRITE_16(uint32_t , uint16_t );
extern void _WRITE_32(uint32_t , uint32_t );
extern uint8_t _READ_8(uint32_t );
extern uint16_t _READ_16(uint32_t );
extern uint32_t _READ_32(uint32_t );
extern uint32_t _READ_CPSR_REGS(void);

#define HALT_CPU_m  asm("WFI");

#endif /* defined(__LOW_LEVE_ACCESS_H) */