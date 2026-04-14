OUTPUT_FORMAT(elf32-i386)
ENTRY(start)

PHDRS
{
    text PT_LOAD FLAGS(5);
    data PT_LOAD FLAGS(6);
}

SECTIONS
{
    . = 1M;
    .multiboot ALIGN(4) : { *(.multiboot) } :text
    .text ALIGN(4K) : { *(.text) } :text
    .rodata ALIGN(4K) : { *(.rodata*) } :text

    .data ALIGN(4K) : { *(.data*) } :data
    .bss ALIGN(4K) :
    {
        *(COMMON)
        *(.bss*)
    } :data
}
