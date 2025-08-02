#include "stdint.h"
#include "../inc/tareas.h"

__attribute__((section(".data"))) int global_A = 0;
__attribute__((section(".data"))) int global_B = 0;

__attribute__((section(".tarea_idle_text"))) void tarea_idle(void){
    while(1){
        asm("WFI");
    }
}

__attribute__((section(".tarea_1_text"))) void tarea_1(void){
    int local_A = 0;
    while(1){
        local_A += 1;
        global_A += 1;
    }
}

__attribute__((section(".tarea_2_text"))) void tarea_2(void){
    int local_B = 0;
    while(1){
        local_B -= 1;
        global_B -= 1;
    }
}

__attribute__((section(".tarea_3_text"))) void tarea_3(void){
    while(1){
        global_A -= 1;
        global_B += 1;
    }
}