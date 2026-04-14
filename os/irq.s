BITS 32

global irq0_handler
global irq1_handler
global isr80_handler
extern timer_handler
extern keyboard_handler
extern syscall_handler

section .text
irq0_handler:
    pusha
    call timer_handler
    mov al, 0x20
    out 0x20, al
    popa
    iret

irq1_handler:
    pusha
    call keyboard_handler
    mov al, 0x20
    out 0x20, al
    popa
    iret

isr80_handler:
    pusha
    mov eax, esp
    push eax
    call syscall_handler
    add esp, 4
    mov [esp + 28], eax
    popa
    iret

section .note.GNU-stack
