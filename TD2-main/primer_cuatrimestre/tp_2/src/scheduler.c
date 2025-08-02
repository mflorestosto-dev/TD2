#include "stdint.h"
#include "../inc/scheduler.h"
#include "../inc/tareas.h"


#define CANT_TAREAS 4

extern uint32_t __task_idle_stack_top_irq__, 
                __task_tarea1_stack_top_irq__,
                __task_tarea2_stack_top_irq__, 
                __task_tarea3_stack_top_irq__;

extern uint32_t __task_idle_stack_top_svc__,
                __task_tarea1_stack_top_svc__,
                __task_tarea2_stack_top_svc__,
                __task_tarea3_stack_top_svc__;

__attribute__((section(".data"))) uint32_t ticks = 0;
__attribute__((section(".data"))) uint32_t tarea_nueva = 0;
__attribute__((section(".data"))) uint32_t inicio = 1;
__attribute__((section(".data"))) uint32_t *true_sp = 0;

__attribute__((section(".data"))) TCB tcb_tareas[CANT_TAREAS];

__attribute__((section(".data"))) 
uint32_t task_ticks[CANT_TAREAS] = {5, 8, 12, 5};

__attribute__((section(".data"))) 
void(*tareas_fun[CANT_TAREAS])(void) = {tarea_idle, 
                                        tarea_1, 
                                        tarea_2, 
                                        tarea_3};

__attribute__((section(".data"))) 
uint32_t *task_stack_IRQ[CANT_TAREAS] = {&__task_idle_stack_top_irq__, 
                                         &__task_tarea1_stack_top_irq__, 
                                         &__task_tarea2_stack_top_irq__, 
                                         &__task_tarea3_stack_top_irq__};

__attribute__((section(".data"))) 
uint32_t *task_stack_SVC[CANT_TAREAS] = {&__task_idle_stack_top_svc__, 
                                         &__task_tarea1_stack_top_svc__, 
                                         &__task_tarea2_stack_top_svc__, 
                                         &__task_tarea3_stack_top_svc__};



void preparar_contexto_tarea(TCB *tcb, void (*entry_point)(void)) {
    int i;
    uint32_t *sp = tcb->sp_irq;

    // LR = PC destino (entry point)
    *(--sp) = (uint32_t) entry_point;

    // R12..R0 = 0
    for (i = 0; i < 13; i++) {
        sp--;
        *(--sp) = 0;
    }

    // SPSR (modo SVC, IRQ habilitadas)
    *(--sp) = 0x113;

    // R0 apunta al final del contexto (se simula como SP_user)
    *(--sp) = (uint32_t) sp;

    // Guardar el SP final
    tcb->sp_irq = sp;
}

__attribute__((section(".kernel_text"))) 
void __scheduler_init(void)
{
    for (uint32_t i = 0; i < CANT_TAREAS; i++) {
        tcb_tareas[i].id = i;
        tcb_tareas[i].ticks = task_ticks[i];
        tcb_tareas[i].sp_irq = task_stack_IRQ[i];
        tcb_tareas[i].sp_svc = task_stack_SVC[i];

        // Preparar contexto de la tarea
        preparar_contexto_tarea(&tcb_tareas[i], tareas_fun[i]);
    }
}

// Prototipos
__attribute__((section(".kernel_text")))
uint32_t *__scheduler(void) {
    ticks++;  // incremento del sistema de ticks (timer IRQ por ejemplo)

    uint32_t tarea_actual = tarea_nueva; // tarea que estaba corriendo

    if (ticks >= tcb_tareas[tarea_actual].ticks) {
        ticks = 0; // reinicio el tick

        // Selección round-robin: paso a la próxima tarea (0 a 3)
        tarea_nueva = tcb_tareas[(tarea_nueva + 1) % 4].id;

        // Guardo el SP_IRQ de la tarea actual (SP actual en modo IRQ)
        // true_sp fue guardado por el manejador IRQ
        tcb_tareas[tarea_actual].sp_irq = true_sp;

        // Cambio a modo SVC para guardar el SP_SVC de la tarea actual
        asm volatile("CPS #0x13"); // Modo SVC (supervisor)
        asm volatile("MOV %0, SP" : "=r"(tcb_tareas[tarea_actual].sp_svc)); // Guardo SP_SVC de la tarea actual

        // Cambio el SP al de la nueva tarea (modo SVC)
        asm volatile("MOV SP, %0" :: "r"(tcb_tareas[tarea_nueva].sp_svc)); // Cargo SP_SVC de la nueva tarea

        // Vuelvo al modo IRQ
        asm volatile("CPS #0x12");
        // No toco SP_IRQ porque va a ser restaurado por el código en asm que hace el RETI
    }

    // Primera vez que cambio de tarea: debo configurar el SP de la tarea inicial
    if (inicio) {
        inicio = 0;

        // Cambio a modo SVC y pongo el SP_SVC de la primera tarea
        asm volatile("CPS #0x13");
        asm volatile("MOV SP, %0" :: "r"(tcb_tareas[tarea_actual].sp_svc));

        // Vuelvo al modo IRQ
        asm volatile("CPS #0x12");
    }

    return tcb_tareas[tarea_nueva].sp_irq;  // Retorno el SP_IRQ de la nueva tarea para que se use en handlers.s
}

