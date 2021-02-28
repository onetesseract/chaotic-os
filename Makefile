SOURCES=boot.o main.o
C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c)
AS_SOURCES = $(wildcard kernel/*.s drivers/*.s cpu/*.s libc/*.s)
ASM_SOURCES = $(wildcard kernel/*.asm drivers/*.asm cpu/*.asm libc/*.asm)
# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o} ${AS_SOURCES:.s=.o} ${ASM_SOURCES:.asm=.o}
CFLAGS= -I.
LDFLAGS=-T link.ld
ASFLAGS=

os.bin: boot/bootsect.bin kernel.bin
	cat $^ > os.bin
# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
kernel.bin: boot/kernel_entry.o ${OBJ}
	i686-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

run: os.bin
	qemu-system-i386 -fda os.bin -serial file:serial.log

clean:
	rm *.o kernel_out $(OBJ) boot/*.o boot/*.bin

kernel_out:
	i686-elf-ld $(LDFLAGS) -o kernel_out $(OBJ)

.s.o:
	i686-elf-as $(ASFLAGS) $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

.c.o:
	i686-elf-gcc $(CFLAGS) -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -ffreestanding -c $< -o $@