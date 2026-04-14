AS=nasm
CC=gcc
LD=ld
CFLAGS=-m32 -nostdlib -fno-builtin -fno-stack-protector -O2 -Wall -Wextra
LDFLAGS=-m elf_i386 -T linker.ld
OBJS=boot.o kernel.o irq.o

.PHONY: all clean run

all: os.iso

boot.o: boot.asm
	$(AS) -f elf32 boot.asm -o boot.o

irq.o: irq.s
	$(AS) -f elf32 irq.s -o irq.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) -o kernel.bin $(OBJS)

os.iso: kernel.bin grub.cfg
	rm -rf isodir
	mkdir -p isodir/boot/grub
	cp kernel.bin isodir/boot/
	cp grub.cfg isodir/boot/grub/
	grub-mkrescue -o os.iso isodir

run: os.iso
	qemu-system-i386 -cdrom os.iso -boot d -m 512 -nographic -monitor none -serial stdio -device isa-debug-exit,iobase=0xf4,iosize=0x04

clean:
	rm -f *.o kernel.bin os.iso
	rm -rf isodir
