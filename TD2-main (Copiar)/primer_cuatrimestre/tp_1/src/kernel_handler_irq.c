#include <stdint.h>
#include "../inc/gic.h"
#include "../inc/timer.h"

void kernel_handler_irq(uint32_t *sp);

//Definicion de punteros a estructuras  de controlador de excepciones y timer 
#define GIC0 ((_gicc_t*)GICC0_ADDR)
#define TIMER0 ((_timer_t*)TIMER0_ADDR)

__attribute__((section(".handler"))) 
void kernel_handler_irq(uint32_t *sp)
{
    // Leer ID de la interrupción (solo los bits 8:0)
    uint32_t id_interrupcion = GIC0->IAR & 0x1FF;

    switch(id_interrupcion)
    {
        case GIC_SOURCE_TIMER0:  
            if(TIMER0->Timer1RIS & 0x1){ //si el bit 0 esta en 1, el timer genero una interrupcion pendiente
                TIMER0->Timer1IntClr = 0x7; // Escribo cualquier cosa en el registro con eso ya resetea
            }  
            break;

        default:
            // Ignorar o loguear interrupciones no manejadas
            break;
    }

    // Señalar fin de interrupción al GIC
    GIC0->EOIR |= id_interrupcion;
}