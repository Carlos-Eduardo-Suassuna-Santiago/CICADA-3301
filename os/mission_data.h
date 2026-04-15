#ifndef MISSION_DATA_H
#define MISSION_DATA_H

/*
 * Fonte unica de dados de missoes do kernel.
 * Formato simples (compile-time):
 * - listas de comandos por nivel
 * - dicas por nivel
 * - acoes on_start/on_complete com owner/perm
 * - tabela final de missoes
 */

static const char *lvl1_cmds[] = {"ls", "cd", "pwd", "cat", "clear", "decode", "help", "history", "whoami", "submit", "manual", "hint", "shutdown"};
static const char *lvl2_cmds[] = {"ls", "cd", "pwd", "cat", "clear", "decode", "decrypt", "help", "history", "whoami", "submit", "manual", "hint", "shutdown"};
static const char *lvl3_cmds[] = {"ls", "cd", "pwd", "cat", "clear", "decode", "decrypt", "hash", "help", "history", "whoami", "submit", "manual", "hint", "shutdown"};
static const char *lvl4_cmds[] = {"ls", "cd", "pwd", "cat", "clear", "decode", "decrypt", "hash", "stego", "help", "history", "whoami", "submit", "manual", "hint", "shutdown"};

static const struct command_meta command_catalog[] = {
    {"help", "List available commands", "help [command]"},
    {"clear", "Clear terminal screen", "clear"},
    {"echo", "Print text to screen", "echo <text>"},
    {"pwd", "Show current directory", "pwd"},
    {"ls", "List files and directories", "ls [directory]"},
    {"cd", "Change current directory", "cd <directory>"},
    {"cat", "Display file content", "cat <file>"},
    {"touch", "Create empty file", "touch <file>"},
    {"mkdir", "Create directory", "mkdir <directory>"},
    {"rmdir", "Remove empty directory", "rmdir <directory>"},
    {"rm", "Remove file", "rm <file>"},
    {"write", "Write text to file", "write <file> <text>"},
    {"append", "Append text to file", "append <file> <text>"},
    {"cp", "Copy file", "cp <source> <dest>"},
    {"mv", "Move/rename file", "mv <source> <dest>"},
    {"history", "Show command history", "history"},
    {"find", "Find files/directories by name", "find <term>"},
    {"tree", "Show directory tree", "tree [directory]"},
    {"login", "Login with username/password", "login"},
    {"logout", "Logout to guest", "logout"},
    {"passwd", "Change current user password", "passwd"},
    {"su", "Switch to another user", "su <username>"},
    {"sudo", "Run command with privilege", "sudo <command>"},
    {"who", "Show active session", "who"},
    {"whoami", "Show current user", "whoami"},
    {"uname", "Show system info", "uname"},
    {"ps", "List system processes", "ps"},
    {"logread", "Read system logs", "logread [n_lines|filter]"},
    {"manual", "Display detailed command manual", "manual [command]"},
    {"decode", "Decode encoded text", "decode <type> <content> | decode <file>"},
    {"decrypt", "Decrypt classic ciphers", "decrypt <file> | decrypt <caesar|vigenere> <file> <key>"},
    {"hash", "Generate/analyze hash", "hash <generate|analyze> <text|file>"},
    {"stego", "Extract hidden message", "stego extract <image>"},
    {"analyze", "Analyze file for clues", "analyze <file>"},
    {"exploit", "Simulate exploitation", "exploit <script>"},
    {"submit", "Submit challenge flag", "submit <flag>"},
    {"ticks", "Show PIT tick counter", "ticks"},
    {"tasks", "Show scheduler state", "tasks"},
    {"syscall", "Test int 0x80 syscall", "syscall"},
    {"hint", "Show mission hints", "hint [n]"},
    {"shutdown", "Power off system", "shutdown"},
    {"exit", "Exit shell", "exit"}
};

static const char *lvl5_cmds[] = {"ls", "cd", "pwd", "cat", "clear", "decode", "decrypt", "hash", "stego", "analyze", "help", "history", "whoami", "submit", "manual", "hint", "shutdown"};
static const char *lvl6_cmds[] = {
    "ls", "cd", "pwd", "cat", "clear", "decode", "decrypt", "hash", "stego", "analyze",
    "login", "logout", "passwd", "su", "sudo", "who", "whoami", "ps", "logread", "manual",
    "history", "help", "submit", "hint", "exploit", "shutdown"
};
static const char *lvl7_cmds[] = {
    "ls", "cd", "pwd", "cat", "clear", "decode", "decrypt", "hash", "stego", "analyze",
    "login", "logout", "passwd", "su", "sudo", "who", "whoami", "ps", "logread", "manual",
    "history", "help", "submit", "hint", "exploit", "shutdown"
};

static const char *lvl1_hints[] = {
    "O arquivo message.txt esta em Base64.",
    "Use decode no conteudo do arquivo.",
    "A resposta descreve o proximo passo."
};
static const char *lvl2_hints[] = {
    "letter.txt usa cifra de Cesar.",
    "Tente deslocamento 3.",
    "A frase decifrada aponta para a flag."
};
static const char *lvl3_hints[] = {
    "password.hash parece SHA256.",
    "Use hash para testar candidatos.",
    "password123 eh o valor esperado nesta fase."
};
static const char *lvl4_hints[] = {
    "image.png contem mensagem textual simulada.",
    "Use stego para extrair pistas.",
    "A dica aponta para logs do sistema."
};
static const char *lvl5_hints[] = {
    "Leia /var/log/system.log.",
    "Use analyze para resumir texto.",
    "A pista desbloqueia /usr/bin/backup.sh."
};
static const char *lvl6_hints[] = {
    "backup.sh representa um vetor de escalonamento.",
    "Use exploit para simular a exploracao.",
    "Depois submeta a flag da fase."
};
static const char *lvl7_hints[] = {
    "Abra /root/final.key.",
    "Decode o bloco Base64 interno.",
    "A mensagem final conclui o CTF."
};

static const struct mission_action lvl1_start[] = {{"/home/guest/message.txt", "SGVsbG8gSW52ZXN0aWdhdG9y", "guest", "644"}};
static const struct mission_action lvl1_done[] = {{"/home/guest/letter.txt", "Brx pdB irxqg wkh qhaw foxh", "guest", "644"}};
static const struct mission_action lvl2_start[] = {{"/home/guest/letter.txt", "Brx pdB irxqg wkh qhaw foxh", "guest", "644"}};
static const struct mission_action lvl2_done[] = {{"/home/guest/password.hash", "ef92b778bafe771e89245b89ecbc08a44a4e166c06659911881f383d4473e94f", "guest", "644"}};
static const struct mission_action lvl3_start[] = {{"/home/guest/password.hash", "ef92b778bafe771e89245b89ecbc08a44a4e166c06659911881f383d4473e94f", "guest", "644"}};
static const struct mission_action lvl3_done[] = {{"/home/guest/image.png", "STEGOMSG:Check the system logs", "guest", "644"}};
static const struct mission_action lvl4_start[] = {{"/home/guest/image.png", "STEGOMSG:Check the system logs", "guest", "644"}};
static const struct mission_action lvl4_done[] = {{"/var/log/system.log", "failed login root\nfailed login root\nsuccessful login analyst", "root", "644"}};
static const struct mission_action lvl5_start[] = {{"/var/log/system.log", "failed login root\nfailed login root\nsuccessful login analyst", "root", "644"}};
static const struct mission_action lvl5_done[] = {{"/usr/bin/backup.sh", "#!/bin/bash\necho 'Running backup...'\n", "root", "755"}};
static const struct mission_action lvl6_start[] = {{"/usr/bin/backup.sh", "#!/bin/bash\necho 'Running backup...'\n", "root", "755"}};
static const struct mission_action lvl7_start[] = {{"/root/final.key", "-----BEGIN MESSAGE-----\nV2VsY29tZSB0byB0aGUgaW5uZXIgY2lyY2xlLg==\n-----END MESSAGE-----\n", "root", "600"}};
static const struct mission_action lvl7_done[] = {{"/root/congratulations.txt", "Welcome to the inner circle.", "root", "600"}};

static const struct mission_def missions[] = {
    {1, "Mensagem Codificada", "Decode o texto Base64 em message.txt", 50, "flag{hello_investigator}", lvl1_cmds, (int)(sizeof(lvl1_cmds)/sizeof(lvl1_cmds[0])), lvl1_hints, (int)(sizeof(lvl1_hints)/sizeof(lvl1_hints[0])), lvl1_start, 1, lvl1_done, 1},
    {2, "Cifra de Cesar", "Decifre letter.txt com Cesar", 75, "flag{caesar_clue}", lvl2_cmds, (int)(sizeof(lvl2_cmds)/sizeof(lvl2_cmds[0])), lvl2_hints, (int)(sizeof(lvl2_hints)/sizeof(lvl2_hints[0])), lvl2_start, 1, lvl2_done, 1},
    {3, "Hash Misterioso", "Identifique hash e senha", 100, "flag{password123}", lvl3_cmds, (int)(sizeof(lvl3_cmds)/sizeof(lvl3_cmds[0])), lvl3_hints, (int)(sizeof(lvl3_hints)/sizeof(lvl3_hints[0])), lvl3_start, 1, lvl3_done, 1},
    {4, "Arquivo Oculto", "Extraia mensagem de image.png", 125, "flag{check_the_logs}", lvl4_cmds, (int)(sizeof(lvl4_cmds)/sizeof(lvl4_cmds[0])), lvl4_hints, (int)(sizeof(lvl4_hints)/sizeof(lvl4_hints[0])), lvl4_start, 1, lvl4_done, 1},
    {5, "Analise de Logs", "Use logs para achar proximo passo", 150, "flag{log_analysis_success}", lvl5_cmds, (int)(sizeof(lvl5_cmds)/sizeof(lvl5_cmds[0])), lvl5_hints, (int)(sizeof(lvl5_hints)/sizeof(lvl5_hints[0])), lvl5_start, 1, lvl5_done, 1},
    {6, "Escalonamento", "Explore backup.sh para acesso root", 175, "flag{root_access_granted}", lvl6_cmds, (int)(sizeof(lvl6_cmds)/sizeof(lvl6_cmds[0])), lvl6_hints, (int)(sizeof(lvl6_hints)/sizeof(lvl6_hints[0])), lvl6_start, 1, 0, 0},
    {7, "Camada Final", "Decodifique /root/final.key", 200, "flag{welcome_to_the_inner_circle}", lvl7_cmds, (int)(sizeof(lvl7_cmds)/sizeof(lvl7_cmds[0])), lvl7_hints, (int)(sizeof(lvl7_hints)/sizeof(lvl7_hints[0])), lvl7_start, 1, lvl7_done, 1}
};

#endif
