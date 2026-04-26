# Testes em VirtualBox - CICADA-3301 OS

## Objetivo

Validar se a correção de boot (GRUB com console+serial+gfxpayload) funciona corretamente em VirtualBox antes de fazer merge da branch `OS/correcao_bugs` para `main`.

## Status

![Status](https://img.shields.io/badge/status-testing-yellow) Não testado em VirtualBox yet

---

## Checklist de Configuração VirtualBox

### 1. Criar Nova VM

```bash
# Comando para criar VM via VirtualBox CLI (opcional)
VBoxManage createvm --name "CICADA-3301-TEST" --ostype Linux26_64 --register
VBoxManage modifyvm CICADA-3301-TEST --memory 512 --cpus 2 --vram 12
VBoxManage modifyvm CICADA-3301-TEST --nic1 nat --nictype1 virtio
VBoxManage createhd --filename ~/VirtualBox\ VMs/CICADA-3301-TEST/disk.vdi --size 10240
VBoxManage storagectl CICADA-3301-TEST --name "SATA" --add sata
VBoxManage storageattach CICADA-3301-TEST --storagectl "SATA" --port 0 --device 0 --type hdd --medium ~/VirtualBox\ VMs/CICADA-3301-TEST/disk.vdi
```

**Ou manualmente via GUI:**

| Configuração | Valor | Motivo |
|------------|-------|--------|
| **VM Name** | CICADA-3301-TEST | Identificação clara |
| **OS Type** | Linux / Other Linux (32-bit) | Suporta i386 multiboot |
| **Memory** | 512 MB | Suficiente para kernel+shell |
| **CPU** | 2 cores | Respeita PIT 100Hz scheduler |
| **Video** | 12 MB VRAM, VBoxVGA | Suporta VGA 0xB8000 |
| **Network** | NAT (virtio) | Boot não depende de rede |
| **Storage** | 10 GB | Espaço para futuras expansões |
| **Chipset** | PIIX3 | Compatível com BIOS legacy (não UEFI) |

### 2. Configurar Boot Media

1. **Baixar ISO**: Obter `/os/INITIUM.iso` do repositório
   ```bash
   # No repo local
   cd /workspaces/CICADA-3301/os
   make clean all  # Gera INITIUM.iso
   ```

2. **Anexar ISO à VM**:
   - VirtualBox GUI → Settings → Storage → Controller SATA
   - Clicar em ícone CD vazio → "Choose a disk file"
   - Selecionar `INITIUM.iso`

3. **Configurar boot order**:
   - Settings → System → Boot Order
   - Colocar "CD/DVD" como primeira opção
   - Garantir "Floppy" ou "Hard Disk" como fallback

### 3. Configurações Opcionais de Debug

Para capturar saída serial em arquivo (equivalente a boot_smoke.sh):

**Via GUI (Advanced):**
- Settings → Serial Ports → Port 1
  - ✅ Enable Serial Port
  - Port Number: COM1
  - Port Mode: **Host Pipe** ou **Host Device** (`/dev/ttyS0` em Linux, `COM1` em Windows)

**Via comando VBoxManage:**
```bash
VBoxManage modifyvm CICADA-3301-TEST \
  --uart1 0x3F8 115200 \
  --uartmode1 "file:/tmp/vm_serial.log"
```

---

## Procedimento de Teste

### Teste 1: Boot Básico (5 min)

**Passos:**
1. Iniciar VM em VirtualBox
2. Observar GRUB menu aparecendo
3. Pressionar ENTER para boot kernel (ou esperar 5s timeout automático)
4. Verificar se aparece:
   - `BOOT:0` - Multiboot signature detected
   - Sequência até `BOOT:7 shell start`
   - Prompt de login: `=== LOGIN ===`

**Resultado Esperado:**
```
BOOT:0 - Checking Multiboot signature...
BOOT:1 - GDT initialized...
BOOT:2 - IDT initialized...
BOOT:3 - PIC remapped...
BOOT:4 - PIT initialized at ~100Hz...
BOOT:5 - Paging disabled (identity mapping)
BOOT:6 - Kernel ready
BOOT:7 shell start
=== LOGIN ===
```

**Falha = investigar**:
- ❌ GRUB não aparece → Problema de boot order/ISO
- ❌ Kernel panic após BOOT:3 → Problema em IDT/PIC
- ❌ Sem login prompt → Shell não iniciou

---

### Teste 2: Entrada de Comando (5 min)

**Passos:**
1. Na tela de login, digitar: `whoami`
2. Pressionar ENTER
3. Verificar resposta
4. Digitar `logout`
5. Verificar se retorna ao login

**Resultado Esperado:**
```
=== LOGIN ===
$ whoami
root

$ logout
=== LOGIN ===
```

**Falha = investigar**:
- ❌ Comando não respondendo → Problema em PIT/scheduler
- ❌ Caracteres não aparecem → Problema em teclado VBoxVGA
- ❌ Logout não limpa tela → Problema em shell state machine

---

### Teste 3: Comandos Python CTF (5 min)

**Passos:**
1. Na tela de login, digitar: `login`
2. Responder com comportamento esperado (desafio 1 deve estar desbloqueado)
3. Executar: `decode`
4. Copiar output e testar se decodificação funciona

**Resultado Esperado:**
```
$ login
<prompt para configuração>
$ decode
Base64 string here...
$ [próximo comando desbloqueado]
```

---

### Teste 4: Persistência Serial (opcional, 5 min)

Se configurou `-uartmode1 file`:

**Passos:**
1. Deixar VM rodando por 30 segundos após login
2. Executar alguns comandos (whoami, help, etc)
3. Parar VM
4. Verificar arquivo de log:
   ```bash
   tail -100 /tmp/vm_serial.log
   ```

**Resultado Esperado:**
Log contém toda sequência de boot + comandos executados:
```
BOOT:0...
BOOT:7 shell start
=== LOGIN ===
$ whoami
root
$ help
...
```

---

## Tabela de Resultados

| Teste | Status | Observações | Log |
|-------|--------|-------------|-----|
| Boot Básico | ⬜ Not Run | - | - |
| Entrada Comando | ⬜ Not Run | - | - |
| Comandos CTF | ⬜ Not Run | - | - |
| Persistência Serial | ⬜ Not Run | - | - |

**Preenchimento:**
- ⬜ = Not Run (cinza)
- 🟨 = In Progress (amarelo)
- ✅ = Pass (verde)
- ❌ = Fail (vermelho)

---

## Problema de Compatibilidade Esperado

### Cenário 1: GRUB não aparece na tela
**Causa provável**: Boot order incorreta ou ISO corrompida
**Solução**:
```bash
# Reconstruir ISO
cd /workspaces/CICADA-3301/os
make clean
make all
# Re-anexar INITIUM.iso à VM
```

### Cenário 2: Kernel boot mas sem tela (apenas preto)
**Causa provável**: Console não configurado corretamente em GRUB
**Solução**: Verificar grub.cfg:
```bash
cat os/grub.cfg | grep -A5 "linux"
```
Deve conter:
```
terminal_input console serial
terminal_output console serial
set gfxpayload=text
```

### Cenário 3: Linha de comando "pisca" ou não responde
**Causa provável**: PIT/timer não funcionando corretamente em VBox
**Solução**: Tentar com configuração diferente:
- Reduzir para 1 CPU
- Aumentar timeout de boot
- Habilitar HAL/APIC simulation

---

## Próximos Passos Após Teste

### ✅ Se tudo passar:
1. Atualizar [#1](https://github.com/Carlos-Eduardo-Suassuna-Santiago/CICADA-3301/issues/1) com resultado ("VirtualBox: ✅ PASS")
2. Mergear `OS/correcao_bugs` → `main`
3. Tag release com versão correspondente

### ❌ Se algum teste falhar:
1. Documentar erro completo aqui
2. Investigar causa em [os/](os/) (GRUB/kernel)
3. Iterar fix em branch e re-testar
4. Nunca mergear com falha VirtualBox

---

## Referências

- **GRUB Config**: [os/grub.cfg](../os/grub.cfg)
- **Boot Script**: [os/boot.asm](../os/boot.asm)
- **Kernel Init**: [os/kernel.c](../os/kernel.c)
- **Test Automation**: [scripts/regression_all.sh](../scripts/regression_all.sh)
- **VirtualBox Docs**: https://www.virtualbox.org/manual/

---

**Última atualização**: 2026-04-24
**Autor**: Cicada Testing Team
