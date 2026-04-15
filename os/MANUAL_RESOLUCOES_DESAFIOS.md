# CICADA-OS: Manual Completo de Resolucao dos Desafios (/os)

Este manual cobre as 7 missoes do kernel em `os/`, com base no comportamento real implementado em `kernel.c` e nos dados de `mission_data.h`.

## 1. Como executar o CTF do diretorio /os

```bash
cd os
make clean all
make run
```

O shell abre com login obrigatorio.

Credenciais padrao:

- root / root
- guest / guest
- analyst / analyst

## 2. Regras importantes do engine de missao

- O progresso e linear: so avanca para a proxima fase apos `submit flag{...}` correto.
- Comandos sao bloqueados por nivel. Use `help` para ver os disponiveis.
- `hint` mostra dicas da fase atual (`hint` ou `hint <numero>`).
- Pontuacao total maxima: 875 pontos.

## 3. Walkthrough completo por desafio

## Desafio 1 - Mensagem Codificada

Objetivo: decodificar Base64 em `/home/guest/message.txt`.

Comandos:

```bash
login
# Username: guest
# Password: guest

cd /home/guest
ls
cat message.txt
decode message.txt
submit flag{hello_investigator}
```

Saida esperada do decode:

```text
[BASE64] Hello Investigator
```

Flag da fase:

```text
flag{hello_investigator}
```

## Desafio 2 - Cifra de Cesar

Objetivo: decifrar `/home/guest/letter.txt`.

Comandos:

```bash
cd /home/guest
cat letter.txt
decrypt letter.txt
# opcional: decrypt caesar letter.txt 3
submit flag{caesar_clue}
```

Resultado esperado (auto ou shift 3):

```text
You may found the next clue
```

Flag da fase:

```text
flag{caesar_clue}
```

## Desafio 3 - Hash Misterioso

Objetivo: identificar o hash em `/home/guest/password.hash` e inferir senha.

Comandos:

```bash
cd /home/guest
cat password.hash
hash analyze password.hash
hash generate password123
submit flag{password123}
```

Observacao:

- O hash da fase e SHA256 de `password123`.

Flag da fase:

```text
flag{password123}
```

## Desafio 4 - Arquivo Oculto

Objetivo: extrair mensagem escondida em `/home/guest/image.png`.

Comandos:

```bash
cd /home/guest
cat image.png
stego extract image.png
submit flag{check_the_logs}
```

Saida esperada do stego:

```text
Check the system logs
```

Flag da fase:

```text
flag{check_the_logs}
```

## Desafio 5 - Analise de Logs

Objetivo: usar logs para descobrir proximo passo e desbloquear script.

Comandos:

```bash
cd /
cat /var/log/system.log
analyze /var/log/system.log
cat /usr/bin/backup.sh
submit flag{log_analysis_success}
```

Saida importante do analyze:

```text
Hidden directory unlocked: /usr/bin
The suspicious log entry points to /usr/bin/backup.sh
```

Flag da fase:

```text
flag{log_analysis_success}
```

## Desafio 6 - Escalonamento

Objetivo: explorar `backup.sh` para preparar chave final.

Ponto critico de implementacao:

- O comando `exploit` exige usuario root no momento da execucao.
- No kernel desta pasta, a credencial padrao do root e `root`.

Comandos recomendados:

```bash
login
# Username: root
# Password: root

exploit /usr/bin/backup.sh
submit flag{root_access_granted}
```

Saida esperada do exploit:

```text
Privilege escalation successful. You are now root.
A final root-only key file has been created at /root/final.key
```

Flag da fase:

```text
flag{root_access_granted}
```

## Desafio 7 - Camada Final

Objetivo: ler `/root/final.key`, decodificar o bloco Base64 e concluir.

Comandos:

```bash
cat /root/final.key
decode base64 /root/final.key
submit flag{welcome_to_the_inner_circle}
```

Saida esperada do decode:

```text
Welcome to the inner circle.
```

Flag da fase:

```text
flag{welcome_to_the_inner_circle}
```

## 4. Lista final de todas as flags

```text
1) flag{hello_investigator}
2) flag{caesar_clue}
3) flag{password123}
4) flag{check_the_logs}
5) flag{log_analysis_success}
6) flag{root_access_granted}
7) flag{welcome_to_the_inner_circle}
```

## 5. Comandos uteis durante a resolucao

```bash
help
hint
hint 1
hint 2
hint 3
history
whoami
pwd
ls
```

Fim.
