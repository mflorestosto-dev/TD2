#include "gic.h"    // El header de gic.c 
#include "timer.h"  // El header de timer.c

void board_init(void);
__attribute__((section(".text"))) void board_init()
{
    
    //Inicializacion de gic y timer
    __gic_init();
    __timer_init();
    __scheduler_init();
}