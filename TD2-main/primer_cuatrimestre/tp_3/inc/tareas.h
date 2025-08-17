#include "stdint.h"

void tarea_1(void);
void tarea_2(void);
void tarea_3(void);
void tarea_idle(void);

#define CANT_TAREAS 4
#define TIEMPO_EJECUCION_TOTAL 30
#define TICKS_TAREA_1 8
#define TICKS_TAREA_2 12
#define TICKS_TAREA_3 5
#define TICKS_IDLE_TASK  (TIEMPO_EJECUCION_TOTAL - (TICKS_TAREA_1 + TICKS_TAREA_2 + TICKS_TAREA_3))
#define TAREA_INICIAL 1
#define NOT_TAREA_INICIAL 0