#!/bin/bash
# virtualbox_test_validate.sh - Validar testes emVM VirtualBox

set -e

VM_NAME="${1:-CICADA-3301-TEST}"
TIMEOUT="${2:-60}"

# Cores
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${BLUE}╔════════════════════════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║  CICADA-3301 VirtualBox Validation                         ║${NC}"
echo -e "${BLUE}╚════════════════════════════════════════════════════════════╝${NC}"

# ==============================================================================
# Verificação básica
# ==============================================================================

if ! command -v VBoxManage &> /dev/null; then
    echo -e "${RED}✗ VirtualBox não instalado${NC}"
    exit 1
fi

if ! VBoxManage showvminfo "$VM_NAME" &> /dev/null; then
    echo -e "${RED}✗ VM '$VM_NAME' não encontrada${NC}"
    echo -e "    Use: ./scripts/virtualbox_test_setup.sh $VM_NAME"
    exit 1
fi

# ==============================================================================
# Checar Status
# ==============================================================================

echo -e "\n${YELLOW}Checando status da VM...${NC}"

VM_STATE=$(VBoxManage showvminfo "$VM_NAME" | grep "^State:" | awk '{print $NF}')
echo "  Estado: $VM_STATE"

if [ "$VM_STATE" != "running" ]; then
    echo -e "${RED}✗ VM não está rodando${NC}"
    echo -e "    Inicie com: ${GREEN}VBoxManage startvm $VM_NAME${NC}"
    exit 1
fi

echo -e "${GREEN}✓ VM está rodando${NC}"

# ==============================================================================
# Coletar Serial Output
# ==============================================================================

echo -e "\n${YELLOW}Coletando saída serial (${TIMEOUT}s)...${NC}"

SERIAL_LOG="/tmp/cicada_virtualbox_${VM_NAME}.log"
BOOT_LOG="/tmp/cicada_boot_validation_${VM_NAME}.log"

# Esperar output aparecer
sleep 3

if [ ! -f "$SERIAL_LOG" ]; then
    echo -e "${YELLOW}⚠ Log serial ainda não criado. Aguardando...${NC}"
    sleep 5
fi

# Coletar durante timeout
START_SIZE=0
for i in $(seq 1 $TIMEOUT); do
    if [ -f "$SERIAL_LOG" ]; then
        SIZE=$(wc -c < "$SERIAL_LOG")
        if [ "$SIZE" -gt "$START_SIZE" ]; then
            echo -n "."
            START_SIZE=$SIZE
        fi
    fi
    sleep 1
done
echo ""

# ==============================================================================
# Validação
# ==============================================================================

echo -e "\n${YELLOW}Validando boot markers...${NC}"

if [ ! -f "$SERIAL_LOG" ]; then
    echo -e "${YELLOW}⚠ Nenhum log serial coletado${NC}"
    echo "  Checar: Saída serial pode estar em outro lugar"
    exit 1
fi

# Copiar para backup
cp "$SERIAL_LOG" "$BOOT_LOG"

# Verificar markers
MARKERS=(
    "BOOT:0"
    "BOOT:7"
    "LOGIN"
)

PASS=0
FAIL=0

for marker in "${MARKERS[@]}"; do
    if grep -q "$marker" "$BOOT_LOG"; then
        echo -e "  ${GREEN}✓ Encontrado: $marker${NC}"
        ((PASS++))
    else
        echo -e "  ${RED}✗ Não encontrado: $marker${NC}"
        ((FAIL++))
    fi
done

# ==============================================================================
# Relatório Final
# ==============================================================================

echo -e "\n${BLUE}Relatório de Validação:${NC}"

cat << EOF

${BLUE}Markers Validados:${NC}
  Sucesso: ${GREEN}$PASS${NC}
  Falha: ${RED}$FAIL${NC}

${BLUE}Log Completo:${NC}
  $BOOT_LOG

${BLUE}Últimas 30 linhas:${NC}
EOF

echo "---"
tail -30 "$BOOT_LOG"
echo "---"

if [ "$FAIL" -eq 0 ] && [ "$PASS" -gt 0 ]; then
    echo -e "\n${GREEN}✓ VALIDAÇÃO PASSOU${NC}"
    exit 0
else
    echo -e "\n${RED}✗ VALIDAÇÃO FALHOU${NC}"
    exit 1
fi
