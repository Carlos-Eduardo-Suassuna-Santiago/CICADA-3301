# CICADA-3301: Guia Completo de Resolução dos Desafios

## 📋 Índice
1. [Introdução](#introdução)
2. [Estrutura do Sistema](#estrutura-do-sistema)
3. [Desafio 1: Mensagem Codificada](#desafio-1-mensagem-codificada)
4. [Desafio 2: Cifra de César](#desafio-2-cifra-de-césar)
5. [Desafio 3: Hash Misterioso](#desafio-3-hash-misterioso)
6. [Desafio 4: Arquivo Oculto](#desafio-4-arquivo-oculto)
7. [Desafio 5: Análise de Logs](#desafio-5-análise-de-logs)
8. [Desafio 6: Escalonamento de Privilégios](#desafio-6-escalonamento-de-privilégios)
9. [Desafio 7: A Camada Final](#desafio-7-a-camada-final)
10. [Resumo de Flags](#resumo-de-flags)

---

## Introdução

**CICADA-3301** é um simulador de segurança cibernética interativo que oferece 7 desafios progressivos de criptografia, esteganografia e análise forense. O sistema desbloqueia novos comandos conforme você avança através dos níveis, criando uma experiência educacional estruturada.

### Informações Gerais

| Aspecto | Detalhes |
|--------|----------|
| **Total de Desafios** | 7 (Progressivos) |
| **Pontos Totais** | 775 pontos |
| **Usuário Padrão** | guest / guest |
| **Sistema** | initium v 0.1 |
| **Tempo Limite Total** | ~2 horas |

### Como Começar

```bash
# Iniciar o sistema
python main.py

# Login
Username: guest
Password: guest

# Visualizar ajuda
help

# Resolver desafios
submit flag{sua_flag_aqui}
```

---

## Estrutura do Sistema

### Desbloqueia Progressiva de Comandos

```
Nível 1  → 11 comandos
Nível 2  → +decrypt (12 total)
Nível 3  → +hash (13 total)
Nível 4  → +stego (14 total)
Nível 5  → +analyze (15 total)
Nível 6  → +sudo, +exploit (17 total)
Nível 7  → Todos disponíveis (17 total)
```

### Comandos Base Disponíveis em Todos os Níveis

- `ls` - Listar diretórios
- `cd` - Mudar diretório
- `pwd` - Mostrar diretório atual
- `cat` - Exibir conteúdo de arquivo
- `clear` - Limpar tela
- `help` - Ajuda (mostra nível atual e comandos)
- `history` - Histórico de comandos
- `whoami` - Mostrar usuário atual
- `submit` - Enviar flag do desafio
- `manual` - Manual de todos os comandos

---

## Desafio 1: Mensagem Codificada

### 📊 Informações

| Propriedade | Valor |
|------------|-------|
| **ID** | 1 |
| **Categoria** | Encoding |
| **Dificuldade** | 🟢 Fácil |
| **Pontos** | 50 |
| **Tempo Limite** | 5 minutos |
| **Comandos Disponíveis** | 11 |
| **Novo Comando** | N/A |

### 🎯 Objetivo

Decodificar um texto em Base64 para encontrar a flag e uma pista para o próximo desafio.

### 📝 Passo a Passo

**Passo 1:** Listar arquivos no diretório home
```bash
ls
```
Você verá: `message.txt`

**Passo 2:** Visualizar conteúdo do arquivo
```bash
cat message.txt
```
Resultado: `SGVsbG8gSW52ZXN0aWdhdG9y`

**Passo 3:** Decodificar usando Base64
```bash
decode message.txt
```
Resultado: `[Base64] Hello Investigator`

**Passo 4:** Submeter a flag
```bash
submit flag{hello_investigator}
```

### ✅ Flag Esperada

```
flag{hello_investigator}
```

### 💡 Dicas

1. O comando `decode` detecta automaticamente o tipo de encriptação (Base64, Hex, Binary)
2. Base64 é um esquema de codificação comum para transmitir dados em formato texto
3. O resultado contém o prefixo `[Base64]` indicando o tipo detectado

### 📚 Conceitos Ensinados

- **Base64**: Sistema de codificação que converte dados binários em caracteres ASCII
- **Decodificação**: Processo de converter dados codificados de volta ao formato original
- **Uso básico de comandos**: `ls`, `cat`, `submit`

### 🔓 Próximo Desafio Desbloqueado

- ✅ `decrypt` comando adicionado
- 📄 Arquivo `letter.txt` criado no diretório home

---

## Desafio 2: Cifra de César

### 📊 Informações

| Propriedade | Valor |
|------------|-------|
| **ID** | 2 |
| **Categoria** | Cryptography |
| **Dificuldade** | 🟢 Fácil |
| **Pontos** | 75 |
| **Tempo Limite** | 5 minutos |
| **Comandos Disponíveis** | 12 |
| **Novo Comando** | `decrypt` |

### 🎯 Objetivo

Descriptografar uma mensagem cifrada com Cifra de César para encontrar a próxima pista.

### 📝 Passo a Passo

**Passo 1:** Listar arquivos
```bash
ls
```
Você verá: `letter.txt`

**Passo 2:** Visualizar conteúdo
```bash
cat letter.txt
```
Resultado: `Brx pdB irxqg wkh qhaw foxh`

**Passo 3:** Usar decrypt para tentar descobrir automaticamente
```bash
decrypt letter.txt
```
O sistema tela detectar e sugerir o deslocamento correto!

**Alternativa - Descriptografar com shift específico:**
```bash
decrypt caesar letter.txt 3
```

**Passo 4:** Submeter a flag
```bash
submit flag{caesar_clue}
```

### ✅ Flag Esperada

```
flag{caesar_clue}
```

### 💡 Dicas

1. **Cifra de César** desloca cada letra por um número fixo de posições
2. O deslocamento testado foi **3** posições (A→D, B→E, etc)
3. O comando `decrypt` tenta deslocamentos automaticamente
4. Resultado esperado: "You may found the next clue"

### 📚 Conceitos Ensinados

- **Cifra de César**: Criptografia clássica que usa deslocamento de letras
- **Análise de frequência**: Como detectar texto legível
- **Criptografia simétrica**: Mesma operação descriptografa

### 🔓 Próximo Desafio Desbloqueado

- ✅ `hash` comando adicionado
- 📄 Arquivo `password.hash` criado

---

## Desafio 3: Hash Misterioso

### 📊 Informações

| Propriedade | Valor |
|------------|-------|
| **ID** | 3 |
| **Categoria** | Hash |
| **Dificuldade** | 🟡 Médio |
| **Pontos** | 100 |
| **Tempo Limite** | 10 minutos |
| **Comandos Disponíveis** | 13 |
| **Novo Comando** | `hash` |

### 🎯 Objetivo

Identificar o algoritmo de hash, depois encontrar a senha original que gera aquele hash.

### 📝 Passo a Passo

**Passo 1:** Listar e exibir arquivo
```bash
cat password.hash
```
Resultado: `ef92b778bafe771e89245b89ecbc7502fccc466aa2885b78f93d8db23b3f95d8`

**Passo 2:** Analisar tipo de hash
```bash
hash analyze password.hash
```
Resultado: `Identified hash type: SHA256` (64 caracteres hex = SHA256)

**Passo 3:** Testar senhas comuns usando hash generate
```bash
hash generate password123
```
Isso gerará: `ef92b778bafe771e89245b89ecbc7502fccc466aa2885b78f93d8db23b3f95d8`

**Passo 4:** Se encontrar correspondência, submeter
```bash
submit flag{password123}
```

### ✅ Flag Esperada

```
flag{password123}
```

### 💡 Dicas

1. **SHA256** produz sempre hashes de **64 caracteres hexadecimais**
2. Use senhas comuns: password, admin, secret, 123456, etc
3. Tente adicionar números: password1, password123
4. O método é **força bruta educacional** - teste palavras comuns
5. Dicionário sugerido: password, admin, secret, letmein, welcome, sunshine, 123456, password123

### 📚 Conceitos Ensinados

- **Hash Criptográfico**: Função unidirecional que produz valor único
- **SHA256**: Algoritmo padrão que produz 256 bits (64 hex chars)
- **Força Bruta**: Testar múltiplas possibilidades até encontrar correspondência
- **Identificação de Algoritmos**: Método baseado no comprimento do hash

### 🔓 Próximo Desafio Desbloqueado

- ✅ `stego` comando adicionado
- 📄 Arquivo `image.png` criado com mensagem escondida

---

## Desafio 4: Arquivo Oculto

### 📊 Informações

| Propriedade | Valor |
|------------|-------|
| **ID** | 4 |
| **Categoria** | Steganography |
| **Dificuldade** | 🟡 Médio |
| **Pontos** | 125 |
| **Tempo Limite** | 15 minutos |
| **Comandos Disponíveis** | 14 |
| **Novo Comando** | `stego` |

### 🎯 Objetivo

Extrair mensagem escondida dentro de um arquivo "imagem" usando esteganografia.

### 📝 Passo a Passo

**Passo 1:** Listar arquivos
```bash
ls
```
Você verá: `image.png`

**Passo 2:** Tentar cat para visualizar
```bash
cat image.png
```
Resultado mostrará dados estranhos (mensagem oculta interna)

**Passo 3:** Usar stego para extrair
```bash
stego extract image.png
```
Resultado: `Check the system logs`

**Passo 4:** Esta é uma dica! Você precisa ir para próxima etapa
```bash
submit flag{check_the_logs}
```

### ✅ Flag Esperada

```
flag{check_the_logs}
```

### 💡 Dicas

1. **Esteganografia** esconde dados dentro de outros arquivos
2. **LSB (Least Significant Bit)** é a técnica comum - modifica bits de baixa ordem
3. Visualmente, o arquivo escondido não é detectável
4. A mensagem extraída diz "Check the system logs" - isso é importante para próximo nível!
5. Nem sempre mensagens extraídas são a flag - podem ser pistas

### 📚 Conceitos Ensinados

- **Esteganografia**: Arte de esconder dados dentro de outros dados
- **LSB Steganography**: Technique que altera os bits menos significantes
- **Detecção**: Como identificar dados escondidos
- **Imaging**: Dados podem ser escondidos em imagens, áudio, vídeo

### 🔓 Próximo Desafio Desbloqueado

- ✅ `analyze` comando adicionado
- 📄 Arquivo `/var/log/system.log` criado

---

## Desafio 5: Análise de Logs

### 📊 Informações

| Propriedade | Valor |
|------------|-------|
| **ID** | 5 |
| **Categoria** | Forensics |
| **Dificuldade** | 🔴 Difícil |
| **Pontos** | 150 |
| **Tempo Limite** | 20 minutos |
| **Comandos Disponíveis** | 15 |
| **Novo Comando** | `analyze` |

### 🎯 Objetivo

Analisar logs do sistema para identificar aspatrão de atividade suspeita e encontrar pistas escondidas.

### 📝 Passo a Passo

**Passo 1:** Navegar para o diretório de logs
```bash
cd /var/log
```

**Passo 2:** Listar conteúdo
```bash
ls
```
Você verá: `system.log`

**Passo 3:** Visualizar o arquivo
```bash
cat system.log
```
Resultado:
```
failed login root
failed login root
successful login analyst
```

**Passo 4:** Usar analyze para buscar padrões suspeitos
```bash
analyze system.log
```
Resultado:
```
Hidden directory unlocked: /usr/bin
The suspicious log entry points to /usr/bin/backup.sh
```

**Passo 5:** Navegar para o diretório apontado
```bash
cd /usr/bin
ls
```
Você verá: `backup.sh`

**Passo 6:** Submeter flag
```bash
submit flag{log_analysis_success}
```

### ✅ Flag Esperada

```
flag{log_analysis_success}
```

### 💡 Dicas

1. **Análise Forense**: Detecta padrões suspeitos em logs
2. Múltiplas tentativas falhadas = possível ataque brute force
3. Um login bem-sucedido após falhas = penetração bem-sucedida
4. Logs são auditoria crítica em segurança
5. O `analyze` busca por palavras-chave como "failed", "successful"
6. O resultado aponta para `/usr/bin/backup.sh` - próximo passo!

### 📚 Conceitos Ensinados

- **Análise de Logs**: Investigação de eventos do sistema
- **Detecção de Intrusão**: Identificar padrões de ataque
- **Forensics**: Investigação pós-incidente
- **Auditoria de Segurança**: Como encontrar evidências de compromisso

### 🔓 Próximo Desafio Desbloqueado

- ✅ `sudo` comando adicionado
- ✅ `exploit` comando adicionado
- 📄 Arquivo `/usr/bin/backup.sh` criado

---

## Desafio 6: Escalonamento de Privilégios

### 📊 Informações

| Propriedade | Valor |
|------------|-------|
| **ID** | 6 |
| **Categoria** | Privilege Escalation |
| **Dificuldade** | 🔴 Difícil |
| **Pontos** | 175 |
| **Tempo Limite** | 25 minutos |
| **Comandos Disponíveis** | 17 |
| **Novos Comandos** | `sudo`, `exploit` |

### 🎯 Objetivo

Explorar uma vulnerabilidade em script do sistema para ganhar acesso root e acessar arquivo final.

### 📝 Passo a Passo

**Passo 1:** Ir para o diretório /usr/bin
```bash
cd /usr/bin
```

**Passo 2:** Listar e visualizar o script
```bash
ls
cat backup.sh
```
Resultado: `#!/bin/bash\necho 'Running backup...'\n`

**Passo 3:** Explorar o script vulnerável com sudo
```bash
sudo exploit backup.sh
```
Resultado:
```
Privilege escalation successful. You are now root.
A final root-only key file has been created at /root/final.key
```

**Passo 4:** Verificar que você é root agora
```bash
whoami
```
Resultado: `root`

**Passo 5:** Submeter flag
```bash
submit flag{root_access_granted}
```

### ✅ Flag Esperada

```
flag{root_access_granted}
```

### 💡 Dicas

1. **Privilege Escalation** é processo de ganhar permissões elevadas
2. **sudo** permite executar comandos como outro usuário (normalmente root)
3. **exploit** command tenta explorar a vulnerabilidade no script
4. Após ganho de privilégio, arquivo `/root/final.key` fica acessível
5. Este é um conceito crítico em segurança - aplicações com privilégios excessivos são riscos
6. Em situação real, scripts não devem permitir escalação

### 📚 Conceitos Ensinados

- **Privilege Escalation**: Técnica para ganhar permissões elevadas
- **sudo**: Comando para executar com privilégios
- **Vulnerabilidades**: Scripts mal configurados são porta aberta
- **Segurança de Permissões**: Importância do princípio de menor privilégio
- **Path to Root**: Como atacantes ganham acesso total

### 🔓 Próximo Desafio Desbloqueado

- 📄 Arquivo `/root/final.key` criado (acessível apenas como root)
- 🏁 Desafio Final disponível

---

## Desafio 7: A Camada Final

### 📊 Informações

| Propriedade | Valor |
|------------|-------|
| **ID** | 7 |
| **Categoria** | Final Challenge |
| **Dificuldade** | 🔴 Difícil |
| **Pontos** | 200 |
| **Tempo Limite** | 30 minutos |
| **Comandos Disponíveis** | 17 (Todos) |

### 🎯 Objetivo

Integrar todos os conhecimentos adquiridos nos desafios anteriores para descriptografar arquivo final e completar o CTF.

### 📝 Passo a Passo

**Passo 1:** Já como root, navegue para /root
```bash
cd /root
```

**Passo 2:** Listar e visualizar arquivo final
```bash
ls
cat final.key
```
Resultado:
```
-----BEGIN MESSAGE-----
V2VsY29tZSB0byB0aGUgaW5uZXIgY2lyY2xlLg==
-----END MESSAGE-----
```

**Passo 3:** Reconhecer o padrão - Base64 entre BEGIN/END
```bash
decode final.key
```
Resultado: `[Base64] Welcome to the inner circle.`

**Passo 4:** Submeter flag final
```bash
submit flag{welcome_to_the_inner_circle}
```

### ✅ Flag Esperada

```
flag{welcome_to_the_inner_circle}
```

### 💡 Dicas

1. Este desafio integra **todos os conceitos anteriores**:
   - ✅ Base64 (Desafio 1)
   - ✅ César (Desafio 2)
   - ✅ Hash (Desafio 3)
   - ✅ Stego (Desafio 4)
   - ✅ Análise (Desafio 5)
   - ✅ Escalação (Desafio 6)

2. Arquivo está em formato PEM (Privacy Enhanced Mail)
3. Conteúdo está em Base64
4. O `decode` remove automaticamente o wrapper PEM
5. Flag contém mensagem de sucesso: "Welcome to the inner circle"

### 📚 Conceitos Ensinados

- **Integração de Técnicas**: Aplicar múltiplas técnicas de segurança
- **Format Recognition**: Identificar formatos standard (PEM, Base64)
- **Layered Encryption**: Múltiplas camadas de proteção
- **Capstone Challenge**: Demonstrar completude de conhecimento

### 🎉 CTF Completo!

Parabéns! Você completou todos os 7 desafios do CICADA-3301!

**Estatísticas Finais:**
- Total de Pontos: **775 puntos**
- Desafios Resolvidos: **7/7** (100%)
- Tempo: Variável
- Status: ✅ CTF COMPLETO

---

## Resumo de Flags

### Todas as Flags em Ordem

| Nível | Desafio | Flag | Pontos |
|-------|---------|------|--------|
| 1 | Mensagem Codificada | `flag{hello_investigator}` | 50 |
| 2 | Cifra de César | `flag{caesar_clue}` | 75 |
| 3 | Hash Misterioso | `flag{password123}` | 100 |
| 4 | Arquivo Oculto | `flag{check_the_logs}` | 125 |
| 5 | Análise de Logs | `flag{log_analysis_success}` | 150 |
| 6 | Escalonamento | `flag{root_access_granted}` | 175 |
| 7 | Camada Final | `flag{welcome_to_the_inner_circle}` | 200 |
| **TOTAL** | **7 Desafios** | **-** | **775** |

---

## Resumo de Comandos por Nível

### Nível 1: Encoding Basics (11 comandos)
```
ls, cd, pwd, cat, clear, decode, help, history, whoami, submit, manual
```

### Nível 2: Cryptography (+ 1 comando)
```
+ decrypt
```

### Nível 3: Hash Analysis (+ 1 comando)
```
+ hash
```

### Nível 4: Steganography (+ 1 comando)
```
+ stego
```

### Nível 5: Forensics (+ 1 comando)
```
+ analyze
```

### Nível 6-7: Advanced (+ 2 comandos)
```
+ sudo, exploit
```

---

## Dicas Gerais para Todos os Desafios

### ✅ Boas Práticas

1. **Leia as mensagens cuidadosamente** - contêm pistas
2. **Use `help` em cada nível** - mostra comandos disponíveis
3. **Explore o filesystem** - arquivos são deixados em locais específicos
4. **Tente `cat` em arquivos** - visualize conteúdo
5. **Use `submit` quando tiver a flag** - ativa próximo nível

### ⚠️ Erros Comuns

1. ❌ Não usar `decode` em Desafio 1 - você nunca encontrará a flag
2. ❌ Tentar usar `sudo` antes do Desafio 6 - não está disponível
3. ❌ Submeter flag errada - cuidado com maiúsculas/minúsculas
4. ❌ Não explorar `/var/log` para Desafio 5
5. ❌ Esquecer que `/root` só é acessível como root

### 🎓 Sequência de Conceitos

```
Base64 → Caesar → SHA256 → Esteganografia → Forensics → Escalação → Integração
Fácil      Fácil    Médio      Médio           Difícil      Difícil      Difícil
```

---

## Recursos de Aprendizado Relacionados

### Conceitos de Segurança

- **Encoding vs Encryptção**: Encoding não é segurança, apenas formatação
- **Cryptografia Clássica**: Entender limites de Caesar cipher
- **Hashing**: Funções unidirecionais para integridade
- **Esteganografia**: Esconder em vez de criptografar
- **Análise Forense**: Investigação de evidências digitais
- **Privilege Escalation**: Vetor de ataque crítico

### Próximos Passos

1. Estude criptografia moderna (AES, RSA)
2. Aprenda sobre buffer overflows
3. Explore web security (SQL Injection, XSS)
4. Pratique em plataformas como: OverTheWire, HackTheBox, TryHackMe
5. Participate em CTFs reais

---

## Troubleshooting

### Problema: "Comando não disponível neste nível"

**Solução**: Você avançou para um novo nível onde este comando ainda não foi desbloqueado. Use `help` para ver comandos disponíveis.

### Problema: "Flag incorreta!"

**Solução**: 
- Verifique maiúsculas/minúsculas
- Certifique-se de que extraiu corretamente
- Use `help` para confirmar que está no nível certo

### Problema: "Arquivo não encontrado"

**Solução**:
- Verifique seu diretório atual com `pwd`
- Use `ls` para listar arquivos
- Alguns arquivos são criados apenas após resolver desafio anterior

### Problema: "Permissão negada"

**Solução**:
- Você provavelmente está tentando acessar `/root` como user guest
- Complete Desafio 6 para obter privilégios root
- Após `exploit`, use `whoami` para confirmar

---

## Contato e Suporte

Para dúvidas sobre o projeto CICADA-3301:
- Verifique o arquivo `README.md`
- Consulte `QUICK_START.md` para setup
- Explore `ANALISE_COMPLETA.md` para arquitetura

---

**Última atualização**: 09 de Abril, 2026  
**Versão**: 1.0  
**Status**: ✅ Completo e Testado
