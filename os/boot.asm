; os/boot.asm
BITS 32

SECTION .multiboot
ALIGN 4
    dd 0x1BADB002
    dd 0x00000003
    dd -(0x1BADB002 + 0x00000003)

SECTION .text
global start
extern kernel_main

start:
    cli
    cld
    xor ebp, ebp
    mov eax, 0x90000
    mov esp, eax
    call kernel_main

.hang:
    hlt
    jmp .hang

SECTION .note.GNU-stack
