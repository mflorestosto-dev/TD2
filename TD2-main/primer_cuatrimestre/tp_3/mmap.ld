/* 
    Definiciones necesarias: formato de salida, arquitectura y punto de entrada
*/
OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_reset_vector)


/* 
    Definiciones de simbolos necesarios
*/
_PUBLIC_RAM_INIT = 0x70010000;
_PUBLIC_STACK_INIT = 0x70020000;

_STACK_SIZE         = 256;


/* 
    Definición del mapa de memoria
*/
MEMORY
{
    public_ram	: org = _PUBLIC_RAM_INIT, len = 128M
    public_stack : org = _PUBLIC_STACK_INIT, len = 1M
}

/* 
    Definición de las secciones
*/
SECTIONS
{
    . = _PUBLIC_RAM_INIT;
    .text : { 
        *(.reset*)
        *(.start*) 
        *(.handlers*)
        *(.kernel_text*)    
        *(.text*)
        } > public_ram
    
    .data : { *(.data*) } > public_ram

    .bss : {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss*)
        __bss_end__ = .;
        } > public_ram

    . = _PUBLIC_STACK_INIT;

    .stack : {
        __system_stack_start__ = .;
        
        . += _STACK_SIZE;
        . = ALIGN(4);
        __stack_top_sys__ = .;

        . += _STACK_SIZE;
        . = ALIGN(4);
        __stack_top_svc__ = .;

        . += _STACK_SIZE;
        . = ALIGN(4);
        __stack_top_irq__ = .;

        . += _STACK_SIZE;
        . = ALIGN(4);
        __stack_top_fiq__ = .;

        . += _STACK_SIZE;
        . = ALIGN(4);
        __stack_top_abt__ = .;

        . += _STACK_SIZE;
        . = ALIGN(4);
        __stack_top_und__ = .;

        /*PILAS DE CADA TAREA*/

        /*Pilas de las tareas en modo IRQ*/    
        . += _STACK_SIZE;
        . = ALIGN(4);
        __task_idle_stack_top_irq__ = .;

        . += _STACK_SIZE;
        . = ALIGN(4);
        __task_tarea1_stack_top_irq__ = .;

        . += _STACK_SIZE;
        . = ALIGN(4);
        __task_tarea2_stack_top_irq__ = .;

        . += _STACK_SIZE;
        . = ALIGN(4);
        __task_tarea3_stack_top_irq__ = .;

        /*Pilas de las tareas en modo SVC*/    
        . += _STACK_SIZE;
        . = ALIGN(4);
        __task_idle_stack_top_svc__ = .;

        . += _STACK_SIZE;
        . = ALIGN(4);
        __task_tarea1_stack_top_svc__ = .;

        . += _STACK_SIZE;
        . = ALIGN(4);
        __task_tarea2_stack_top_svc__ = .;

        . += _STACK_SIZE;
        . = ALIGN(4);
        __task_tarea3_stack_top_svc__ = .;

        /*Pilas de las tareas en modo SYS*/    
        . += _STACK_SIZE;
        . = ALIGN(4);
        __task_idle_stack_top_sys__ = .;

        . += _STACK_SIZE;
        . = ALIGN(4);
        __task_tarea1_stack_top_sys__ = .;

        . += _STACK_SIZE;
        . = ALIGN(4);
        __task_tarea2_stack_top_sys__ = .;

        . += _STACK_SIZE;
        . = ALIGN(4);
        __task_tarea3_stack_top_sys__ = .;

        __system_stack_end__ = .;
    } > public_stack

}
