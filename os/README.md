# CICADA-OS

Este subprojeto é um kernel mínimo bootável desenvolvido como um sistema operacional real.

## Requisitos

- nasm
- gcc-multilib ou gcc com suporte a `-m32`
- binutils
- grub-mkrescue
- qemu-system-i386

## Uso

```bash
cd os
make
make run
```

### Validação automatizada

```bash
make boot-smoke
```

Esse alvo recompila a ISO, verifica a assinatura Multiboot do kernel e confirma que o boot chega até a tela de login.

## Recursos atuais

- kernel mínimo em modo protegido
- GDT/segmentação configurada
- IDT inicializada e PIC remapeado
- PIT timer habilitado e contador de ticks
- scheduler round-robin de tres tarefas (shell + idle + worker)
- teclado capturado por IRQ e buffer de entrada
- base de syscall via `int 0x80`
- engine de missao com 7 niveis, desbloqueio de comandos por fase e pontuacao acumulada
- shell basico com comandos `help`, `clear`, `echo`, `pwd`, `ls`, `cd`, `cat`, `touch`, `mkdir`, `rmdir`, `rm`, `write`, `append`, `cp`, `mv`, `history`, `find`, `tree`, `login`, `logout`, `passwd`, `su`, `sudo`, `who`, `whoami`, `uname`, `ps`, `logread`, `manual`, `decode`, `decrypt`, `hash`, `stego`, `analyze`, `exploit`, `submit`, `hint`, `ticks`, `tasks`, `syscall`, `shutdown` e `exit`
- saída também enviada para a porta serial COM1 para testes com QEMU `-nographic`
- comando dedicado `hint` com dicas por nível (`hint` e `hint <numero>`)
- configuração do GRUB com console local + serial e `gfxpayload=text` para compatibilidade com VirtualBox e hardware BIOS

O arquivo gerado é `os/INITIUM.iso`.
