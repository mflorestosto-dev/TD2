#include "stdint.h"
#include "../inc/scheduler.h"

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
void(*tareas_fun[CANT_TAREAS])(void) = {idle, 
                                        tarea1, 
                                        tarea2, 
                                        tarea3};

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

        preparar_contexto_tarea(&tcb_tareas[i], tareas_fun[i]);
    }
}

// Prototipos
void guardar_contexto(uint32_t **sp_actual)
{
    asm volatile(
        "stmdb sp!, {r0-r12, lr} \n"  // guarda registros
        "mrs r0, spsr \n"
        "stmdb sp!, {r0} \n"         // guarda SPSR
        "mov %[output], sp \n"       // guarda nuevo SP
        : [output] "=r" (*sp_actual)
        :
        : "memory", "r0"
    );
}
void restaurar_contexto(uint32_t *nuevo_sp)
{
    asm volatile(
        "mov sp, %[input] \n"         // restauramos SP
        "ldmia sp!, {r0} \n"          // SPSR
        "msr spsr_cxsf, r0 \n"
        "ldmia sp!, {r0-r12, lr} \n"  // registros + LR
        "subs pc, lr, #0 \n"          // vuelve a modo anterior
        :
        : [input] "r" (nuevo_sp)
        : "memory", "r0"
    );
}
uint32_t seleccionar_siguiente_tarea(void)
{
    uint32_t siguiente = (tarea_nueva + 1) % CANT_TAREAS;
    return siguiente;
}

__attribute__((section(".kernel_text")))
void __scheduler(void)
{
    ticks++;

    if (inicio) {
        inicio = 0;
        tarea_nueva = 0;
        true_sp = tcb_tareas[0].sp_irq;
        restaurar_contexto(true_sp);
        return;
    }

    uint32_t tarea_actual = tarea_nueva;

    // Si aún tiene tiempo de ejecución, seguimos
    if (--tcb_tareas[tarea_actual].ticks > 0)
        return;

    // Se terminó su tiempo → buscar siguiente
    tcb_tareas[tarea_actual].ticks = task_ticks[tarea_actual];

    // Guardamos contexto actual
    guardar_contexto(&tcb_tareas[tarea_actual].sp_irq);

    // Seleccionamos próxima tarea
    tarea_nueva = seleccionar_siguiente_tarea();

    // Restauramos contexto de la nueva tarea
    true_sp = tcb_tareas[tarea_nueva].sp_irq;
    restaurar_contexto(true_sp);
}
