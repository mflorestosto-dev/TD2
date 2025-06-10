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

_STACK_SIZE = 0x100;

/* 
    Definición del mapa de memoria
*/
MEMORY
{
    public_ram	: org = _PUBLIC_RAM_INIT, len = 0x1000
    public_stack : org = _PUBLIC_STACK_INIT, len = 0x1000
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
        } > public_ram
    
    .data : { *(.data*) } > public_ram

    .bss : {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss*)
        __bss_end__ = .;
        } > public_ram

    . = _PUBLIC_STACK_INIT

    . stack : ALIGN(8) {
        _system_heap_top = .;
        . += _SYSTEM_HEAP_LEAN * 4;
        _system_heap_bottom = .;
        
        /* Pilas por modo */
        . = ALIGN(8);
        stack_sys_start = .;
        . += _STACK_SIZE;
        stack_top_sys = .;

        . = ALIGN(8);
        stack_svc_start = .;
        . += _STACK_SIZE;
        stack_top_svc = .;

        . = ALIGN(8);
        stack_irq_start = .;
        . += _STACK_SIZE;
        stack_top_irq = .;

        . = ALIGN(8);
        stack_fiq_start = .;
        . += _STACK_SIZE;
        stack_top_fiq = .;

        . = ALIGN(8);
        stack_abt_start = .;
        . += _STACK_SIZE;
        stack_top_abt = .;

        . = ALIGN(8);
        stack_und_start = .;
        . += _STACK_SIZE;
        stack_top_und = .;
    } > public_stack
}
