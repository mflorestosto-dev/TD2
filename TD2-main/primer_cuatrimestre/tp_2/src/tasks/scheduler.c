#include "stdint.h"
#include "../../inc/scheduler.h"
#include "../../inc/tareas.h"

extern uint32_t __task_idle_stack_top_irq__, 
                __task_tarea1_stack_top_irq__,
                __task_tarea2_stack_top_irq__, 
                __task_tarea3_stack_top_irq__;

extern uint32_t __task_idle_stack_top_svc__,
                __task_tarea1_stack_top_svc__,
                __task_tarea2_stack_top_svc__,
                __task_tarea3_stack_top_svc__;

extern uint32_t __task_idle_stack_top_sys__,
                __task_tarea1_stack_top_sys__,
                __task_tarea2_stack_top_sys__,
                __task_tarea3_stack_top_sys__;

uint32_t ticks = 0;
uint32_t inicio = TAREA_INICIAL;
uint32_t *tarea_sp = 0;

__attribute__((section(".data"))) 
TCB tcb_tareas[CANT_TAREAS];

uint32_t task_ticks[CANT_TAREAS] = {TICKS_IDLE_TASK, 
                                    TICKS_TAREA_1, 
                                    TICKS_TAREA_2, 
                                    TICKS_TAREA_3};

void(*tareas_fun[CANT_TAREAS])(void) = {tarea_idle, 
                                        tarea_1, 
                                        tarea_2, 
                                        tarea_3};

uint32_t *task_stack_IRQ[CANT_TAREAS] = {&__task_idle_stack_top_irq__, 
                                         &__task_tarea1_stack_top_irq__, 
                                         &__task_tarea2_stack_top_irq__, 
                                         &__task_tarea3_stack_top_irq__};

uint32_t *task_stack_SVC[CANT_TAREAS] = {&__task_idle_stack_top_svc__, 
                                         &__task_tarea1_stack_top_svc__, 
                                         &__task_tarea2_stack_top_svc__, 
                                         &__task_tarea3_stack_top_svc__};

uint32_t *task_stack_SYS[CANT_TAREAS] = {&__task_idle_stack_top_sys__, 
                                         &__task_tarea1_stack_top_sys__, 
                                         &__task_tarea2_stack_top_sys__, 
                                         &__task_tarea3_stack_top_sys__};


void init_irq_stack(void) {
    for (uint32_t i = 0; i < CANT_TAREAS; i++) {
        tcb_tareas[i].sp_irq = task_stack_IRQ[i];

        // Pusheo LR (funcion de la tarea)
        *(--tcb_tareas[i].sp_irq) = (uint32_t) tareas_fun[i];

        // Pusheo R12..R0
        for(uint32_t j = 0; j < 13; j++) {
            *(--tcb_tareas[i].sp_irq) = j;
        }   

        // Pusheo R0 y SPSR
        uint32_t r0_actual = (uint32_t) tcb_tareas[i].sp_irq;
        *(--tcb_tareas[i].sp_irq) = 0x113; // SPSR para que la tarea corra en modo SVC
        *(--tcb_tareas[i].sp_irq) = r0_actual;
    }
}

void init_svc_stack(void) {
    for (uint32_t i = 0; i < CANT_TAREAS; i++) {
        tcb_tareas[i].sp_svc = task_stack_SVC[i];

        // Pusheo LR (funcion de la tarea)
        *(--tcb_tareas[i].sp_svc) = (uint32_t) tareas_fun[i];

        // Pusheo R12..R0
        for(uint32_t j = 0; j < 13; j++) {
            *(--tcb_tareas[i].sp_svc) = j;
        }   

        // Pusheo R0 y SPSR
        uint32_t r0_actual = (uint32_t) tcb_tareas[i].sp_svc;
        *(--tcb_tareas[i].sp_svc) = 0x113; // SPSR para que la tarea corra en modo SVC
        *(--tcb_tareas[i].sp_svc) = r0_actual;
    }
}

void init_sys_stack(void) {
    for (uint32_t i = 0; i < CANT_TAREAS; i++) {
        tcb_tareas[i].sp_sys = task_stack_SYS[i];

        // Pusheo LR (funcion de la tarea)
        *(--tcb_tareas[i].sp_sys) = (uint32_t) tareas_fun[i];

        // Pusheo R12..R0
        for(uint32_t j = 0; j < 13; j++) {
            *(--tcb_tareas[i].sp_sys) = j;
        }   

        // Pusheo R0 y SPSR
        uint32_t r0_actual = (uint32_t) tcb_tareas[i].sp_sys;
        *(--tcb_tareas[i].sp_sys) = 0x11F; // SPSR para que la tarea corra en modo SYS
        *(--tcb_tareas[i].sp_sys) = r0_actual;
    }
}

__attribute__((section(".kernel_text"))) 
void __scheduler_init(void)
{
    
    // 1. Inicializa los campos básicos de cada TCB (Task Control Block)
    for (uint32_t i = 0; i < CANT_TAREAS; i++) {
        tcb_tareas[i].id = i;
        tcb_tareas[i].ticks = task_ticks[i];
    }
    
    // 2. Llama a las funciones dedicadas para inicializar las pilas de cada modo
    init_irq_stack();
    init_svc_stack();
    init_sys_stack();

}



// Prototipos
__attribute__((section(".kernel_text")))
uint32_t *__scheduler(void) {
    ticks++;  // incremento del sistema de ticks (timer IRQ por ejemplo)
    uint32_t tarea_nueva = 0, tarea_actual = 0; // tarea que estaba corriendo

    tarea_actual = tarea_nueva;

    // Primera vez que cambio de tarea: debo configurar el SP de la tarea inicial
    if (inicio) {
        inicio = NOT_TAREA_INICIAL;

        // Cambio a modo SVC y pongo el SP_SVC de la primera tarea
        __asm__ volatile("CPS #0x13");
        __asm__ volatile("MOV SP, %0" :: "r"(tcb_tareas[tarea_actual].sp_svc));

        // Cambio a modo SYS y pongo el SP_SYS de la primera tarea
        __asm__ volatile("CPS #0x1F");
        __asm__ volatile("MOV SP, %0" :: "r"(tcb_tareas[tarea_actual].sp_sys));

        // Vuelvo al modo IRQ
        __asm__ volatile("CPS #0x12");
        return tcb_tareas[tarea_actual].sp_irq; // Retorno el SP_IRQ de la primera tarea para que se use en handlers.s
    }

    if (ticks >= tcb_tareas[tarea_actual].ticks) {
        ticks = 0; // reinicio el tick

        // Selección round-robin: paso a la próxima tarea (0 a 3)
        tarea_nueva = tcb_tareas[(tarea_nueva + 1) % 4].id;

        // Guardo el SP_IRQ de la tarea actual (SP actual en modo IRQ)
        // true_sp fue guardado por el manejador IRQ
        tcb_tareas[tarea_actual].sp_irq = tarea_sp;

        // Cambio a modo SVC para guardar el SP_SVC de la tarea actual
        __asm__ volatile("CPS #0x13"); // Modo SVC (supervisor)

        // Cambio el SP al de la nueva tarea (modo SVC)
        __asm__ volatile("MOV SP, %0" :: "r"(tcb_tareas[tarea_nueva].sp_svc)); // Cargo SP_SVC de la nueva tarea

        // Cambio a modo SVC para guardar el SP_SVC de la tarea actual
        __asm__ volatile("CPS #0x1F"); // Modo SYS (supervisor)

        // Cambio el SP al de la nueva tarea (modo SYS)
        __asm__ volatile("MOV SP, %0" :: "r"(tcb_tareas[tarea_nueva].sp_sys)); // Cargo SP_SYS de la nueva tarea


        // Vuelvo al modo IRQ
        __asm__ volatile("CPS #0x12");
        // No toco SP_IRQ porque va a ser restaurado por el código en __asm__ que hace el RETI
    }

    return tcb_tareas[tarea_nueva].sp_irq;  // Retorno el SP_IRQ de la nueva tarea para que se use en handlers.s
}

