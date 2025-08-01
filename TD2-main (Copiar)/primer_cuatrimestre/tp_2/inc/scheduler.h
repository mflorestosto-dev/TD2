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

void tarea1(void);
void tarea2(void);
void tarea3(void);
void idle(void);
