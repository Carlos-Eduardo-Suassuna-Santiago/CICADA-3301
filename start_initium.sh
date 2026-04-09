#!/bin/bash

# ==============================
# CONFIG
# ==============================
PROJECT_DIR="/home/$USER/CICADA-3301"
BRANCH="developer"

# log
exec >> /home/$USER/initium.log 2>&1

echo "================================="
echo "INITIUM START: $(date)"
echo "================================="

cd "$PROJECT_DIR" || exit 1

# ==============================
# ATUALIZAR REPOSITÓRIO
# ==============================
echo "[+] Atualizando repositório..."

git fetch origin

LOCAL=$(git rev-parse HEAD)
REMOTE=$(git rev-parse origin/$BRANCH)

if [ "$LOCAL" != "$REMOTE" ]; then
    echo "[+] Atualização encontrada..."
    git pull origin $BRANCH
else
    echo "[+] Já atualizado"
fi

# ==============================
# AMBIENTE PYTHON
# ==============================
if [ -d ".venv" ]; then
    echo "[+] Ativando venv..."
    source .venv/bin/activate
fi

# ==============================
# LOOP DE EXECUÇÃO
# ==============================
echo "[+] Iniciando sistema..."

while true; do
    python3 main.py
    echo "[!] Reiniciando em 2s..."
    sleep 2
done