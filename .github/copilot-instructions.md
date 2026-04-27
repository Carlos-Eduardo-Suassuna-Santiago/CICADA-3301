# Project Guidelines

## Build And Test
- Ambiente Python:
  - Crie/ative venv e instale dependencias com `pip install -r requirements.txt`.
  - Execute o app principal com `python main.py`.
- Fluxo principal de validacao:
  - `make regression` para validar desafios CTF (script: `scripts/regression_all.sh`).
- Subprojeto de SO (`os/`):
  - `make os-build` para gerar `os/INITIUM.iso`.
  - `make os-smoke` para smoke test de boot.
  - `make clean` para limpeza de artefatos.
- Teste VirtualBox quando alteracoes envolverem boot/kernel:
  - `make vbox-setup`, `make vbox-test`, `make vbox-clean`.

## Architecture
- O projeto tem duas frentes:
  - Simulador CTF em Python no diretorio raiz.
  - Kernel bootavel em C/ASM no diretorio `os/`.
- No simulador Python, respeite os limites de responsabilidade:
  - `kernel/`: orquestracao de boot e inicializacao.
  - `terminal/`: loop interativo, parser e despacho de comandos.
  - `commands/`: implementacoes de comandos (padrao Command).
  - `auth/`, `filesystem/`, `ctf/`, `security/`: servicos de dominio.
  - `system_data/` e `ctf/challenges.json`: persistencia/config baseada em JSON.

## Conventions
- Novo comando deve seguir o padrao existente em `commands/`:
  - Herdar de `BaseCommand`.
  - Definir `name`, `description`, `usage`.
  - Implementar `execute(terminal, args)` sem quebrar a interface atual.
- Preserve compatibilidade com dados JSON existentes (`system_data/*.json`, `ctf/challenges.json`).
- Evite refatoracoes amplas sem necessidade: priorize mudancas pequenas e localizadas.
- Ao mexer no subprojeto `os/`, mantenha toolchain 32-bit e fluxo de build atual do `os/Makefile`.

## Project Pitfalls
- Nao ha suite formal de testes unitarios; `make regression` e a principal validacao automatizada.
- O estado e bastante compartilhado via objeto `terminal` e componentes do `kernel`; cuidado com efeitos colaterais.
- Varias documentacoes estao em portugues; mantenha consistencia de linguagem em textos de suporte.

## Linked Docs
- Visao geral e contexto: [README.md](../README.md)
- Arquitetura detalhada: [ANALISE_COMPLETA.md](../ANALISE_COMPLETA.md)
- Guia de uso/resolucao CTF: [GUIA_RESOLUCAO.md](../GUIA_RESOLUCAO.md)
- Roadmap e melhorias: [QUICK_START.md](../QUICK_START.md), [ROADMAP_DETALHADO.md](../ROADMAP_DETALHADO.md)
- Fluxo de teste em VirtualBox: [docs/VIRTUALBOX_TESTING.md](../docs/VIRTUALBOX_TESTING.md), [VIRTUALBOX_QUICK_START.md](../VIRTUALBOX_QUICK_START.md)
- Subprojeto de SO: [os/README.md](../os/README.md)
