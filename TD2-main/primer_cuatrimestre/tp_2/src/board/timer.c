
#include "../../inc/timer.h"

void __timer_init(void );

__attribute__((section(".kernel_text"))) void __timer_init() {
        _timer_t* const TIMER0 = ( _timer_t* )TIMER0_ADDR;

        TIMER0->Timer1Load     = 0x00010000; // Completar que se hace en estas lineas, segun manual de usuario
        TIMER0->Timer1Ctrl     = 0x00000002; // Completar que se hace en estas lineas, segun manual de usuario
        TIMER0->Timer1Ctrl    |= 0x00000040; // Completar que se hace en estas lineas, segun manual de usuario
        TIMER0->Timer1Ctrl    |= 0x00000020; // Completar que se hace en estas lineas, segun manual de usuario
        TIMER0->Timer1Ctrl    |= 0x00000080; // Completar que se hace en estas lineas, segun manual de usuario
    }