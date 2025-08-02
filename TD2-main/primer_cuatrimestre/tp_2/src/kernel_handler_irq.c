#include <stdint.h>
#include "../inc/gic.h"
#include "../inc/timer.h"
#include "../inc/scheduler.h"

#define GIC0    ((_gicc_t*)GICC0_ADDR)
#define TIMER0  ((_timer_t*)TIMER0_ADDR)

extern uint32_t* true_sp;

uint32_t* kernel_handler_irq(context_t* contexto);

__attribute__((section(".handler")))
uint32_t* kernel_handler_irq(context_t* contexto)
{
    uint32_t *nuevo_SP = (uint32_t*)0xdeadbeef;  // Valor por defecto para depuración
    uint32_t id_interrupcion = GIC0->IAR & 0x1FF; // ID de la interrupción activa

    // Guardamos el stack pointer real (usado luego en handlers.s para restaurar contexto)
    true_sp = (uint32_t*)(contexto->sp - 8);  // SP apuntando a posición real de contexto guardado

    switch (id_interrupcion)
    {
        case GIC_SOURCE_TIMER0:
            if (TIMER0->Timer1RIS & 0x1) {
                TIMER0->Timer1IntClr = 0x7;  // Limpiar interrupción del timer
                nuevo_SP = __scheduler();     // Obtener el nuevo stack pointer si hay cambio de tarea
            }
            break;

        default:
            // Aquí podrías loguear interrupciones no manejadas o ignorarlas
            break;
    }

    GIC0->EOIR |= id_interrupcion;  // Notificar al GIC fin de interrupción

    return nuevo_SP;  // El nuevo SP será usado por handlers.s para restaurar contexto
}
