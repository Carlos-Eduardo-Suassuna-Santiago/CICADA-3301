#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

LOG_FILE="${LOG_FILE:-/tmp/initium_boot_smoke_serial.log}"
QEMU_STDOUT_LOG="${QEMU_STDOUT_LOG:-/tmp/initium_boot_smoke_qemu_stdout.log}"
BOOT_TIMEOUT="${BOOT_TIMEOUT:-20}"
QEMU_MEM_MB="${QEMU_MEM_MB:-256}"

for cmd in qemu-system-i386 grub-file timeout; do
    if ! command -v "$cmd" >/dev/null 2>&1; then
        echo "[FAIL] Dependencia ausente: $cmd"
        exit 1
    fi
done

echo "[1/4] Build da ISO"
make clean all >/tmp/initium_boot_smoke_build.log 2>&1
echo "      Build concluido"

echo "[2/4] Verificacao Multiboot"
if grub-file --is-x86-multiboot kernel.bin; then
    echo "      kernel.bin reconhecido como Multiboot"
else
    echo "[FAIL] kernel.bin nao e Multiboot valido"
    exit 1
fi

echo "[3/4] Boot smoke no QEMU (${BOOT_TIMEOUT}s)"
set +e
timeout "${BOOT_TIMEOUT}s" qemu-system-i386 \
    -cdrom INITIUM.iso \
    -boot d \
    -m "$QEMU_MEM_MB" \
    -display none \
    -serial "file:$LOG_FILE" \
    -monitor none \
    >"$QEMU_STDOUT_LOG" 2>&1
QEMU_EXIT=$?
set -e

echo "[4/4] Validacao de runtime"
HAS_BOOT_MARKER=0
HAS_LOGIN_MARKER=0

if grep -q "BOOT:7 shell start" "$LOG_FILE"; then
    HAS_BOOT_MARKER=1
fi
if grep -q "=== LOGIN ===" "$LOG_FILE"; then
    HAS_LOGIN_MARKER=1
fi

if [[ "$HAS_BOOT_MARKER" -eq 1 && "$HAS_LOGIN_MARKER" -eq 1 ]]; then
    echo "[PASS] Boot validado: shell iniciado e tela de login detectada"
    echo "       Log: $LOG_FILE"
    exit 0
fi

echo "[FAIL] Nao foi possivel comprovar boot/login no tempo limite"
echo "       Exit QEMU/timeout: $QEMU_EXIT"
echo "       Ultimas linhas do log:"
tail -n 80 "$LOG_FILE" || true
exit 1
