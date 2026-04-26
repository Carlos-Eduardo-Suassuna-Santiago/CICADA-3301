# Teste VirtualBox - Quick Start

## 🚀 Start Rápido (3 comandos)

```bash
# 1. Criar e configurar VM
bash scripts/virtualbox_test_setup.sh

# 2. (Em outra aba) Monitorar boot em tempo real
watch -n 1 'tail -20 /tmp/cicada_virtualbox_CICADA-3301-TEST.log'

# 3. (De volta) Iniciar VM e validar
VBoxManage startvm CICADA-3301-TEST
sleep 5
bash scripts/virtualbox_test_validate.sh
```

**Tempo esperado**: ~2 minutos até resultado

---

## 📋 Procedimento Completo

### Pré-requisitos
- VirtualBox 6.0+ instalado
- ISO gerado: `./os/INITIUM.iso`
- **IMPORTANTE**: Sistema operacional pode ser Windows, Linux ou macOS

### Fase 1: Criar VM (1 min)

```bash
chmod +x scripts/virtualbox_test_setup.sh
./scripts/virtualbox_test_setup.sh CICADA-3301-TEST /workspaces/CICADA-3301/os/INITIUM.iso
```

**O que acontece:**
- ✅ Cria VM com 512MB RAM, 2 CPUs
- ✅ Anexa ISO como boot device
- ✅ Configura serial output para log file
- ✅ Pronto para boot

**Output esperado:**
```
[1/4] Verificando pré-requisitos...
✓ VirtualBox encontrado
✓ ISO encontrada

[4/4] Resumo da Configuração
VM Criada: CICADA-3301-TEST
...
✓ Setup concluído!
```

### Fase 2: Iniciar VM (10-20 seg)

**Opção A: Via CLI (sem GUI)**
```bash
VBoxManage startvm CICADA-3301-TEST --type headless
```

**Opção B: Via GUI (recomendado para debug visual)**
```bash
VBoxManage startvm CICADA-3301-TEST
# Abre janela VirtualBox com VM em boot
```

Nesta janela você deve ver:
```
GRUB menu...
         GNU GRUB 2.06~rc1

[CICADA-3301 - Multiboot Kernel]

Loading kernel...
BOOT:0 - Checking Multiboot signature...
BOOT:1 - GDT initialized...
...
BOOT:7 shell start
=== LOGIN ===
```

**Se aparecer erro:**
- ❌ `GRUB: file not found` → ISO corrompida, reconstruir
- ❌ `Kernel panic` → Problema em boot.asm ou kernel.c
- ❌ Tela preta → Problema in display/serial conf

### Fase 3: Monitorar Boot (abrir em outro terminal)

Enquanto VM está inicializando:

```bash
# Terminal 2
watch -n 1 'tail -20 /tmp/cicada_virtualbox_CICADA-3301-TEST.log'
```

Você deve ver output similar:

```
BOOT:0 - Checking Multiboot signature...
BOOT:1 - GDT initialized...
BOOT:2 - IDT initialized...
BOOT:3 - PIC remapped...
BOOT:4 - PIT initialized at ~100Hz...
BOOT:5 - Paging disabled (identity mapping)
BOOT:6 - Kernel ready, initializing shell...
BOOT:7 shell start
=== LOGIN ===
```

Que significa:
- ✅ Bootloader (GRUB) funcionando
- ✅ Kernel init (GDT/IDT) funcionando
- ✅ Interrupções (PIC/PIT) funcionando
- ✅ Shell iniciado e aguardando entrada

### Fase 4: Validação Automatizada (1 min)

De volta ao Terminal 1:

```bash
chmod +x scripts/virtualbox_test_validate.sh
./scripts/virtualbox_test_validate.sh CICADA-3301-TEST 60
```

**Output esperado:**
```
[Validando boot markers...]

  ✓ Encontrado: BOOT:0
  ✓ Encontrado: BOOT:7
  ✓ Encontrado: LOGIN

✓ VALIDAÇÃO PASSOU
```

### Fase 5: Teste Interativo (5 min, opcional)

Na janela da VM, você pode testar comandos:

```
=== LOGIN ===
$ whoami
root

$ help
Comandos disponíveis:
  cd, ls, pwd, logout...

$ logout
=== LOGIN ===
```

Se funcionar, então VirtualBox compatibility ✅

---

## 📊 Resultados Esperados

| Teste | Status | Critério de Sucesso |
|-------|--------|-------------------|
| **Boot Básico** | 🟢 PASS | BOOT:0 até BOOT:7 em log |
| **Login Prompt** | 🟢 PASS | `=== LOGIN ===` detectado |
| **Comandos** | 🟢 PASS | `whoami` → `root` |
| **Serial Capture** | 🟢 PASS | Log file atualizado em tempo real |

---

## ❌ Troubleshooting

### Problema: "VM não encontrada"
```bash
# Listar VMs disponíveis
VBoxManage list vms

# Se não aparecer, criar com setup script novamente
./scripts/virtualbox_test_setup.sh CICADA-3301-TEST
```

### Problema: "Tela preta, sem output"
1. Verificar se screenshot aparece conteúdo:
   ```bash
   VBoxManage controlvm CICADA-3301-TEST screenshotpng /tmp/screenshot.png
   file /tmp/screenshot.png  # Se 0 bytes, problema no display
   ```

2. Reconstruir ISO:
   ```bash
   cd os && make clean all && cd ..
   # Re-anexar ISO: ./scripts/virtualbox_test_setup.sh
   ```

3. Tentar em modo headless com mais verbosidade:
   ```bash
   VBoxManage startvm CICADA-3301-TEST --type headless
   sleep 10
   tail -100 /tmp/cicada_virtualbox_CICADA-3301-TEST.log
   ```

### Problema: "Validação falhou - markers não encontrados"

Significa boot não completou. Checklist:

1. VM está realmente rodando?
   ```bash
   VBoxManage list runningvms | grep CICADA
   ```

2. ISO está correta?
   ```bash
   file os/INITIUM.iso
   # Deve ser: ISO 9660 CD-ROM filesystem data
   ```

3. GRUB config está OK?
   ```bash
   cat os/grub.cfg | head -20
   # Deve ter: terminal_input console serial
   ```

4. Se tudo OK, deixar rodar mais tempo:
   ```bash
   ./scripts/virtualbox_test_validate.sh CICADA-3301-TEST 120  # 120 segundos
   ```

---

## 📌 Próximos Passos

**Se ✅ PASS:**
1. Documentar resultado em [docs/VIRTUALBOX_TESTING.md](docs/VIRTUALBOX_TESTING.md)
2. Atualizar tabela de resultados
3. OK para mergear branch `OS/correcao_bugs` → `main`

**Se ❌ FAIL:**
1. Coletar logs completos:
   ```bash
   cp /tmp/cicada_*.log ./test_results/
   git add test_results/
   ```
2. Abrir issue com logs + screenshots
3. Investigar problema em `os/` e iterar

---

## 🧹 Limpeza

Quando terminar os testes:

```bash
# Parar VM
VBoxManage controlvm CICADA-3301-TEST poweroff

# Deletar VM (libera espaço)
VBoxManage unregistervm CICADA-3301-TEST --delete

# Limpar logs
rm /tmp/cicada_*.log
```

---

**Tempo total**: ~30 minutos (setup + testes + cleanup)
**Última atualização**: 2026-04-24
