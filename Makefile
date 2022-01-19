CC = arm-none-eabi-gcc
CFLAGS = -c -MMD -mcpu=cortex-m0plus -mthumb -mfloat-abi=soft -O0 -g3 -Wall -fmessage-length=0 -Werror-implicit-function-declaration -Wno-comment -Wno-unused-function -ffunction-sections -fdata-sections -ffreestanding -nostdlib
LDFLAGS = -mcpu=cortex-m0plus -mthumb -mfloat-abi=soft -specs=nosys.specs -specs=nano.specs -T linkerscript.ld -Wl,-Map=a.map -Wl,--gc-sections -ffreestanding -nostdlib

all: a.elf dump

a.o: main.c
	@printf "\nCompiling...\n"
	$(CC) main.c -o a.o $(CFLAGS)

a.elf: a.o
	@printf "\nLinking...\n"
	$(CC) a.o -o a.elf $(LDFLAGS)

dump: a.elf
	@printf "\nCreating binary and programming files...\n"
	arm-none-eabi-objcopy -O binary a.elf a.bin
	arm-none-eabi-objcopy -O ihex a.elf a.hex
	arm-none-eabi-objcopy -O srec a.elf a.srec
	arm-none-eabi-objdump -d a.elf > a.list
	arm-none-eabi-size a.elf

clean:
	rm -rf a.*

