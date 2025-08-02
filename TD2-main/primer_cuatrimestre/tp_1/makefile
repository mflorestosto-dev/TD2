CHAIN=arm-none-eabi
CFLAGS=-std=gnu99 -Wall -mfpu=neon -mhard-float -mcpu=cortex-a8 -DCPU_A8 -g
# CFLAGS=-std=gnu99 -Wall -mfpu=neon -mcpu=cortex-a8
AFLAGS=-g
OBJ=obj/
BIN=bin/
INC=inc/
SRC=src/
LST=lst/

# all: $(BIN)bios.bin $(BIN)bios.MLO $(OBJ)bios.elf
all: $(BIN)bios.bin $(OBJ)bios.elf

$(BIN)bios.bin: $(OBJ)bios.elf
	$(CHAIN)-objcopy -O binary $< $@

#$(CHAIN)-ld --defsym BOOT_START_ADDR=$(START_ADDR) -T td3bbb_memmap.ld $(OBJ)*.o -o $(OBJ)bios.elf -Map $(LST)bios_ld_map.map 

$(OBJ)bios.elf: $(OBJ)startup.o $(OBJ)handlers.o $(OBJ)reset_vector.o $(OBJ)kernel_handler_irq.o $(OBJ)board_init.o $(OBJ)gic.o $(OBJ)timer.o
	@echo "Linkeando..."
	mkdir -p obj
	mkdir -p lst
	mkdir -p inc
	$(CHAIN)-ld -T mmap.ld $(OBJ)*.o -o $(OBJ)bios.elf -Map $(LST)bios_ld_map.map 
	@echo "Linkeo finalizado!!"
	@echo ""
	@echo "Generando archivos de información: mapa de memoria y simbolos"
	readelf -a $(OBJ)bios.elf > $(LST)bios_readelf.txt
	$(CHAIN)-objdump -D $(OBJ)bios.elf > $(LST)bios.lst

	
$(OBJ)kernel_handler_irq.o: $(SRC)kernel_handler_irq.c
	mkdir -p $(OBJ)
	$(CHAIN)-gcc $(CFLAGS) -c $< -o $@

$(OBJ)timer.o: $(SRC)timer.c
	mkdir -p $(OBJ)
	$(CHAIN)-gcc $(CFLAGS) -c $< -o $@

$(OBJ)gic.o: $(SRC)gic.c
	mkdir -p $(OBJ)
	$(CHAIN)-gcc $(CFLAGS) -c $< -o $@

$(OBJ)board_init.o: $(SRC)board_init.c
	mkdir -p $(OBJ)
	$(CHAIN)-gcc $(CFLAGS) -c $< -o $@
#
# Ensamblado de código assembler
#

# Ensamblamos: startup.s
$(OBJ)startup.o: $(SRC)startup.s
	@echo ""
	mkdir -p bin
	mkdir -p obj
	mkdir -p lst
	@echo "Ensamblando startup.s ..."
	$(CHAIN)-as $(AFLAGS) $(SRC)startup.s -o $(OBJ)startup.o -a > $(LST)startup.lst

# Ensamblamos: reset_vector.s
$(OBJ)reset_vector.o: $(SRC)reset_vector.s
	@echo ""
	mkdir -p bin
	mkdir -p obj
	mkdir -p lst	
	@echo "Ensamblando reset_vector.s ..."
	$(CHAIN)-as $(AFLAGS) $(SRC)reset_vector.s -o $(OBJ)reset_vector.o -a > $(LST)reset_vector.lst

# Ensamblamos: reset_vector.s
$(OBJ)handlers.o: $(SRC)handlers.s
	@echo ""
	mkdir -p bin
	mkdir -p obj
	mkdir -p lst	
	@echo "Ensamblando handlers.s ..."
	$(CHAIN)-as $(AFLAGS) $(SRC)handlers.s -o $(OBJ)handlers.o -a > $(LST)handlers.lst

clean:
	rm -rf $(OBJ)*.o
	rm -rf $(OBJ)*.objstart
	rm -rf $(OBJ)*.elf
	rm -rf $(BIN)*.bin
	rm -rf $(LST)*.lst 
	rm -rf $(LST)*.txt 
	rm -rf $(LST)*.map 
