#!/bin/bash
# virtualbox_test_setup.sh - Configurar VM de teste no VirtualBox para CICADA-3301

set -e

# Cores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configurações
VM_NAME="${1:-CICADA-3301-TEST}"
ISO_PATH="${2:-./os/INITIUM.iso}"
MEMORY_MB="512"
CPUS="2"

echo -e "${BLUE}╔════════════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║  CICADA-3301 VirtualBox Test Setup                         ║${NC}"
echo -e "${BLUE}╚════════════════════════════════════════════════════════════╝${NC}"

# ==============================================================================
# FASE 1: Verificação de Pré-requisitos
# ==============================================================================

echo -e "\n${YELLOW}[1/4] Verificando pré-requisitos...${NC}"

# Verificar VirtualBox
if ! command -v VBoxManage &> /dev/null; then
    echo -e "${RED}✗ VirtualBox não instalado${NC}"
    echo "  Instalar: sudo apt install virtualbox"
    exit 1
fi
echo -e "${GREEN}✓ VirtualBox encontrado${NC}"

# Verificar ISO
if [ ! -f "$ISO_PATH" ]; then
    echo -e "${YELLOW}⚠ ISO não encontrada em $ISO_PATH${NC}"
    echo "  Reconstruindo..."
    if [ -d "os" ]; then
        cd os
        make clean all
        cd ..
        ISO_PATH="./os/INITIUM.iso"
    else
        echo -e "${RED}✗ Não consegui reconstruir ISO${NC}"
        exit 1
    fi
fi
echo -e "${GREEN}✓ ISO encontrada: $ISO_PATH${NC}"

# ==============================================================================
# FASE 2: Verificar se VM já existe
# ==============================================================================

echo -e "\n${YELLOW}[2/4] Configurando VM...${NC}"

if VBoxManage showvminfo "$VM_NAME" &> /dev/null; then
    echo -e "${YELLOW}⚠ VM '$VM_NAME' já existe${NC}"
    read -p "  Deletar e recriar? (y/n): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        echo "  Deletando VM antiga..."
        VBoxManage unregistervm "$VM_NAME" --delete 2>/dev/null || true
    else
        echo "  Pulando criação. Use nome diferente com: $0 NOVO_NOME"
        exit 0
    fi
fi

# ==============================================================================
# FASE 3: Criar VM
# ==============================================================================

echo "  Criando nova VM..."
VBoxManage createvm \
    --name "$VM_NAME" \
    --ostype Linux26_64 \
    --register \
    --basefolder "$HOME/VirtualBox VMs" \
    > /dev/null 2>&1

echo "  Configurando hardware..."
VBoxManage modifyvm "$VM_NAME" \
    --memory "$MEMORY_MB" \
    --cpus "$CPUS" \
    --vram 12 \
    --chipset piix3 \
    --boot1 dvd \
    --boot2 disk \
    --boot3 none \
    --boot4 none \
    --nic1 nat \
    --nictype1 virtio \
    --uart1 0x3F8 115200 \
    > /dev/null 2>&1

echo "  Configurando storage..."
# Criar disco virtual
DISK_PATH="$HOME/VirtualBox VMs/$VM_NAME/disk.vdi"
mkdir -p "$(dirname "$DISK_PATH")"
VBoxManage createhd \
    --filename "$DISK_PATH" \
    --size 10240 \
    > /dev/null 2>&1

# Adicionar controller SATA
VBoxManage storagectl "$VM_NAME" \
    --name "SATA Controller" \
    --add sata \
    --controller IntelAhci \
    > /dev/null 2>&1

# Anexar disco
VBoxManage storageattach "$VM_NAME" \
    --storagectl "SATA Controller" \
    --port 0 \
    --device 0 \
    --type hdd \
    --medium "$DISK_PATH" \
    > /dev/null 2>&1

# Anexar ISO
ISO_ABSOLUTE=$(cd "$(dirname "$ISO_PATH")" && pwd)/$(basename "$ISO_PATH")
VBoxManage storagectl "$VM_NAME" \
    --name "IDE Controller" \
    --add ide \
    > /dev/null 2>&1 || true

VBoxManage storageattach "$VM_NAME" \
    --storagectl "IDE Controller" \
    --port 0 \
    --device 0 \
    --type dvddrive \
    --medium "$ISO_ABSOLUTE" \
    > /dev/null 2>&1 || VBoxManage storageattach "$VM_NAME" \
    --storagectl "SATA Controller" \
    --port 1 \
    --device 0 \
    --type dvddrive \
    --medium "$ISO_ABSOLUTE" \
    > /dev/null 2>&1

echo -e "${GREEN}✓ VM criada com sucesso${NC}"

# ==============================================================================
# FASE 4: Configuração de Debug (Serial)
# ==============================================================================

echo -e "\n${YELLOW}[3/4] Configurando saída serial para debug...${NC}"

SERIAL_LOG="/tmp/cicada_virtualbox_${VM_NAME}.log"

# Configurar serial para arquivo
VBoxManage modifyvm "$VM_NAME" \
    --uartmode1 server \
    > /dev/null 2>&1 || VBoxManage modifyvm "$VM_NAME" \
    --uartmode1 file \
    --uartpath1 "$SERIAL_LOG" \
    > /dev/null 2>&1

echo -e "${GREEN}✓ Serial log: $SERIAL_LOG${NC}"

# ==============================================================================
# FASE 5: Informações Finais
# ==============================================================================

echo -e "\n${YELLOW}[4/4] Resumo da Configuração${NC}"

cat << EOF

${BLUE}VM Criada: ${GREEN}$VM_NAME${NC}

${BLUE}Especificações:${NC}
  • Memória: ${GREEN}${MEMORY_MB} MB${NC}
  • CPUs: ${GREEN}${CPUS}${NC}
  • Chipset: ${GREEN}PIIX3 (BIOS Legacy)${NC}
  • Boot: ${GREEN}DVD → Disco${NC}
  • ISO: ${GREEN}$ISO_ABSOLUTE${NC}
  • Serial Log: ${GREEN}$SERIAL_LOG${NC}

${BLUE}Próximos Passos:${NC}
  1. Iniciar VM:
     ${GREEN}VBoxManage startvm "$VM_NAME"${NC}

  2. Monitorar tela (em outro terminal):
     ${GREEN}watch -n 1 "tail -20 $SERIAL_LOG" 2>/dev/null || true${NC}

  3. Testar checklist em: docs/VIRTUALBOX_TESTING.md

${BLUE}Para Deletar VM Later:${NC}
  ${GREEN}VBoxManage unregistervm "$VM_NAME" --delete${NC}

${BLUE}Para Capturar Screenshot:${NC}
  ${GREEN}VBoxManage controlvm "$VM_NAME" screenshotpng /tmp/vm_screenshot.png${NC}

EOF

echo -e "\n${GREEN}✓ Setup concluído!${NC}\n"
