#include <stdint.h>

typedef struct {
    uint32_t* sp_svc;
    uint32_t* sp_irq;        // Stack pointer de la tarea
    uint32_t  ticks;         // Ticks asignados a esta tarea
    uint32_t  id;            // Cuántos ticks van ejecutados
} TCB;

typedef struct {
    uint32_t sp, spsr, r0_r12[13], lr;
}context_t;

uint32_t *__scheduler(void);
void __scheduler_init(void);
void preparar_contexto_tarea(TCB *tcb, void (*entry_point)(void));
