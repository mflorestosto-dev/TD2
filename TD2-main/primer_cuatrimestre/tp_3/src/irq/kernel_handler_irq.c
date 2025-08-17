#include <stdint.h>
#include "../../inc/gic.h"
#include "../../inc/timer.h"
#include "../../inc/scheduler.h"

#define GIC0    ((_gicc_t*)GICC0_ADDR)
#define TIMER0  ((_timer_t*)TIMER0_ADDR)
#define CONTEXT_OFFSET_RO_TO_SP 0

extern uint32_t *tarea_sp;

uint32_t* kernel_handler_irq(uint32_t* sp);

__attribute__((section(".text")))
uint32_t* kernel_handler_irq(uint32_t* sp)
{
    uint32_t *nuevo_sp = 0; // Valor por defecto para depuración
    uint32_t id_interrupcion = GIC0->IAR & 0x1FF; // ID de la interrupción activa

    // Guardamos el stack pointer real (usado luego en handlers.s para restaurar contexto)
    tarea_sp = sp - CONTEXT_OFFSET_RO_TO_SP;  // SP apuntando a posición real de contexto guardado

    switch (id_interrupcion)
    {
        case GIC_SOURCE_TIMER0:
            if (TIMER0->Timer1MIS & 0x1) {
                TIMER0->Timer1IntClr = 0x1;  // Limpiar interrupción del timer
                nuevo_sp = __scheduler();     // Obtener el nuevo stack pointer si hay cambio de tarea
            }
            break;

        default:
            break;
    }

    GIC0->EOIR = id_interrupcion;  // Notificar al GIC fin de interrupción

    return nuevo_sp;  // El nuevo SP será usado por handlers.s para restaurar contexto
}
