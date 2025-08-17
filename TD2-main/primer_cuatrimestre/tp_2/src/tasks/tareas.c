#include "stdint.h"
#include "../../inc/tareas.h"

int global_A = 0;
int global_B = 0;

__attribute__((section(".text"))) void tarea_idle(void){
    __asm__("CPS #0x1F"); // Cambiar a modo SYS
    while(1){
        __asm__("WFI");
    }
}

__attribute__((section(".text"))) void tarea_1(void){
    static int local_A = 0;
    __asm__("CPS #0x1F"); // Cambiar a modo SYS
    while(1){
        local_A += 1;
        global_A += 1;
    }
}

__attribute__((section(".text"))) void tarea_2(void){
    static int local_B = 0;
    __asm__("CPS #0x1F"); // Cambiar a modo SYS
    while(1){
        local_B -= 1;
        global_B -= 1;
    }
}

__attribute__((section(".text"))) void tarea_3(void){
    __asm__("CPS #0x1F"); // Cambiar a modo SYS
    while(1){
        global_A -= 1;
        global_B += 1;
    }
}