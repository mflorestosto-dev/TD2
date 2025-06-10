#include "gic.h"    // El header de gic.c 
#include "timer.h"  // El header de timer.c

void board_init(void) {
    
    //Inicializacion de gic y timer
    __gic_init();
    __timer_init();
}