void idle(void);

__attribute__((section(".kernel_text"))) void idle(void){
    __asm__("WFI");
}