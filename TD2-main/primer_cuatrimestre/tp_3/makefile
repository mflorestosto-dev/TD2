CHAIN=arm-none-eabi
CFLAGS=-std=gnu99 -Wall -mfpu=neon -mhard-float -mcpu=cortex-a8 -DCPU_A8 -g
AFLAGS=-g -mfpu=neon -mfloat-abi=hard
OBJ=obj/
BIN=bin/
INC=inc/
SRC=src/
LST=lst/

# all: $(BIN)bios.bin $(BIN)bios.MLO $(OBJ)bios.elf
all: $(BIN)bios.bin $(OBJ)bios.elf

$(BIN)bios.bin: $(OBJ)bios.elf
	$(CHAIN)-objcopy -O binary $< $@

$(OBJ)bios.elf: $(OBJ)startup.o $(OBJ)handlers.o $(OBJ)reset_vector.o $(OBJ)kernel_handler_irq.o $(OBJ)board_init.o $(OBJ)gic.o $(OBJ)timer.o $(OBJ)scheduler.o $(OBJ)tareas.o $(OBJ)uart.o $(OBJ)kernel_handler_swi.o $(OBJ)syscalls.o $(OBJ)console_utils.o
	@echo "Linkeando..."
	mkdir -p obj
	mkdir -p lst
	mkdir -p inc
	$(CHAIN)-gcc -nostartfiles -nostdlib -T mmap.ld $(OBJ)*.o -o $(OBJ)bios.elf -Wl,-Map,$(LST)bios_ld_map.map -lgcc
	@echo ""
	@echo "Generando archivos de información: mapa de memoria y simbolos"
	readelf -a $(OBJ)bios.elf > $(LST)bios_readelf.txt
	$(CHAIN)-objdump -D $(OBJ)bios.elf > $(LST)bios.lst

$(OBJ)kernel_handler_irq.o: $(SRC)irq/kernel_handler_irq.c
	mkdir -p $(OBJ)
	$(CHAIN)-gcc $(CFLAGS) -c $< -o $@

$(OBJ)timer.o: $(SRC)board/timer.c
	mkdir -p $(OBJ)
	$(CHAIN)-gcc $(CFLAGS) -c $< -o $@

$(OBJ)gic.o: $(SRC)board/gic.c
	mkdir -p $(OBJ)
	$(CHAIN)-gcc $(CFLAGS) -c $< -o $@

$(OBJ)board_init.o: $(SRC)kernel/board_init.c
	mkdir -p $(OBJ)
	$(CHAIN)-gcc $(CFLAGS) -c $< -o $@

$(OBJ)scheduler.o: $(SRC)tasks/scheduler.c
	mkdir -p $(OBJ)
	$(CHAIN)-gcc $(CFLAGS) -c $< -o $@

$(OBJ)tareas.o: $(SRC)tasks/tareas.c
	mkdir -p $(OBJ)
	$(CHAIN)-gcc $(CFLAGS) -c $< -o $@

$(OBJ)uart.o: $(SRC)board/uart.c
	mkdir -p $(OBJ)
	$(CHAIN)-gcc $(CFLAGS) -c $< -o $@

$(OBJ)kernel_handler_swi.o: $(SRC)irq/kernel_handler_swi.c
	mkdir -p $(OBJ)
	$(CHAIN)-gcc $(CFLAGS) -c $< -o $@

$(OBJ)syscalls.o: $(SRC)irq/syscalls.c
	mkdir -p $(OBJ)
	$(CHAIN)-gcc $(CFLAGS) -c $< -o $@

$(OBJ)console_utils.o: $(SRC)utils/console_utils.c
	mkdir -p $(OBJ)
	$(CHAIN)-gcc $(CFLAGS) -c $< -o $@



#
# Ensamblado de código assembler
#

# Ensamblamos: startup.s
$(OBJ)startup.o: $(SRC)kernel/startup.s
	@echo ""
	mkdir -p $(BIN)
	mkdir -p $(OBJ)
	mkdir -p $(LST)
	@echo "Ensamblando startup.s ..."
	$(CHAIN)-as $(AFLAGS) $(SRC)kernel/startup.s -o $(OBJ)startup.o -a > $(LST)startup.lst

# Ensamblamos: reset_vector.s
$(OBJ)reset_vector.o: $(SRC)kernel/reset_vector.s
	@echo ""
	mkdir -p $(BIN)
	mkdir -p $(OBJ)
	mkdir -p $(LST)
	@echo "Ensamblando reset_vector.s ..."
	$(CHAIN)-as $(AFLAGS) $(SRC)kernel/reset_vector.s -o $(OBJ)reset_vector.o -a > $(LST)reset_vector.lst

# Ensamblamos: handlers.s
$(OBJ)handlers.o: $(SRC)kernel/handlers.s
	@echo ""
	mkdir -p $(BIN)
	mkdir -p $(OBJ)
	mkdir -p $(LST)
	@echo "Ensamblando handlers.s ..."
	$(CHAIN)-as $(AFLAGS) $(SRC)kernel/handlers.s -o $(OBJ)handlers.o -a > $(LST)handlers.lst

clean:
	rm -rf $(OBJ)*.o
	rm -rf $(OBJ)*.objstart
	rm -rf $(OBJ)*.elf
	rm -rf $(BIN)*.bin
	rm -rf $(LST)*.lst 
	rm -rf $(LST)*.txt 
	rm -rf $(LST)*.map