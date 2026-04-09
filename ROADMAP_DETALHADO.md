# 🛣️ ROADMAP DETALHADO COM EXEMPLOS DE CÓDIGO

## FASE 1: CORREÇÕES CRÍTICAS (Semanas 1-4)

### 1.1 Setup de Testes com pytest

**Arquivo:** `tests/conftest.py`
```python
import pytest
import json
import tempfile
import os
from pathlib import Path

@pytest.fixture
def temp_dir():
    """Cria um diretório temporário para testes."""
    with tempfile.TemporaryDirectory() as tmpdir:
        yield tmpdir

@pytest.fixture
def sample_users():
    """Usuários de teste."""
    return {
        "guest": {
            "password": "$2b$12$zK1StL4VCDASSHlLkmDh1eZ6Ad5LVV8hA5QDM6M3CBwcO3r7.EVoG",
            "role": "user"
        },
        "admin": {
            "password": "$2b$12$8aaU22OI0akzGplx0gNe.unaQFR0AA4TKquB1KQwBfiyQHGuego8m",
            "role": "admin"
        }
    }

@pytest.fixture
def sample_filesystem():
    """Sistema de arquivos de teste."""
    return {
        "/": {
            "type": "dir",
            "owner": "root",
            "perm": "755",
            "content": {
                "home": {
                    "type": "dir",
                    "owner": "root",
                    "perm": "755",
                    "content": {
                        "guest": {
                            "type": "dir",
                            "owner": "guest",
                            "perm": "755",
                            "content": {}
                        }
                    }
                },
                "etc": {
                    "type": "dir",
                    "owner": "root",
                    "perm": "755",
                    "content": {
                        "passwd": {
                            "type": "file",
                            "owner": "root",
                            "perm": "644",
                            "content": "root:x:0:0:root:/:bin/bash"
                        }
                    }
                }
            }
        }
    }
```

**Arquivo:** `tests/test_auth.py`
```python
import pytest
from auth.user_manager import UserManager
import bcrypt

class TestUserManager:
    
    def test_login_with_correct_password(self, sample_users):
        """Testa login com senha correta."""
        manager = UserManager(sample_users)
        # Senha original do guest é "guest"
        assert manager.login("guest", "guest") == True
    
    def test_login_with_incorrect_password(self, sample_users):
        """Testa login com senha incorreta."""
        manager = UserManager(sample_users)
        assert manager.login("guest", "wrong") == False
    
    def test_login_with_nonexistent_user(self, sample_users):
        """Testa login com usuário inexistente."""
        manager = UserManager(sample_users)
        assert manager.login("nonexistent", "password") == False
    
    def test_get_current_user_after_login(self, sample_users):
        """Testa obter usuário atual após login."""
        manager = UserManager(sample_users)
        manager.login("guest", "guest")
        assert manager.get_current_user() == "guest"
    
    def test_logout(self, sample_users):
        """Testa logout."""
        manager = UserManager(sample_users)
        manager.login("guest", "guest")
        manager.logout()
        assert manager.get_current_user() == "guest"  # volta para guest
    
    def test_get_role(self, sample_users):
        """Testa obter role do usuário."""
        manager = UserManager(sample_users)
        manager.login("admin", "admin")
        assert manager.get_role() == "admin"
```

**Arquivo:** `tests/test_crypto.py`
```python
import pytest
from security.decoder import Decoder
from security.cripto_engine import CryptoEngine

class TestDecoder:
    
    def test_decode_base64(self):
        """Testa decodificação Base64."""
        decoder = Decoder()
        result = decoder.decode_base64("SGVsbG8gV29ybGQ=")
        assert result == "Hello World"
    
    def test_decode_hex(self):
        """Testa decodificação Hex."""
        decoder = Decoder()
        result = decoder.decode_hex("48656c6c6f")
        assert result == "Hello"
    
    def test_decode_binary(self):
        """Testa decodificação binária."""
        decoder = Decoder()
        result = decoder.decode_binary("01001000 01100101 01101100 01101100 01101111")
        assert result == "Hello"
    
    def test_auto_decode_base64(self):
        """Testa auto-decodificação (Base64)."""
        decoder = Decoder()
        result = decoder.auto_decode("SGVsbG8gV29ybGQ=")
        assert "[Base64]" in result
        assert "Hello World" in result

class TestCryptoEngine:
    
    def test_caesar_decrypt(self):
        """Testa decodificação Caesar."""
        engine = CryptoEngine()
        result = engine.decrypt_caesar("KHOOR", 3)
        assert result == "HELLO"
    
    def test_vigenere_decrypt(self):
        """Testa decodificação Vigenère."""
        engine = CryptoEngine()
        result = engine.decrypt_vigenere("RIJVS", "KEY")
        assert result == "HELLO"
```

**Arquivo:** `tests/test_filesystem.py`
```python
import pytest
from filesystem.virtual_fs import VirtualFileSystem

class TestVirtualFileSystem:
    
    def test_list_root_directory(self, sample_filesystem):
        """Testa listar diretório raiz."""
        vfs = VirtualFileSystem(sample_filesystem)
        items = vfs.list_dir("/", "root")
        item_names = [item[0] for item in items]
        assert "home" in item_names
        assert "etc" in item_names
    
    def test_read_file(self, sample_filesystem):
        """Testa ler arquivo."""
        vfs = VirtualFileSystem(sample_filesystem)
        content = vfs.read_file("/etc/passwd", "root")
        assert content == "root:x:0:0:root:/:bin/bash"
    
    def test_read_nonexistent_file(self, sample_filesystem):
        """Testa ler arquivo inexistente."""
        vfs = VirtualFileSystem(sample_filesystem)
        content = vfs.read_file("/nonexistent", "root")
        assert content is None
    
    def test_change_directory(self, sample_filesystem):
        """Testa mudar diretório."""
        vfs = VirtualFileSystem(sample_filesystem)
        result = vfs.change_dir("/home", "root")
        assert result == True
        assert vfs.current_path == "/home"
    
    def test_change_to_invalid_directory(self, sample_filesystem):
        """Testa mudar para diretório inválido."""
        vfs = VirtualFileSystem(sample_filesystem)
        result = vfs.change_dir("/nonexistent", "root")
        assert result == False
    
    def test_permission_denied(self, sample_filesystem):
        """Testa negação de permissão."""
        sample_filesystem["/"]["content"]["secret"] = {
            "type": "file",
            "owner": "root",
            "perm": "600",  # Apenas root pode ler
            "content": "secret data"
        }
        vfs = VirtualFileSystem(sample_filesystem)
        content = vfs.read_file("/secret", "guest")
        assert content is None
```

**Adicionar ao `requirements.txt`:**
```
pytest==7.4.0
pytest-cov==4.1.0
pytest-asyncio==0.23.0
```

**Arquivo:** `.github/workflows/tests.yml`
```yaml
name: Tests

on:
  push:
    branches: [ main, developer ]
  pull_request:
    branches: [ main, developer ]

jobs:
  test:
    runs-on: ubuntu-latest
    strategy:
      matrix:
        python-version: ['3.9', '3.10', '3.11']
    
    steps:
    - uses: actions/checkout@v3
    
    - name: Set up Python ${{ matrix.python-version }}
      uses: actions/setup-python@v4
      with:
        python-version: ${{ matrix.python-version }}
    
    - name: Install dependencies
      run: |
        python -m pip install --upgrade pip
        pip install -r requirements.txt
    
    - name: Run tests with coverage
      run: |
        pytest tests/ --cov=. --cov-report=xml
    
    - name: Upload coverage reports
      uses: codecov/codecov-action@v3
      with:
        file: ./coverage.xml
        fail_ci_if_error: true
```

---

### 1.2 Type Hints

**Arquivo:** `kernel/kernel.py` (com type hints)
```python
import json
import os
from datetime import datetime
from typing import Dict, Any, Optional
from terminal.terminal import Terminal
from filesystem.virtual_fs import VirtualFileSystem
from auth.user_manager import UserManager
from logs.logger import Logger

class Kernel:
    """Kernel principal do sistema CICADA-3301."""
    
    def __init__(self) -> None:
        """Inicializa o kernel do sistema."""
        self.system_name: str = "initium"
        self.version: str = "0.1"
        self.users: Dict[str, Dict[str, Any]] = {}
        self.filesystem: Dict[str, Any] = {}
        self.load_users()
        self.load_filesystem()
        self.vfs: VirtualFileSystem = VirtualFileSystem(self.filesystem)
        self.user_manager: UserManager = UserManager(self.users)
        self.logger: Logger = Logger()

    def boot(self) -> None:
        """Inicia o sistema."""
        self.logger.log("SYSTEM BOOT")
        terminal = Terminal(self)
        terminal.start()

    def _print_banner(self) -> None:
        """Imprime banner do sistema."""
        print("=" * 40)
        print(f"{self.system_name} v {self.version}")
        print("Cyber Security Simulation Environment")
        print("=" * 40)
        print("\n")

    def load_users(self) -> None:
        """Carrega banco de dados de usuários."""
        path: str = "system_data/users.json"
        
        try:
            if os.path.exists(path):
                with open(path) as f:
                    self.users = json.load(f)
            else:
                self.logger.log("WARN: User database not found")
        except Exception as e:
            self.logger.log(f"ERROR: Failed to load users: {str(e)}")

    def load_filesystem(self) -> None:
        """Carrega banco de dados de filesystem."""
        path: str = "system_data/filesystem.json"
        
        try:
            if os.path.exists(path):
                with open(path) as f:
                    self.filesystem = json.load(f)
            else:
                self.logger.log("WARN: Filesystem database not found")
        except Exception as e:
            self.logger.log(f"ERROR: Failed to load filesystem: {str(e)}")

    def log(self, message: str) -> None:
        """Log mensagem no arquivo de sistema."""
        try:
            with open("logs/system.log", "a") as f:
                f.write(f"{datetime.now()} : {message}\n")
        except Exception as e:
            print(f"ERROR: Failed to write log: {str(e)}")
```

---

### 1.3 Tratamento de Exceções Robusto

**Arquivo:** `exceptions.py` (novo)
```python
"""Custom exceptions para CICADA-3301."""

class CICADAException(Exception):
    """Exceção base do sistema."""
    pass

class AuthenticationError(CICADAException):
    """Erro de autenticação."""
    pass

class PermissionDeniedError(CICADAException):
    """Permissão negada."""
    pass

class FileNotFoundError(CICADAException):
    """Arquivo não encontrado."""
    pass

class InvalidCommandError(CICADAException):
    """Comando inválido."""
    pass

class CryptographyError(CICADAException):
    """Erro em operação criptográfica."""
    pass

class ValidationError(CICADAException):
    """Erro de validação."""
    pass
```

---

### 1.4 Validação de Entrada

**Arquivo:** `terminal/validator.py` (novo)
```python
"""Validadores para inputs do sistema."""

import re
from typing import List, Tuple
from exceptions import ValidationError

class InputValidator:
    """Valida inputs do usuário."""
    
    # Regex para validação de path
    VALID_PATH_PATTERN = re.compile(r"^[a-zA-Z0-9_\-./]*$")
    
    # Regex para validação de username
    VALID_USERNAME_PATTERN = re.compile(r"^[a-zA-Z0-9_]{1,32}$")
    
    # Caracteres perigosos
    DANGEROUS_CHARS = ['$', '`', '\\', '|', ';', '&', '>', '<']
    
    @staticmethod
    def validate_path(path: str) -> str:
        """Valida path de arquivo."""
        if not path:
            raise ValidationError("Path não pode estar vazio")
        
        if len(path) > 255:
            raise ValidationError("Path muito longo")
        
        if not InputValidator.VALID_PATH_PATTERN.match(path):
            raise ValidationError("Path contém caracteres inválidos")
        
        return path
    
    @staticmethod
    def validate_username(username: str) -> str:
        """Valida nome de usuário."""
        if not username:
            raise ValidationError("Username não pode estar vazio")
        
        if not InputValidator.VALID_USERNAME_PATTERN.match(username):
            raise ValidationError("Username inválido")
        
        return username
    
    @staticmethod
    def validate_password(password: str) -> str:
        """Valida senha."""
        if not password:
            raise ValidationError("Senha não pode estar vazia")
        
        if len(password) < 4:
            raise ValidationError("Senha muito curta")
        
        if len(password) > 128:
            raise ValidationError("Senha muito longa")
        
        return password
    
    @staticmethod
    def validate_command(command: str) -> str:
        """Valida comando."""
        if not command:
            raise ValidationError("Comando não pode estar vazio")
        
        for char in InputValidator.DANGEROUS_CHARS:
            if char in command:
                raise ValidationError(f"Comando contém caractere perigoso: {char}")
        
        return command.strip()
    
    @staticmethod
    def validate_and_split_args(args_str: str) -> List[str]:
        """Valida e divide argumentos."""
        try:
            # Simples split por espaço
            args = args_str.split()
            
            # Validar cada argumento
            for arg in args:
                if len(arg) > 255:
                    raise ValidationError("Argumento muito longo")
            
            return args
        except Exception as e:
            raise ValidationError(f"Erro ao processar argumentos: {str(e)}")
```

---

## FASE 2: DESAFIOS CTF (Semanas 5-8)

### 2.1 Expandir Desafios

**Arquivo:** `ctf/challenges_expanded.json` (exemplo)
```json
{
  "challenges": [
    {
      "id": 1,
      "name": "Base64 Decoding",
      "description": "Descriptografe a mensagem Base64 em message.txt",
      "category": "encoding",
      "difficulty": "easy",
      "points": 50,
      "flag": "flag{hello_cyber_ctf}",
      "hints": [
        "O arquivo está em encoding Base64",
        "Use o comando 'decode message.txt'",
        "A resposta começa com 'flag{'"
      ],
      "time_limit": 300,
      "on_start": [
        {
          "action": "create_file",
          "path": "/home/guest/message.txt",
          "content": "SGVsbG8gQ3liZXIgQ1Rm"
        }
      ],
      "on_complete": [
        {
          "action": "create_file",
          "path": "/home/guest/next.txt",
          "content": "Caesar shift 3"
        }
      ]
    },
    {
      "id": 2,
      "name": "Caesar Cipher",
      "description": "Descriptografe a mensagem Caesar (shift 3)",
      "category": "cryptography",
      "difficulty": "easy",
      "points": 75,
      "flag": "flag{caesar_cipher}",
      "hints": [
        "Use 'decrypt caesar <file> <shift>'",
        "Tente diferentes valores de shift",
        "O shift é 3"
      ],
      "time_limit": 600,
      "on_start": [
        {
          "action": "create_file",
          "path": "/home/guest/caesar.txt",
          "content": "IODXU#FLSKHU"
        }
      ],
      "on_complete": []
    },
    {
      "id": 3,
      "name": "Vigenère Cipher",
      "description": "Descriptografe usando Vigenère com chave 'SECRET'",
      "category": "cryptography",
      "difficulty": "medium",
      "points": 100,
      "flag": "flag{vigenere_key_secret}",
      "hints": [
        "Use 'decrypt vigenere <file> <key>'",
        "A chave é uma palavra comum",
        "Tente: KEY, SECRET, CIPHER"
      ],
      "time_limit": 900,
      "on_start": [
        {
          "action": "create_file",
          "path": "/home/guest/vigenere.txt",
          "content": "XOSYDZ_OYW_FIYUDX"
        }
      ],
      "on_complete": []
    },
    {
      "id": 4,
      "name": "Hidden Message",
      "description": "Extraia a mensagem escondida usando esteganografia LSB",
      "category": "steganography",
      "difficulty": "medium",
      "points": 120,
      "flag": "flag{stego_lsb_revealed}",
      "hints": [
        "Use 'stego extract <image>'",
        "LSB significa Least Significant Bit",
        "A imagem está em /assets"
      ],
      "time_limit": 1200,
      "on_start": [
        {
          "action": "copy_file",
          "source": "/root/hidden.png",
          "dest": "/home/guest/hidden.png"
        }
      ],
      "on_complete": []
    },
    {
      "id": 5,
      "name": "Hash Verification",
      "description": "Encontre a string que gera o hash SHA256 fornecido",
      "category": "cryptography",
      "difficulty": "hard",
      "points": 150,
      "flag": "flag{hash_found_success}",
      "hints": [
        "Use 'hash <text>' para gerar hashes",
        "Brute force pequenas palavras",
        "Pode ser uma palavra comum + número"
      ],
      "time_limit": 1800,
      "on_start": [
        {
          "action": "create_file",
          "path": "/home/guest/target_hash.txt",
          "content": "5e427f2ef834a80c3c95ba6f2cb93e25c96f6d5c8e63c8d6f8f8f8f8f8f8f8f"
        }
      ],
      "on_complete": []
    }
  ]
}
```

### 2.2 Sistema de Pontuação

**Arquivo:** `ctf/scoring_system.py` (novo)
```python
"""Sistema de pontuação para CTF."""

import json
from typing import Dict, List, Any
from datetime import datetime

class ScoringSystem:
    """Gerencia pontuação de usuários."""
    
    def __init__(self, scores_file: str = "system_data/scores.json"):
        """Inicializa sistema de pontuação."""
        self.scores_file = scores_file
        self.scores: Dict[str, Dict[str, Any]] = self._load_scores()
    
    def _load_scores(self) -> Dict[str, Dict[str, Any]]:
        """Carrega scores do arquivo."""
        try:
            with open(self.scores_file, 'r') as f:
                return json.load(f)
        except FileNotFoundError:
            return {}
    
    def _save_scores(self) -> None:
        """Salva scores no arquivo."""
        with open(self.scores_file, 'w') as f:
            json.dump(self.scores, f, indent=2)
    
    def add_score(self, username: str, challenge_id: int, points: int) -> None:
        """Adiciona score para um usuário em um desafio."""
        if username not in self.scores:
            self.scores[username] = {
                "total_points": 0,
                "challenges": {},
                "solved_count": 0
            }
        
        self.scores[username]["total_points"] += points
        self.scores[username]["challenges"][str(challenge_id)] = {
            "points": points,
            "timestamp": datetime.now().isoformat()
        }
        self.scores[username]["solved_count"] += 1
        
        self._save_scores()
    
    def get_leaderboard(self, top_n: int = 10) -> List[tuple]:
        """Retorna leaderboard top N."""
        sorted_users = sorted(
            self.scores.items(),
            key=lambda x: x[1]["total_points"],
            reverse=True
        )
        return sorted_users[:top_n]
    
    def get_user_stats(self, username: str) -> Dict[str, Any]:
        """Retorna estatísticas do usuário."""
        if username not in self.scores:
            return {"error": "User not found"}
        
        return self.scores[username]
    
    def print_leaderboard(self) -> None:
        """Imprime leaderboard."""
        leaderboard = self.get_leaderboard()
        
        print("\n" + "="*50)
        print(f"{'RANK':<5} {'PLAYER':<20} {'POINTS':<10} {'SOLVED':<10}")
        print("="*50)
        
        for rank, (username, data) in enumerate(leaderboard, 1):
            print(f"{rank:<5} {username:<20} {data['total_points']:<10} {data['solved_count']:<10}")
        
        print("="*50 + "\n")
```

---

## FASE 3: MELHORIAS UX (Semanas 9-12)

### 3.1 Parser Avançado

**Arquivo:** `terminal/advanced_parser.py` (novo)
```python
"""Parser avançado com suporte a pipes e redirecionamento."""

import re
from typing import List, Tuple, Optional
from exceptions import ValidationError

class Command:
    """Representa um comando parseado."""
    
    def __init__(self, name: str, args: List[str]):
        self.name = name
        self.args = args

class Pipeline:
    """Representa um pipeline de comandos."""
    
    def __init__(self, commands: List[Command]):
        self.commands = commands

class AdvancedParser:
    """Parser avançado."""
    
    # Regex para detectar pipes
    PIPE_PATTERN = re.compile(r'\s*\|\s*')
    
    # Regex para detectar redirecionamento
    REDIRECT_PATTERN = re.compile(r'\s*(?:>>?|<)\s*')
    
    @staticmethod
    def parse_line(line: str) -> Tuple[Optional[Pipeline], Optional[dict]]:
        """
        Parse uma linha de comando.
        Retorna (pipeline, redirection) ou (None, None) se inválida.
        """
        line = line.strip()
        
        if not line:
            return None, None
        
        # Verificar redirecionamento
        redirect_match = AdvancedParser.REDIRECT_PATTERN.search(line)
        redirect_info = None
        
        if redirect_match:
            # Implementar lógica de redirecionamento
            pass
        
        # Verificar pipes
        if '|' in line:
            commands = AdvancedParser._parse_pipeline(line)
            return Pipeline(commands), redirect_info
        else:
            # Comando simples
            cmd = AdvancedParser._parse_command(line)
            if cmd:
                return Pipeline([cmd]), redirect_info
        
        return None, None
    
    @staticmethod
    def _parse_command(cmd_str: str) -> Optional[Command]:
        """Parse um comando simples."""
        parts = cmd_str.split()
        
        if not parts:
            return None
        
        return Command(parts[0], parts[1:])
    
    @staticmethod
    def _parse_pipeline(line: str) -> List[Command]:
        """Parse um pipeline de comandos."""
        commands = []
        parts = AdvancedParser.PIPE_PATTERN.split(line)
        
        for part in parts:
            cmd = AdvancedParser._parse_command(part)
            if cmd:
                commands.append(cmd)
        
        return commands
```

### 3.2 Autocompletar

**Arquivo:** `terminal/autocomplete.py` (novo)
```python
"""Sistema de autocompletar."""

from typing import List, Optional

class Autocompleter:
    """Fornece sugestões de autocompletar."""
    
    def __init__(self, terminal):
        self.terminal = terminal
        self.commands = list(terminal.commands.keys())
    
    def get_suggestions(self, text: str) -> List[str]:
        """Retorna sugestões baseado no texto."""
        if not text:
            return self.commands
        
        # Autocompletar de comandos
        if ' ' not in text:
            return [cmd for cmd in self.commands if cmd.startswith(text)]
        
        # Autocompletar de arquivos
        parts = text.split()
        if len(parts) > 1:
            query = parts[-1]
            files = self._get_file_suggestions(query)
            return files
        
        return []
    
    def _get_file_suggestions(self, path: str) -> List[str]:
        """Retorna sugestões de arquivos."""
        try:
            items = self.terminal.vfs.list_dir(path, self.terminal.auth.get_current_user())
            return [item[0] for item in items if item[0].startswith(path.split('/')[-1])]
        except:
            return []
```

---

## FASE 4: CRIPTOGRAFIA AVANÇADA (Semanas 13-16)

### 4.1 Adicionar AES

**Arquivo:** `security/aes_engine.py` (novo)
```python
"""Motor AES para criptografia simétrica."""

from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes
from Crypto.Protocol.KDF import PBKDF2
import base64
from typing import Tuple

class AESEngine:
    """Encrição/descrição com AES."""
    
    def __init__(self, password: str):
        """Inicializa com senha."""
        self.password = password
    
    def _derive_key(self, salt: bytes) -> bytes:
        """Deriva chave da senha."""
        return PBKDF2(self.password, salt, dkLen=32, count=100000)
    
    def encrypt(self, plaintext: str) -> str:
        """Cripta texto."""
        salt = get_random_bytes(16)
        key = self._derive_key(salt)
        
        cipher = AES.new(key, AES.MODE_GCM)
        ciphertext, tag = cipher.encrypt_and_digest(plaintext.encode())
        
        # Retorna salt + nonce + tag + ciphertext em base64
        encrypted_data = salt + cipher.nonce + tag + ciphertext
        return base64.b64encode(encrypted_data).decode()
    
    def decrypt(self, encrypted_data: str) -> str:
        """Descriptografa texto."""
        try:
            encrypted_bytes = base64.b64decode(encrypted_data)
            
            salt = encrypted_bytes[:16]
            nonce = encrypted_bytes[16:32]
            tag = encrypted_bytes[32:48]
            ciphertext = encrypted_bytes[48:]
            
            key = self._derive_key(salt)
            cipher = AES.new(key, AES.MODE_GCM, nonce=nonce)
            
            plaintext = cipher.decrypt_and_verify(ciphertext, tag)
            return plaintext.decode()
        except Exception as e:
            raise ValueError(f"Decryption failed: {str(e)}")
```

**Adicionar ao requirements.txt:**
```
pycryptodomex==3.18.0
```

---

## FASE 5: DOCKERIZAÇÃO E DEPLOYMENT

### 5.1 Dockerfile

**Arquivo:** `Dockerfile`
```dockerfile
FROM python:3.11-slim

WORKDIR /app

# Instalar dependências do sistema
RUN apt-get update && apt-get install -y \
    git \
    && rm -rf /var/lib/apt/lists/*

# Copiar requirements
COPY requirements.txt .

# Instalar dependências Python
RUN pip install --no-cache-dir -r requirements.txt

# Copiar código
COPY . .

# Criar volumes para dados persistentes
VOLUME ["/app/system_data", "/app/logs"]

# Executar
CMD ["python", "main.py"]
```

### 5.2 docker-compose.yml

```yaml
version: '3.8'

services:
  cicada:
    build: .
    container_name: cicada-3301
    volumes:
      - ./system_data:/app/system_data
      - ./logs:/app/logs
      - ./ctf:/app/ctf
    environment:
      - PYTHONUNBUFFERED=1
      - DEBUG=false
    stdin_open: true
    tty: true
    restart: unless-stopped
```

---

## MÉTRICAS DE PROGRESSO

### Checklist de Implementação

**Fase 1 (Semanas 1-4):**
- [ ] Testes com pytest (50% cobertura)
- [ ] Type hints em 3 módulos principais
- [ ] Tratamento de exceções robusto
- [ ] Validador de entrada centralizado

**Fase 2 (Semanas 5-8):**
- [ ] 10+ desafios CTF implementados
- [ ] Sistema de pontuação funcional
- [ ] Leaderboard visual
- [ ] Hints por desafio

**Fase 3 (Semanas 9-12):**
- [ ] Parser com pipes funcionando
- [ ] Redirecionamento de output
- [ ] Autocompletar básico
- [ ] Histórico persistente

**Fase 4 (Semanas 13-16):**
- [ ] AES implementado
- [ ] RSA implementado
- [ ] Dashboard web (opcional)
- [ ] Suporte a plugins (opcional)

**Fase 5 (Semanas 17-20):**
- [ ] Dockerfile funcional
- [ ] docker-compose.yml
- [ ] CI/CD completo
- [ ] Documentação de deployment

---

## ESTIMATIVAS DE ESFORÇO

| Tarefa | Horas | Prioridade |
|--------|-------|-----------|
| Testes básicos | 40 | ⭐⭐⭐⭐⭐ |
| Type hints | 30 | ⭐⭐⭐⭐ |
| Tratamento exceções | 25 | ⭐⭐⭐⭐ |
| Desafios CTF (10) | 60 | ⭐⭐⭐⭐ |
| Sistema pontuação | 20 | ⭐⭐⭐ |
| Parser avançado | 50 | ⭐⭐⭐ |
| AES + RSA | 40 | ⭐⭐⭐ |
| Dashboard web | 80 | ⭐⭐ |
| Docker + CI/CD | 30 | ⭐⭐⭐ |
| Documentação | 40 | ⭐⭐⭐ |

**Total estimado: 415 horas (~10 semanas em tempo integral)**

---

## RECURSOS RECOMENDADOS

### Bibliotecas Python
- `pytest` - Testes
- `pycryptodomex` - Criptografia avançada
- `sqlalchemy` - ORM para banco de dados
- `flask` - Web framework (opcional)
- `mypy` - Type checking

### Ferramentas
- GitHub Actions - CI/CD
- Codecov - Cobertura de testes
- Docker - Containerização
- PostgreSQL - Banco de dados (opcional)

---

*Roadmap versão 1.0 - 30/03/2026*
