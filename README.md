# CICADA-3301

Este repositório agora contém dois projetos:

- `CICADA-3301`: simulação de terminal e ambiente CTF em Python.
- `os/`: subprojeto de kernel mínimo bootável com GRUB e QEMU.

Use `cd os && make` para construir a imagem ISO do novo sistema operacional real.

## Comandos úteis



## Teste em VirtualBox

Para validar se a correção de boot funciona em VirtualBox (importante antes de merge):

```bash
# 1. Criar VM e configurar (1 min)
make vbox-setup

# 2. Iniciar VM e validar automaticamente (2 min)
make vbox-test

# 3. Limpeza após testes
make vbox-clean
```

Se preferir testes manuais, veja [VIRTUALBOX_QUICK_START.md](VIRTUALBOX_QUICK_START.md) para procedimento passo-a-passo.

**Status atual**: Não testado em VirtualBox
