
#include "../../inc/gic.h"

void __gic_init(void );

__attribute__((section(".kernel_text"))) void __gic_init() {
        _gicc_t* const GICC0 = (_gicc_t*)GICC0_ADDR;
        _gicd_t* const GICD0 = (_gicd_t*)GICD0_ADDR;

        GICC0->PMR  = 0x000000F0;          // Completar que se hace en estas lineas, segun manual de usuario
        GICD0->ISENABLER[1] |= 0x00000010; // Completar que se hace en estas lineas, segun manual de usuario
        GICD0->ISENABLER[1] |= 0x00001000; // Completar que se hace en estas lineas, segun manual de usuario
        GICC0->CTLR         = 0x00000001;  // Completar que se hace en estas lineas, segun manual de usuario
        GICD0->CTLR         = 0x00000001;  // Completar que se hace en estas lineas, segun manual de usuario
    }