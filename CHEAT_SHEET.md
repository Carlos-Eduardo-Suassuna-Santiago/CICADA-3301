# CICADA-3301: Quick Reference & Cheat Sheet

## ⚡ Quick Start

```bash
# Login
Username: guest
Password: guest

# Navegar e explorar
ls                          # Listar arquivos
cd /home/guest              # Mudar diretório
pwd                         # Ver localização atual
cat <arquivo>               # Ver conteúdo
clear                       # Limpar tela
```

## 🎯 Solução Rápida - Passo a Passo

### ✅ Desafio 1: Mensagem Codificada (50 pts)

```bash
cat message.txt                        # Ver: SGVsbG8gSW52ZXN0aWdhdG9y
decode message.txt                     # Decoder Base64
submit flag{hello_investigator}
```

---

### ✅ Desafio 2: Cifra de César (75 pts)

```bash
cat letter.txt                         # Ver: Brx pdB irxqg wkh qhaw foxh
decrypt letter.txt                     # Auto-detect shift 3
submit flag{caesar_clue}
```

---

### ✅ Desafio 3: Hash Misterioso (100 pts)

```bash
cat password.hash                      # Ver hash longo
hash analyze password.hash             # Confirma: SHA256
hash generate password123              # Encontra match
submit flag{password123}
```

---

### ✅ Desafio 4: Arquivo Oculto (125 pts)

```bash
cat image.png                          # Ver dados estranhos
stego extract image.png                # Extrai: "Check the system logs"
submit flag{check_the_logs}
```

---

### ✅ Desafio 5: Análise de Logs (150 pts)

```bash
cd /var/log
cat system.log                         # Ver Failed/Success patterns
analyze system.log                     # Revela: /usr/bin/backup.sh
submit flag{log_analysis_success}
```

---

### ✅ Desafio 6: Escalonamento (175 pts)

```bash
cd /usr/bin
sudo exploit backup.sh                 # Executa exploração
whoami                                 # Confirma: root
submit flag{root_access_granted}
```

---

### ✅ Desafio 7: Camada Final (200 pts)

```bash
cd /root
cat final.key                          # Ver wrapped Base64
decode final.key                       # Decodifica
submit flag{welcome_to_the_inner_circle}
```

---

## 📊 Todas as Senhas & Hashes

| Desafio | Tipo | Valor | Flag |
|---------|------|-------|------|
| 1 | Base64 Msg | `SGVsbG8gSW52ZXN0aWdhdG9y` | `hello_investigator` |
| 2 | Caesar Text | `Brx pdB irxqg wkh qhaw foxh` (shift +3) | `caesar_clue` |
| 3 | SHA256 | `ef92b778bafe771e89245b89ecbc7502fccc466aa2885b78f93d8db23b3f95d8` | `password123` |
| 4 | Stego Msg | `Check the system logs` | `check_the_logs` |
| 5 | Log Pattern | Failed→Success auth | `log_analysis_success` |
| 6 | Exploit | backup.sh vuln | `root_access_granted` |
| 7 | Base64 Wrap | `-----BEGIN...V2VsY2...END-----` | `welcome_to_the_inner_circle` |

---

## 🗂️ Estrutura de Arquivos por Desafio

### Level 1 → Files Created
```
/home/guest/message.txt
/home/guest/letter.txt (criado ao completar)
```

### Level 2 → Files Created
```
/home/guest/letter.txt
/home/guest/password.hash (criado ao completar)
```

### Level 3 → Files Created
```
/home/guest/password.hash
/home/guest/image.png (criado ao completar)
```

### Level 4 → Files Created
```
/home/guest/image.png
/var/log/system.log (criado ao completar)
```

### Level 5 → Files Created
```
/var/log/system.log
/usr/bin/backup.sh (criado ao completar)
```

### Level 6 → Files Created
```
/usr/bin/backup.sh
/root/final.key (criado ao completar)
```

### Level 7 → Final
```
/root/final.key
/root/congratulations.txt (criado ao completar)
```

---

## 🔑 Todas as 7 Flags

```
1. flag{hello_investigator}
2. flag{caesar_clue}
3. flag{password123}
4. flag{check_the_logs}
5. flag{log_analysis_success}
6. flag{root_access_granted}
7. flag{welcome_to_the_inner_circle}
```

**Total: 775 pontos**

---

## 🛠️ Comandos Disponíveis por Nível

| Cmd | L1 | L2 | L3 | L4 | L5 | L6 | L7 |
|-----|----|----|----|----|----|----|-----|
| ls | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| cd | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| pwd | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| cat | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| clear | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| decode | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| decrypt | ❌ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| hash | ❌ | ❌ | ✅ | ✅ | ✅ | ✅ | ✅ |
| stego | ❌ | ❌ | ❌ | ✅ | ✅ | ✅ | ✅ |
| analyze | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ | ✅ |
| sudo | ❌ | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ |
| exploit | ❌ | ❌ | ❌ | ❌ | ❌ | ✅ | ✅ |
| help | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| history | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| whoami | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| submit | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| manual | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |

---

## 💡 Pro Tips

### Timing

- **Fácil (1-2)**: ~10 min total
- **Médio (3-4)**: ~15 min total
- **Difícil (5-7)**: ~35 min total
- **Total Esperado**: ~60 min

### Estratégia

1. Sempre use `help` ao mudar de nível
2. Explore com `ls` antes de usar comandos específicos
3. Use `cat` para visualizar antes de processar
4. Logs geralmente têm `/var/log` ou similar
5. Root files estão em `/root`

### Debugging

```bash
pwd                         # Confirme localização
ls -la                      # Veja permissões (não disponível, mas ls funciona)
whoami                      # Confirme usuário/root
history                     # Revise comandos
```

---

## 🚀 Uma Liners (Soluções Completas)

```bash
# Level 1 Solve
decode message.txt && submit flag{hello_investigator}

# Level 2 Solve  
decrypt letter.txt && submit flag{caesar_clue}

# Level 3 Solve
hash generate password123 && submit flag{password123}

# Level 4 Solve
stego extract image.png && submit flag{check_the_logs}

# Level 5 Solve
cd /var/log && analyze system.log && submit flag{log_analysis_success}

# Level 6 Solve
sudo exploit backup.sh && submit flag{root_access_granted}

# Level 7 Solve
decode /root/final.key && submit flag{welcome_to_the_inner_circle}
```

---

## 📱 Smartphone Quick Ref

**Flag 1**: `hello_investigator`
**Flag 2**: `caesar_clue`
**Flag 3**: `password123`
**Flag 4**: `check_the_logs`
**Flag 5**: `log_analysis_success`
**Flag 6**: `root_access_granted`
**Flag 7**: `welcome_to_the_inner_circle`

---

**Version**: 1.0 | **Updated**: 04/09/2026 | **Status**: ✅ Complete
