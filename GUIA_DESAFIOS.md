# 🛡️ Guia Completo: Resolvendo os 21 Desafios do CICADA-3301

## 🎯 Introdução

Bem-vindo ao **CICADA-3301**, um simulador de segurança cibernética com 21 desafios progressivos! Este guia irá te ensinar passo a passo como resolver cada desafio, desde criptografia básica até conceitos avançados de segurança.

### 📋 Pré-requisitos
- Acesso ao terminal do CICADA-3301
- Conhecimento básico de comandos Linux
- Familiaridade com conceitos de segurança

### 🚀 Como Começar
1. Execute o programa: `python main.py`
2. Faça login como `guest` (senha: `guest`)
3. Use o comando `submit <flag>` para enviar suas respostas

---

## 🏁 Desafio 1: Base64

### 📝 Descrição
"Decode the message in message.txt"

### 📁 Arquivos Criados
- `/home/guest/message.txt` - Contém uma string codificada em Base64

### 🔍 Como Resolver
1. Leia o arquivo: `cat message.txt`
2. Você verá: `SGVsbG8gQ3liZXIgQ3Rm`
3. Use o comando decode: `decode base64 SGVsbG8gQ3liZXIgQ3Rm`

### 💻 Comandos
```bash
cat message.txt
decode base64 SGVsbG8gQ3liZXIgQ3Rm
submit flag{hello_cyber_ctf}
```

### ✅ Flag
`flag{hello_cyber_ctf}`

---

## 🔐 Desafio 2: Caesar Cipher

### 📝 Descrição
"Decrypt the Caesar cipher in caesar.txt (shift 3)"

### 📁 Arquivos Criados
- `/home/guest/caesar.txt` - Contém texto cifrado com Caesar (deslocamento 3)

### 🔍 Como Resolver
1. Leia o arquivo: `cat caesar.txt`
2. Você verá: `FDHVDU LV HDVB`
3. Use o comando decrypt: `decrypt caesar FDHVDU LV HDVB 3`

### 💻 Comandos
```bash
cat caesar.txt
decrypt caesar "FDHVDU LV HDVB" 3
submit flag{caesar_is_easy}
```

### ✅ Flag
`flag{caesar_is_easy}`

---

## 🔑 Desafio 3: Vigenere Cipher

### 📝 Descrição
"Decrypt the Vigenere cipher in vigenere.txt (key: KEY)"

### 📁 Arquivos Criados
- `/home/guest/vigenere.txt` - Contém texto cifrado com Vigenère

### 🔍 Como Resolver
1. Leia o arquivo: `cat vigenere.txt`
2. Você verá: `WKLV LV D YLJHQHUB`
3. Use o comando decrypt: `decrypt vigenere WKLV LV D YLJHQHUB KEY`

### 💻 Comandos
```bash
cat vigenere.txt
decrypt vigenere "WKLV LV D YLJHQHUB" KEY
submit flag{vigenere_is_fun}
```

### ✅ Flag
`flag{vigenere_is_fun}`

---

## 🔢 Desafio 4: Hex Decoding

### 📝 Descrição
"Decode the hex string in hex.txt"

### 📁 Arquivos Criados
- `/home/guest/hex.txt` - Contém string hexadecimal

### 🔍 Como Resolver
1. Leia o arquivo: `cat hex.txt`
2. Você verá: `48656c6c6f20576f726c64`
3. Use o comando decode: `decode hex 48656c6c6f20576f726c64`

### 💻 Comandos
```bash
cat hex.txt
decode hex 48656c6c6f20576f726c64
submit flag{hex_is_simple}
```

### ✅ Flag
`flag{hex_is_simple}`

---

## 🔀 Desafio 5: Binary Decoding

### 📝 Descrição
"Decode the binary string in binary.txt"

### 📁 Arquivos Criados
- `/home/guest/binary.txt` - Contém string binária

### 🔍 Como Resolver
1. Leia o arquivo: `cat binary.txt`
2. Você verá: `01001000 01100101 01101100 01101100 01101111`
3. Use o comando decode: `decode binary "01001000 01100101 01101100 01101100 01101111"`

### 💻 Comandos
```bash
cat binary.txt
decode binary "01001000 01100101 01101100 01101100 01101111"
submit flag{binary_is_tricky}
```

### ✅ Flag
`flag{binary_is_tricky}`

---

## 🔒 Desafio 6: SHA256 Hash

### 📝 Descrição
"Find the original string for the SHA256 hash in hash.txt"

### 📁 Arquivos Criados
- `/home/guest/hash.txt` - Contém hash SHA256
- `/home/guest/hint.txt` - Dica sobre o conteúdo original

### 🔍 Como Resolver
1. Leia os arquivos: `cat hash.txt` e `cat hint.txt`
2. Hash: `a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3`
3. Dica: "The original string is 'hello world'"
4. Verifique com: `hash sha256 "hello world"`

### 💻 Comandos
```bash
cat hash.txt
cat hint.txt
hash sha256 "hello world"
submit flag{sha256_is_secure}
```

### ✅ Flag
`flag{sha256_is_secure}`

---

## 🖼️ Desafio 7: Steganography LSB

### 📝 Descrição
"Extract the hidden message from stego.png"

### 📁 Arquivos Criados
- `/home/guest/stego.png` - Imagem com mensagem oculta

### 🔍 Como Resolver
1. Use o comando stego: `stego lsb stego.png`
2. A mensagem oculta será extraída

### 💻 Comandos
```bash
stego lsb stego.png
submit flag{stego_is_hidden}
```

### ✅ Flag
`flag{stego_is_hidden}`

---

## 🔄 Desafio 8: ROT13 Cipher

### 📝 Descrição
"Decrypt the ROT13 cipher in rot13.txt"

### 📁 Arquivos Criados
- `/home/guest/rot13.txt` - Texto cifrado com ROT13

### 🔍 Como Resolver
1. Leia o arquivo: `cat rot13.txt`
2. Você verá: `URYYB JBEYQ`
3. ROT13 é Caesar com deslocamento 13: `decrypt caesar "URYYB JBEYQ" 13`

### 💻 Comandos
```bash
cat rot13.txt
decrypt caesar "URYYB JBEYQ" 13
submit flag{rot13_is_classic}
```

### ✅ Flag
`flag{rot13_is_classic}`

---

## 📡 Desafio 9: Morse Code

### 📝 Descrição
"Decode the Morse code in morse.txt"

### 📁 Arquivos Criados
- `/home/guest/morse.txt` - Código Morse

### 🔍 Como Resolver
1. Leia o arquivo: `cat morse.txt`
2. Você verá: `.... . .-.. .-.. ---`
3. Use o comando decode: `decode morse ".... . .-.. .-.. ---"`

### 💻 Comandos
```bash
cat morse.txt
decode morse ".... . .-.. .-.. ---"
submit flag{morse_is_dots}
```

### ✅ Flag
`flag{morse_is_dots}`

---

## 🔄 Desafio 10: Reverse String

### 📝 Descrição
"Reverse the string in reverse.txt to find the flag"

### 📁 Arquivos Criados
- `/home/guest/reverse.txt` - String invertida

### 🔍 Como Resolver
1. Leia o arquivo: `cat reverse.txt`
2. Você verá: `dlrow olleh`
3. Inverta manualmente: `hello world`
4. A flag é `flag{reverse_engineering}`

### 💻 Comandos
```bash
cat reverse.txt
# Inverta manualmente: dlrow olleh → hello world
submit flag{reverse_engineering}
```

### ✅ Flag
`flag{reverse_engineering}`

---

## 🔄 Desafio 11: Atbash Cipher

### 📝 Descrição
"Decrypt the Atbash cipher in atbash.txt"

### 📁 Arquivos Criados
- `/home/guest/atbash.txt` - Texto cifrado com Atbash

### 🔍 Como Resolver
1. Leia o arquivo: `cat atbash.txt`
2. Você verá: `SVOOL DLIOW`
3. Atbash troca A↔Z, B↔Y, etc.: `HELLO WORLD`
4. A flag é `flag{atbash_is_mirror}`

### 💻 Comandos
```bash
cat atbash.txt
# Atbash: S→H, V→E, O→L, O→L, etc.
submit flag{atbash_is_mirror}
```

### ✅ Flag
`flag{atbash_is_mirror}`

---

## 🛤️ Desafio 12: Rail Fence Cipher

### 📝 Descrição
"Decrypt the Rail Fence cipher in railfence.txt (rails: 3)"

### 📁 Arquivos Criados
- `/home/guest/railfence.txt` - Texto cifrado com Rail Fence

### 🔍 Como Resolver
1. Leia o arquivo: `cat railfence.txt`
2. Você verá: `HLOOLELWRD`
3. Rail Fence com 3 trilhos: reorganize em zigzag
4. Resultado: `HELLO WORLD`
5. A flag é `flag{rail_fence_zigzag}`

### 💻 Comandos
```bash
cat railfence.txt
# Rail Fence 3 rails:
# H   O   L   R
#  E L   E W D
#   L   O   L
# Result: HELLO WORLD
submit flag{rail_fence_zigzag}
```

### ✅ Flag
`flag{rail_fence_zigzag}`

---

## 🥓 Desafio 13: Bacon Cipher

### 📝 Descrição
"Decrypt the Bacon cipher in bacon.txt"

### 📁 Arquivos Criados
- `/home/guest/bacon.txt` - Código Bacon

### 🔍 Como Resolver
1. Leia o arquivo: `cat bacon.txt`
2. Você verá: `ABAAAABABABABABAABAB`
3. Bacon: A=0, B=1
4. Converta para binário e depois texto
5. Resultado: `HELLO`
6. A flag é `flag{bacon_is_pig}`

### 💻 Comandos
```bash
cat bacon.txt
# Bacon cipher: A=0, B=1
# ABAAA ABABA BABAB AABAB
# 01111 01010 10101 00101
# Em ASCII: H E L L O
submit flag{bacon_is_pig}
```

### ✅ Flag
`flag{bacon_is_pig}`

---

## 🧠 Desafio 14: Logic Puzzle

### 📝 Descrição
"Solve the logic puzzle in logic.txt"

### 📁 Arquivos Criados
- `/home/guest/logic.txt` - Puzzle lógico
- `/home/guest/clue.txt` - Pistas

### 🔍 Como Resolver
1. Leia os arquivos: `cat logic.txt` e `cat clue.txt`
2. Puzzle: "(A AND B) OR (C AND D)"
3. Pistas: "A=True, B=True, C=False, D=True"
4. Calcule: (True AND True) OR (False AND True) = True OR False = True
5. A flag é `flag{logic_is_truth}`

### 💻 Comandos
```bash
cat logic.txt
cat clue.txt
# (A AND B) OR (C AND D)
# (T AND T) OR (F AND T)
# T OR F = T
submit flag{logic_is_truth}
```

### ✅ Flag
`flag{logic_is_truth}`

---

## 💉 Desafio 15: SQL Injection

### 📝 Descrição
"Find the vulnerability in sql.txt"

### 📁 Arquivos Criados
- `/home/guest/sql.txt` - Query SQL

### 🔍 Como Resolver
1. Leia o arquivo: `cat sql.txt`
2. Você verá: `SELECT flag FROM challenges WHERE id=15`
3. Esta query é vulnerável a SQL injection
4. A flag é `flag{sql_injection_bad}`

### 💻 Comandos
```bash
cat sql.txt
# Query vulnerável: SELECT flag FROM challenges WHERE id=15
# Poderia ser injetado como: id=15 OR 1=1
submit flag{sql_injection_bad}
```

### ✅ Flag
`flag{sql_injection_bad}`

---

## ⚠️ Desafio 16: XSS Vulnerability

### 📝 Descrição
"Identify the XSS vulnerability in xss.txt"

### 📁 Arquivos Criados
- `/home/guest/xss.txt` - Código HTML com XSS

### 🔍 Como Resolver
1. Leia o arquivo: `cat xss.txt`
2. Você verá: `<script>alert('XSS')</script>`
3. Este é um exemplo clássico de XSS (Cross-Site Scripting)
4. A flag é `flag{xss_is_dangerous}`

### 💻 Comandos
```bash
cat xss.txt
# <script>alert('XSS')</script> - XSS vulnerability
submit flag{xss_is_dangerous}
```

### ✅ Flag
`flag{xss_is_dangerous}`

---

## 🔐 Desafio 17: Cryptography Basics

### 📝 Descrição
"Find the encryption key in the filesystem"

### 📁 Arquivos Criados
- `/home/guest/crypto.txt` - Mensagem sobre a chave
- `/etc/secret.key` - Arquivo com a chave

### 🔍 Como Resolver
1. Leia o arquivo: `cat crypto.txt`
2. Você verá: "The key is hidden in the filesystem"
3. Procure pela chave: `cat /etc/secret.key`
4. Você verá: `AES_KEY_123456789`
5. A flag é `flag{crypto_needs_keys}`

### 💻 Comandos
```bash
cat crypto.txt
cat /etc/secret.key
submit flag{crypto_needs_keys}
```

### ✅ Flag
`flag{crypto_needs_keys}`

---

## 🔍 Desafio 18: Digital Forensics

### 📝 Descrição
"Analyze the system logs for the flag"

### 📁 Arquivos Criados
- `/home/guest/forensic.txt` - Instrução para verificar logs

### 🔍 Como Resolver
1. Leia o arquivo: `cat forensic.txt`
2. Você verá: "Check the logs for suspicious activity"
3. Verifique os logs: `logread`
4. Procure por atividades suspeitas nos logs
5. A flag é `flag{forensics_is_clues}`

### 💻 Comandos
```bash
cat forensic.txt
logread
# Procure por atividades suspeitas nos logs
submit flag{forensics_is_clues}
```

### ✅ Flag
`flag{forensics_is_clues}`

---

## 🌐 Desafio 19: Network Analysis

### 📝 Descrição
"Analyze the network traffic in network.txt"

### 📁 Arquivos Criados
- `/home/guest/network.txt` - Dados de rede

### 🔍 Como Resolver
1. Leia o arquivo: `cat network.txt`
2. Você verá: `192.168.1.1:8080/flag`
3. Este é um endereço IP e porta suspeito
4. A flag é `flag{network_is_packets}`

### 💻 Comandos
```bash
cat network.txt
# 192.168.1.1:8080/flag - endereço suspeito
submit flag{network_is_packets}
```

### ✅ Flag
`flag{network_is_packets}`

---

## 🔑 Desafio 20: Password Cracking

### 📝 Descrição
"Crack the password hash in passwd.txt"

### 📁 Arquivos Criados
- `/home/guest/passwd.txt` - Hash de senha

### 🔍 Como Resolver
1. Leia o arquivo: `cat passwd.txt`
2. Você verá: `$2b$12$abcdefghijklmnopqrstuvwx`
3. Este é um hash bcrypt
4. Tente crackear ou identifique que é bcrypt
5. A flag é `flag{passwords_must_be_strong}`

### 💻 Comandos
```bash
cat passwd.txt
# $2b$12$... - bcrypt hash
submit flag{passwords_must_be_strong}
```

### ✅ Flag
`flag{passwords_must_be_strong}`

---

## 🎉 Desafio 21: Final Challenge

### 📝 Descrição
"Combine all techniques learned to find the final flag"

### 📁 Arquivos Criados
- `/home/guest/final_challenge.txt` - Desafio final

### 🔍 Como Resolver
1. Leia o arquivo: `cat final_challenge.txt`
2. Você verá: `Base64: RmxhZ3tjb25ncmF0dWxhdGlvbnNfbWFzdGVyfQ==`
3. Use decode base64: `decode base64 RmxhZ3tjb25ncmF0dWxhdGlvbnNfbWFzdGVyfQ==`
4. Resultado: `Flag{congratulations_master}`

### 💻 Comandos
```bash
cat final_challenge.txt
decode base64 RmxhZ3tjb25ncmF0dWxhdGlvbnNfbWFzdGVyfQ==
submit flag{congratulations_master}
```

### ✅ Flag
`flag{congratulations_master}`

---

## 🏆 Conclusão

Parabéns! Você completou todos os 21 desafios do CICADA-3301! 🎉

### 📊 Resumo das Técnicas Aprendidas

| Categoria | Técnicas | Desafios |
|-----------|----------|----------|
| **Codificação** | Base64, Hex, Binary, Morse | 1, 4, 5, 9 |
| **Criptografia** | Caesar, Vigenère, ROT13, Atbash, Rail Fence, Bacon | 2, 3, 8, 11, 12, 13 |
| **Hash** | SHA256, bcrypt | 6, 20 |
| **Esteganografia** | LSB | 7 |
| **Lógica** | Boolean logic | 14 |
| **Web Security** | SQL Injection, XSS | 15, 16 |
| **Sistema** | File system, Logs, Network | 17, 18, 19 |
| **Forense** | String reversal | 10 |

### 🎯 Próximos Passos
- Pratique em plataformas como HackTheBox, TryHackMe
- Estude certificações como CEH, OSCP
- Contribua para projetos open source de segurança

### 📞 Suporte
Se tiver dúvidas sobre algum desafio, consulte a documentação do projeto ou abra uma issue no repositório.

**Happy Hacking! 🔒**