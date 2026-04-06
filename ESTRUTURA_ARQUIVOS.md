# 📁 ESTRUTURA DE ARQUIVOS RECOMENDADA

## Estrutura Atual vs. Proposta

### Árvore Atual
```
CICADA-3301/
├── main.py
├── README.md
├── requirements.txt
├── start_initium.sh
├── assets/
├── auth/
│   ├── __init__.py
│   └── user_manager.py
├── commands/
│   ├── __init__.py
│   ├── base_command.py
│   └── ... (24 commands)
├── ctf/
│   ├── __init__.py
│   ├── challenge_manager.py
│   ├── challenges.json
│   └── missions_engine.py
├── filesystem/
│   ├── __init__.py
│   └── virtual_fs.py
├── kernel/
│   ├── __init__.py
│   └── kernel.py
├── logs/
│   └── logger.py
├── security/
│   ├── __init__.py
│   ├── cripto_engine.py
│   ├── decoder.py
│   ├── hash_engine.py
│   └── stego_engine.py
├── system_data/
│   ├── filesystem.json
│   ├── process_manager.py
│   ├── users.json
│   └── __pycache__/
├── terminal/
│   ├── __init__.py
│   ├── input_handler.py
│   ├── parser.py
│   └── terminal.py
└── tools/
    └── embed_stego.py
```

### Estrutura Proposta (v1.0)
```
CICADA-3301/
├── main.py
├── py.typed                          ⭐ NOVO
├── README.md
├── ANALISE_COMPLETA.md              ⭐ NOVO
├── ROADMAP_DETALHADO.md             ⭐ NOVO
├── QUICK_START.md                   ⭐ NOVO
├── requirements.txt
├── setup.py                         ⭐ NOVO
├── pytest.ini                       ⭐ NOVO
├── mypy.ini                         ⭐ NOVO
├── .gitignore                       ⭐ ATUALIZAR
├── setup_tests.sh                   ⭐ NOVO
├── start_initium.sh
│
├── .github/
│   └── workflows/                   ⭐ NOVO
│       ├── tests.yml
│       ├── lint.yml
│       └── security.yml
│
├── doc/                             ⭐ NOVO
│   ├── API.md
│   ├── ARCHITECTURE.md
│   ├── CHALLENGES.md
│   ├── CONTRIBUTING.md
│   ├── DEPLOYMENT.md
│   ├── SECURITY.md
│   └── TUTORIALS.md
│
├── tests/                           ⭐ NOVO
│   ├── __init__.py
│   ├── conftest.py
│   ├── test_auth.py
│   ├── test_filesystem.py
│   ├── test_crypto.py
│   ├── test_terminal.py
│   ├── test_commands.py
│   ├── test_parser.py
│   └── fixtures/
│       ├── test_data.json
│       └── sample_image.png
│
├── config/                          ⭐ NOVO
│   ├── config.yaml
│   ├── challenges_template.json
│   └── users_template.json
│
├── assets/
│   ├── images/                      ⭐ NOVO
│   ├── fonts/                       ⭐ NOVO
│   └── ...
│
├── auth/
│   ├── __init__.py
│   ├── user_manager.py
│   └── session_manager.py           ⭐ NOVO
│
├── commands/
│   ├── __init__.py
│   ├── base_command.py
│   ├── cat.py
│   ├── cd.py
│   ├── clear.py
│   ├── decode.py
│   ├── decrypt.py
│   ├── hash.py
│   ├── help.py
│   ├── history.py
│   ├── login.py
│   ├── logout.py
│   ├── logread.py
│   ├── ls.py
│   ├── passwd.py
│   ├── ps.py
│   ├── pwd.py
│   ├── shutdown.py
│   ├── stego.py
│   ├── su.py
│   ├── submit.py
│   ├── sudo.py
│   ├── who.py
│   ├── whoami.py
│   ├── leaderboard.py               ⭐ NOVO
│   ├── hint.py                      ⭐ NOVO
│   └── score.py                     ⭐ NOVO
│
├── ctf/
│   ├── __init__.py
│   ├── challenge_manager.py
│   ├── challenges.json
│   ├── missions_engine.py
│   ├── scoring_system.py            ⭐ NOVO
│   └── hints_engine.py              ⭐ NOVO
│
├── exceptions/                      ⭐ NOVO
│   ├── __init__.py
│   └── custom_exceptions.py
│
├── filesystem/
│   ├── __init__.py
│   ├── virtual_fs.py
│   └── permissions.py               ⭐ NOVO
│
├── kernel/
│   ├── __init__.py
│   └── kernel.py
│
├── logs/
│   ├── __init__.py
│   ├── logger.py
│   └── formatters.py                ⭐ NOVO
│
├── security/
│   ├── __init__.py
│   ├── cripto_engine.py
│   ├── decoder.py
│   ├── hash_engine.py
│   ├── stego_engine.py
│   ├── aes_engine.py                ⭐ NOVO
│   ├── rsa_engine.py                ⭐ NOVO
│   └── validator.py                 ⭐ NOVO
│
├── system_data/
│   ├── __init__.py
│   ├── filesystem.json
│   ├── process_manager.py
│   ├── users.json
│   ├── scores.json                  ⭐ NOVO
│   └── config_loader.py             ⭐ NOVO
│
├── terminal/
│   ├── __init__.py
│   ├── terminal.py
│   ├── input_handler.py
│   ├── parser.py
│   ├── advanced_parser.py           ⭐ NOVO
│   ├── autocomplete.py              ⭐ NOVO
│   ├── colors.py                    ⭐ NOVO
│   └── output_formatter.py          ⭐ NOVO
│
├── tools/
│   ├── __init__.py
│   ├── embed_stego.py
│   ├── generate_challenges.py       ⭐ NOVO
│   ├── migrate_db.py                ⭐ NOVO
│   └── generate_test_data.py        ⭐ NOVO
│
├── web/                             ⭐ NOVO (opcional)
│   ├── __init__.py
│   ├── app.py
│   ├── routes.py
│   ├── templates/
│   └── static/
│
├── Dockerfile                       ⭐ NOVO
├── docker-compose.yml               ⭐ NOVO
├── .dockerignore                    ⭐ NOVO
└── .gitignore                       ⭐ ATUALIZAR
```

---

## 📝 ARQUIVOS A CRIAR POR PRIORIDADE

### FASE 1: CRÍTICA (Semana 1)

#### tests/conftest.py
```python
# Fixtures para testes
# - sample_users
# - sample_filesystem
# - sample_challenges
# Redimensão: ~80 linhas
```

#### tests/test_auth.py
```python
# Testes para UserManager
# - test_login
# - test_logout
# - test_permissions
# Redimensão: ~120 linhas
```

#### .gitignore (ATUALIZAR)
```
__pycache__/
*.pyc
*.pyo
*.egg-info/
dist/
build/
.pytest_cache/
.coverage
htmlcov/
.mypy_cache/
*.db
.env
.venv/
venv/
logs/*.log
system_data/scores.json
```

#### pytest.ini
```ini
[pytest]
testpaths = tests
python_files = test_*.py
python_classes = Test*
python_functions = test_*
addopts = -v --strict-markers
markers =
    unit: Unit tests
    integration: Integration tests
    slow: Slow tests
```

#### pyproject.toml (NOVO)
```toml
[build-system]
requires = ["setuptools>=45", "wheel", "setuptools_scm[toml]>=6.2"]
build-backend = "setuptools.build_meta"

[project]
name = "cicada-3301"
version = "0.2.0"
description = "Cybersecurity CTF Simulation Environment"
requires-python = ">=3.9"
dependencies = [
    "bcrypt==5.0.0",
    "Pillow==10.0.0",
    "pycryptodomex==3.18.0",
]

[tool.mypy]
python_version = "3.9"
warn_return_any = true
warn_unused_configs = true
ignore_missing_imports = true
```

---

### FASE 1: CRÍTICA (Semana 2-3)

#### exceptions/custom_exceptions.py
```python
# Classes de exceção custom
# Redimensão: ~50 linhas
```

#### terminal/validator.py
```python
# Validador centralizado de input
# Redimensão: ~100 linhas
```

#### tests/test_filesystem.py
```python
# Testes para VirtualFileSystem
# Redimensão: ~200 linhas
```

#### tests/test_crypto.py
```python
# Testes para módulos security
# Redimensão: ~150 linhas
```

---

### FASE 2: IMPORTANTE (Semana 4-6)

#### ctf/scoring_system.py
```python
# Sistema de pontuação
# Redimensão: ~150 linhas
```

#### ctf/hints_engine.py
```python
# Motor de dicas
# Redimensão: ~80 linhas
```

#### commands/leaderboard.py
```python
# Comando leaderboard
# Redimensão: ~50 linhas
```

#### commands/hint.py
```python
# Comando hint
# Redimensão: ~40 linhas
```

#### security/aes_engine.py
```python
# Criptografia AES
# Redimensão: ~100 linhas
```

#### doc/API.md
```markdown
# Documentação de API
# - Estrutura de dados
# - Endpoints/Commands
# - Exemplos
```

---

### FASE 3: MELHORIAS (Semana 7-10)

#### terminal/advanced_parser.py
```python
# Parser com pipes e redirecionamento
# Redimensão: ~200 linhas
```

#### terminal/autocomplete.py
```python
# Sistema de autocompletar
# Redimensão: ~100 linhas
```

#### security/rsa_engine.py
```python
# Criptografia RSA
# Redimensão: ~150 linhas
```

#### system_data/config_loader.py
```python
# Carregador de configuração
# Redimensão: ~80 linhas
```

#### config/config.yaml
```yaml
# Arquivo de configuração
# - Debug mode
# - Timeouts
# - Features flags
```

---

### FASE 4: OPCIONAL (Semana 11-20)

#### web/app.py
```python
# Flask app (OPCIONAL)
# Dashboard web
# Redimensão: ~200 linhas
```

#### Dockerfile
```dockerfile
# Containerização Docker
```

#### docker-compose.yml
```yaml
# Orquestração Docker Compose
```

#### .github/workflows/
```yaml
# CI/CD com GitHub Actions
# - tests.yml
# - lint.yml
# - security.yml
```

---

## 📊 Índice de Modificações por Arquivo

### MODIFICAR (Pequenas)
| Arquivo | Mudança | Impacto |
|---------|---------|---------|
| `requirements.txt` | Adicionar pytest, mypy | Baixo |
| `main.py` | Adicionar try-catch | Baixo |
| `.gitignore` | Adicionar padrões | Nenhum |
| `kernel/kernel.py` | Type hints | Baixo |

### MODIFICAR (Médias)
| Arquivo | Mudança | Impacto |
|---------|---------|---------|
| `terminal/terminal.py` | Exceções, logging | Médio |
| `auth/user_manager.py` | Type hints, rate limiting | Médio |
| `filesystem/virtual_fs.py` | Completar create_file, exceções | Médio |

### MODIFICAR (Grandes)
| Arquivo | Mudança | Impacto |
|---------|---------|---------|
| `ctf/challenges.json` | 10+ desafios novos | Médio |

### CRIAR (Novos)
| Arquivo | Tipo | Tamanho |
|---------|------|--------|
| `tests/` | Diretório | ~1000 linhas |
| `exceptions/` | Módulo | ~50 linhas |
| `doc/` | Documentação | ~3000 linhas |
| `config/` | Configuração | ~200 linhas |
| Vários segurança/CTF | Módulos | ~800 linhas |

---

## 🎯 ORDEM RECOMENDADA DE IMPLEMENTAÇÃO

### Dia 1-2: Setup Inicial
1. ✅ Criar `/tests` com `conftest.py`
2. ✅ Atualizar `requirements.txt`
3. ✅ Criar `pytest.ini`, `.gitignore`

### Dia 3-5: Testes Básicos
1. ✅ Implementar `test_auth.py`
2. ✅ Implementar `test_filesystem.py`
3. ✅ Implementar `test_crypto.py`

### Dia 6-7: Type Hints & Exceções
1. ✅ Criar `exceptions/custom_exceptions.py`
2. ✅ Adicionar type hints em `kernel.py`
3. ✅ Adicionar type hints em `auth/user_manager.py`

### Dia 8-10: Desafios CTF
1. ✅ Expandir `challenges.json`
2. ✅ Implementar `scoring_system.py`
3. ✅ Implementar `leaderboard.py`

### Dia 11-14: Melhorias UX
1. ✅ Implementar `advanced_parser.py`
2. ✅ Implementar `autocomplete.py`
3. ✅ Implementar `hint.py`

### Dia 15+: Extensões
1. ✅ Criptografia AES/RSA
2. ✅ Dashboard web (opcional)
3. ✅ Docker & CI/CD

---

## 📈 Estimativas de Linhas de Código

### Total Atual
| Categoria | LOC |
|-----------|-----|
| Core | 1200 |
| Commands | 1500 |
| Tests | 0 |
| Docs | 100 |
| **Total** | **2800** |

### Target (v1.0)
| Categoria | LOC |
|-----------|-----|
| Core | 2000 |
| Commands | 2000 |
| Security | 1500 |
| Tests | 3000 |
| Docs | 2000 |
| **Total** | **10500** |

### Growth: 2800 → 10500 (+275%)

---

## 🔄 Integração Contínua

### GitHub Actions Setup
```yaml
# .github/workflows/tests.yml
- Rodar pytest em cada push
- Coverage report
- Type check com mypy
- Lint com flake8

# .github/workflows/security.yml
- Bandit para segurança
- Dependency check
- Secret scanning
```

---

## 📦 Publicação em PyPI (Futuro)

```python
# setup.py
from setuptools import setup, find_packages

setup(
    name="cicada-3301",
    version="1.0.0",
    description="Cybersecurity CTF Simulation",
    author="...",
    packages=find_packages(),
    install_requires=[
        "bcrypt>=5.0",
        "Pillow>=10.0",
        "pycryptodomex>=3.18",
    ],
    extras_require={
        "dev": ["pytest", "mypy", "black"],
    },
    python_requires=">=3.9",
)
```

---

*Estrutura de arquivos versão 1.0 - 30/03/2026*
