#include "../../inc/gic.h"    // El header de gic.c 
#include "../../inc/timer.h"  // El header de timer.c
#include "../../inc/scheduler.h"   // El header de scheduler.c
#include "../../inc/uart.h"  // El header de uart.c

void board_init(void);
__attribute__((section(".text"))) void board_init()
{
    
    //Inicializacion de gic y timer
    __gic_init();
    __timer_init();
    __scheduler_init();
    __uart_init(0);
}