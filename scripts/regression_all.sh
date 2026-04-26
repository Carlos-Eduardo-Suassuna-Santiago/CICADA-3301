#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

PYTHON_BIN="${PYTHON_BIN:-$ROOT_DIR/.venv/bin/python}"

if [[ ! -x "$PYTHON_BIN" ]]; then
    echo "[FAIL] Python do ambiente virtual nao encontrado: $PYTHON_BIN"
    echo "       Configure o ambiente e dependencias antes de rodar a regressao."
    exit 1
fi

echo "[1/3] Validando desafios 1-7 da simulacao Python"
"$PYTHON_BIN" - <<'PY'
import io
import time
import contextlib

from kernel.kernel import Kernel
from terminal.terminal import Terminal

k = Kernel()
t = Terminal(k)

if not t.auth.login("guest", "guest"):
    raise SystemExit("[FAIL] login guest/guest falhou")

t.auth.set_current_user("guest")
t.vfs.current_path = "/home/guest"
t.session_start = time.time()

def run(command: str) -> str:
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf):
        t.execute(command)
    return buf.getvalue()

checks = [
    ("decode message.txt", "Hello Investigator"),
    ("submit flag{hello_investigator}", "Flag correct"),
    ("submit flag{caesar_clue}", "Flag correct"),
    ("submit flag{password123}", "Flag correct"),
    ("submit flag{check_the_logs}", "Flag correct"),
    ("submit flag{log_analysis_success}", "Flag correct"),
    ("submit flag{root_access_granted}", "Flag correct"),
    ("submit flag{welcome_to_the_inner_circle}", "PARAB"),
]

# Comandos auxiliares para progredir os desafios corretamente.
setup_cmds = [
    "decrypt letter.txt",
    "hash analyze password.hash",
    "hash generate password123",
    "stego extract image.png",
    "analyze system.log",
    "sudo exploit backup.sh",
    "decode /root/final.key",
]

for cmd, expected in checks:
    if cmd == "submit flag{caesar_clue}":
        run(setup_cmds[0])
    elif cmd == "submit flag{password123}":
        run(setup_cmds[1]); run(setup_cmds[2])
    elif cmd == "submit flag{check_the_logs}":
        run(setup_cmds[3])
    elif cmd == "submit flag{log_analysis_success}":
        run(setup_cmds[4])
    elif cmd == "submit flag{root_access_granted}":
        run(setup_cmds[5])
    elif cmd == "submit flag{welcome_to_the_inner_circle}":
        run(setup_cmds[6])

    out = run(cmd)
    if expected not in out:
        raise SystemExit(f"[FAIL] '{cmd}' nao retornou marcador esperado: {expected}\nSaida:\n{out}")

print("[PASS] Fluxo oficial dos desafios 1-7 validado")
PY

echo "[2/3] Validando boot smoke do kernel OS"
cd "$ROOT_DIR/os"
make boot-smoke

echo "[3/3] Regressao completa"
echo "[PASS] Python CTF + Boot kernel OS validados"
