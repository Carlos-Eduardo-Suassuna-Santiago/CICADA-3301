#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define UI_HEADER_LINES 10
#define VGA_MEMORY ((volatile uint16_t *)0xB8000)
#define SERIAL_PORT 0x3F8

static volatile uint16_t *const VGA = VGA_MEMORY;
static uint16_t cursor_pos = 0;
static const uint8_t default_color = 0x07;

static inline uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ volatile ("inb %1, %0" : "=a" (value) : "Nd" (port));
    return value;
}

static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a" (value), "Nd" (port));
}

static inline void outw(uint16_t port, uint16_t value) {
    __asm__ volatile ("outw %0, %1" : : "a" (value), "Nd" (port));
}

static inline uint8_t serial_is_transmit_empty(void) {
    return inb(SERIAL_PORT + 5) & 0x20;
}

static inline uint8_t serial_has_data(void) {
    return inb(SERIAL_PORT + 5) & 0x01;
}

static void serial_putchar(char c) {
    while (!serial_is_transmit_empty()) {
        ;
    }
    outb(SERIAL_PORT, c);
}

static void serial_init(void) {
    outb(SERIAL_PORT + 1, 0x00);
    outb(SERIAL_PORT + 3, 0x80);
    outb(SERIAL_PORT + 0, 0x03);
    outb(SERIAL_PORT + 1, 0x00);
    outb(SERIAL_PORT + 3, 0x03);
    outb(SERIAL_PORT + 2, 0xC7);
    outb(SERIAL_PORT + 4, 0x0B);
}

static void serial_puts_raw(const char *str) {
    while (*str) {
        while (!serial_is_transmit_empty()) {
            ;
        }
        outb(SERIAL_PORT, (uint8_t)*str++);
    }
}

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct idt_entry {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_hi;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

static struct gdt_entry gdt_entries[3];
static struct gdt_ptr gdt_ptr;
static struct idt_entry idt_entries[256];
static struct idt_ptr idt_ptr;

static void print_boot_logo(void);
static int str_contains(const char *text, const char *pattern);

extern void irq0_handler(void);
extern void irq1_handler(void);
extern void isr80_handler(void);

static void scroll(void) {
    int start_row = UI_HEADER_LINES;
    if (start_row < 0 || start_row >= VGA_HEIGHT) {
        start_row = 0;
    }

    for (int y = start_row + 1; y < VGA_HEIGHT; ++y) {
        for (int x = 0; x < VGA_WIDTH; ++x) {
            VGA[(y - 1) * VGA_WIDTH + x] = VGA[y * VGA_WIDTH + x];
        }
    }
    uint16_t blank = ' ' | (default_color << 8);
    for (int x = 0; x < VGA_WIDTH; ++x) {
        VGA[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = blank;
    }
    cursor_pos = (VGA_HEIGHT - 1) * VGA_WIDTH;
}

static void putchar(char c) {
    if (c == '\r') {
        return;
    }

    if (c == '\n') {
        cursor_pos += VGA_WIDTH - (cursor_pos % VGA_WIDTH);
    } else {
        VGA[cursor_pos++] = (uint16_t)c | (default_color << 8);
    }

    if (cursor_pos >= VGA_WIDTH * VGA_HEIGHT) {
        scroll();
    }
    serial_putchar(c);
}

static void puts(const char *str) {
    while (*str) {
        putchar(*str++);
    }
}

static void print_uint(uint32_t value) {
    char buffer[11];
    int pos = 0;
    if (value == 0) {
        putchar('0');
        return;
    }
    while (value > 0 && pos < (int)sizeof(buffer) - 1) {
        buffer[pos++] = '0' + (value % 10);
        value /= 10;
    }
    while (pos > 0) {
        putchar(buffer[--pos]);
    }
}

static int strcmp(const char *a, const char *b) {
    while (*a && *a == *b) {
        ++a;
        ++b;
    }
    return (int)(uint8_t)*a - (int)(uint8_t)*b;
}

static int strncmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; ++i) {
        if (a[i] != b[i]) {
            return (int)(uint8_t)a[i] - (int)(uint8_t)b[i];
        }
        if (a[i] == '\0') {
            return 0;
        }
    }
    return 0;
}

static const char *skip_spaces(const char *s) {
    while (*s == ' ' || *s == '\t') {
        ++s;
    }
    return s;
}

struct file_entry {
    int used;
    char path[128];
    char content[512];
};

struct dir_entry {
    int used;
    char path[128];
};

#define MAX_FILES 64
#define MAX_DIRS 32
#define MAX_HISTORY 32
#define MAX_USERS 8
#define MAX_LOGS 64

static struct file_entry fs_files[MAX_FILES];
static struct dir_entry fs_dirs[MAX_DIRS];
static char current_path[128] = "/";
static char command_history[MAX_HISTORY][128];
static int history_count = 0;

struct user_entry {
    int used;
    char name[32];
    char pass[32];
    int is_admin;
};

static struct user_entry users[MAX_USERS];
static int current_user = 0;
static char system_logs[MAX_LOGS][128];
static int log_count = 0;

struct mission_action {
    const char *path;
    const char *content;
};

struct mission_def {
    int id;
    const char *name;
    const char *description;
    int points;
    const char *flag;
    const char **commands;
    int command_count;
    const char **hints;
    int hint_count;
    const struct mission_action *on_start;
    int on_start_count;
    const struct mission_action *on_complete;
    int on_complete_count;
};

static const char *lvl1_cmds[] = {"ls", "cd", "pwd", "cat", "clear", "decode", "help", "history", "whoami", "submit", "manual", "hint", "shutdown"};
static const char *lvl2_cmds[] = {"ls", "cd", "pwd", "cat", "clear", "decode", "decrypt", "help", "history", "whoami", "submit", "manual", "hint", "shutdown"};
static const char *lvl3_cmds[] = {"ls", "cd", "pwd", "cat", "clear", "decode", "decrypt", "hash", "help", "history", "whoami", "submit", "manual", "hint", "shutdown"};
static const char *lvl4_cmds[] = {"ls", "cd", "pwd", "cat", "clear", "decode", "decrypt", "hash", "stego", "help", "history", "whoami", "submit", "manual", "hint", "shutdown"};

static const char *all_shell_commands[] = {
    "help", "clear", "echo", "pwd", "ls", "cd", "cat", "touch", "mkdir", "rmdir", "rm",
    "write", "append", "cp", "mv", "history", "find", "tree", "login", "logout", "passwd",
    "su", "sudo", "who", "whoami", "uname", "ps", "logread", "manual", "decode", "decrypt",
    "hash", "stego", "analyze", "exploit", "submit", "ticks", "tasks", "syscall", "hint",
    "shutdown", "exit"
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

static const struct mission_action lvl1_start[] = {{"/home/guest/message.txt", "SGVsbG8gSW52ZXN0aWdhdG9y"}};
static const struct mission_action lvl1_done[] = {{"/home/guest/letter.txt", "Brx pdB irxqg wkh qhaw foxh"}};
static const struct mission_action lvl2_start[] = {{"/home/guest/letter.txt", "Brx pdB irxqg wkh qhaw foxh"}};
static const struct mission_action lvl2_done[] = {{"/home/guest/password.hash", "ef92b778bafe771e89245b89ecbc08a44a4e166c06659911881f383d4473e94f"}};
static const struct mission_action lvl3_start[] = {{"/home/guest/password.hash", "ef92b778bafe771e89245b89ecbc08a44a4e166c06659911881f383d4473e94f"}};
static const struct mission_action lvl3_done[] = {{"/home/guest/image.png", "STEGOMSG:Check the system logs"}};
static const struct mission_action lvl4_start[] = {{"/home/guest/image.png", "STEGOMSG:Check the system logs"}};
static const struct mission_action lvl4_done[] = {{"/var/log/system.log", "failed login root\nfailed login root\nsuccessful login analyst"}};
static const struct mission_action lvl5_start[] = {{"/var/log/system.log", "failed login root\nfailed login root\nsuccessful login analyst"}};
static const struct mission_action lvl5_done[] = {{"/usr/bin/backup.sh", "#!/bin/bash\necho 'Running backup...'\n"}};
static const struct mission_action lvl6_start[] = {{"/usr/bin/backup.sh", "#!/bin/bash\necho 'Running backup...'\n"}};
static const struct mission_action lvl7_start[] = {{"/root/final.key", "-----BEGIN MESSAGE-----\nV2VsY29tZSB0byB0aGUgaW5uZXIgY2lyY2xlLg==\n-----END MESSAGE-----\n"}};
static const struct mission_action lvl7_done[] = {{"/root/congratulations.txt", "Welcome to the inner circle."}};

static const struct mission_def missions[] = {
    {1, "Mensagem Codificada", "Decode o texto Base64 em message.txt", 50, "flag{hello_investigator}", lvl1_cmds, (int)(sizeof(lvl1_cmds)/sizeof(lvl1_cmds[0])), lvl1_hints, (int)(sizeof(lvl1_hints)/sizeof(lvl1_hints[0])), lvl1_start, 1, lvl1_done, 1},
    {2, "Cifra de Cesar", "Decifre letter.txt com Cesar", 75, "flag{caesar_clue}", lvl2_cmds, (int)(sizeof(lvl2_cmds)/sizeof(lvl2_cmds[0])), lvl2_hints, (int)(sizeof(lvl2_hints)/sizeof(lvl2_hints[0])), lvl2_start, 1, lvl2_done, 1},
    {3, "Hash Misterioso", "Identifique hash e senha", 100, "flag{password123}", lvl3_cmds, (int)(sizeof(lvl3_cmds)/sizeof(lvl3_cmds[0])), lvl3_hints, (int)(sizeof(lvl3_hints)/sizeof(lvl3_hints[0])), lvl3_start, 1, lvl3_done, 1},
    {4, "Arquivo Oculto", "Extraia mensagem de image.png", 125, "flag{check_the_logs}", lvl4_cmds, (int)(sizeof(lvl4_cmds)/sizeof(lvl4_cmds[0])), lvl4_hints, (int)(sizeof(lvl4_hints)/sizeof(lvl4_hints[0])), lvl4_start, 1, lvl4_done, 1},
    {5, "Analise de Logs", "Use logs para achar proximo passo", 150, "flag{log_analysis_success}", lvl5_cmds, (int)(sizeof(lvl5_cmds)/sizeof(lvl5_cmds[0])), lvl5_hints, (int)(sizeof(lvl5_hints)/sizeof(lvl5_hints[0])), lvl5_start, 1, lvl5_done, 1},
    {6, "Escalonamento", "Explore backup.sh para acesso root", 175, "flag{root_access_granted}", lvl6_cmds, (int)(sizeof(lvl6_cmds)/sizeof(lvl6_cmds[0])), lvl6_hints, (int)(sizeof(lvl6_hints)/sizeof(lvl6_hints[0])), lvl6_start, 1, 0, 0},
    {7, "Camada Final", "Decodifique /root/final.key", 200, "flag{welcome_to_the_inner_circle}", lvl7_cmds, (int)(sizeof(lvl7_cmds)/sizeof(lvl7_cmds[0])), lvl7_hints, (int)(sizeof(lvl7_hints)/sizeof(lvl7_hints[0])), lvl7_start, 1, lvl7_done, 1}
};

static const int mission_count = (int)(sizeof(missions) / sizeof(missions[0]));
static int mission_current = 0;
static int mission_score = 0;

static int str_len(const char *s) {
    int i = 0;
    while (s[i] != '\0') {
        ++i;
    }
    return i;
}

static void str_copy(char *dst, const char *src, int max_len) {
    int i = 0;
    while (i < max_len - 1 && src[i] != '\0') {
        dst[i] = src[i];
        ++i;
    }
    dst[i] = '\0';
}

static void str_concat(char *dst, const char *src, int max_len) {
    int i = str_len(dst);
    int j = 0;
    while (i < max_len - 1 && src[j] != '\0') {
        dst[i++] = src[j++];
    }
    dst[i] = '\0';
}

static int str_eq(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

static void log_event(const char *msg) {
    if (log_count < MAX_LOGS) {
        str_copy(system_logs[log_count], msg, 128);
        log_count++;
        return;
    }
    for (int i = 1; i < MAX_LOGS; ++i) {
        str_copy(system_logs[i - 1], system_logs[i], 128);
    }
    str_copy(system_logs[MAX_LOGS - 1], msg, 128);
}

static int user_find(const char *name) {
    for (int i = 0; i < MAX_USERS; ++i) {
        if (users[i].used && str_eq(users[i].name, name)) {
            return i;
        }
    }
    return -1;
}

static void users_init(void) {
    for (int i = 0; i < MAX_USERS; ++i) {
        users[i].used = 0;
    }

    users[0].used = 1;
    str_copy(users[0].name, "root", 32);
    str_copy(users[0].pass, "root", 32);
    users[0].is_admin = 1;

    users[1].used = 1;
    str_copy(users[1].name, "guest", 32);
    str_copy(users[1].pass, "guest", 32);
    users[1].is_admin = 0;

    users[2].used = 1;
    str_copy(users[2].name, "analyst", 32);
    str_copy(users[2].pass, "analyst", 32);
    users[2].is_admin = 0;

    current_user = 0;
}

static int parse_two_tokens(const char *src, char *a, int a_len, char *b, int b_len) {
    const char *p = skip_spaces(src);
    int i = 0;
    int j = 0;

    while (*p != '\0' && *p != ' ' && *p != '\t' && i < a_len - 1) {
        a[i++] = *p++;
    }
    a[i] = '\0';

    p = skip_spaces(p);
    while (*p != '\0' && *p != ' ' && *p != '\t' && j < b_len - 1) {
        b[j++] = *p++;
    }
    b[j] = '\0';

    return a[0] != '\0' && b[0] != '\0';
}

static int hex_val(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    if (c >= 'a' && c <= 'f') {
        return 10 + (c - 'a');
    }
    if (c >= 'A' && c <= 'F') {
        return 10 + (c - 'A');
    }
    return -1;
}

static int decode_hex_to_buf(const char *hex, char *out, int out_len) {
    int n = str_len(hex);
    int o = 0;
    if (n % 2 != 0) {
        return 0;
    }
    for (int i = 0; i < n && o < out_len - 1; i += 2) {
        int hi = hex_val(hex[i]);
        int lo = hex_val(hex[i + 1]);
        if (hi < 0 || lo < 0) {
            return 0;
        }
        out[o++] = (char)((hi << 4) | lo);
    }
    out[o] = '\0';
    return 1;
}

static int is_base64_char(char c) {
    return (c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z') ||
           (c >= '0' && c <= '9') ||
           c == '+' || c == '/';
}

static int base64_val(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c - 'A';
    }
    if (c >= 'a' && c <= 'z') {
        return 26 + (c - 'a');
    }
    if (c >= '0' && c <= '9') {
        return 52 + (c - '0');
    }
    if (c == '+') {
        return 62;
    }
    if (c == '/') {
        return 63;
    }
    return -1;
}

static int decode_base64_to_buf(const char *in, char *out, int out_len) {
    char clean[768];
    int n = 0;
    int line_start = 1;

    for (int i = 0; in[i] != '\0' && n < (int)sizeof(clean) - 1; ++i) {
        char c = in[i];

        if (line_start && c == '-') {
            while (in[i] != '\0' && in[i] != '\n') {
                ++i;
            }
            line_start = 1;
            continue;
        }

        if (c == '\n' || c == '\r') {
            line_start = 1;
            continue;
        }

        line_start = 0;
        if (is_base64_char(c) || c == '=') {
            clean[n++] = c;
        }
    }
    clean[n] = '\0';

    if (n < 4 || (n % 4) != 0) {
        return 0;
    }

    int o = 0;
    for (int i = 0; i < n && o < out_len - 1; i += 4) {
        int a = base64_val(clean[i]);
        int b = base64_val(clean[i + 1]);
        int c = (clean[i + 2] == '=') ? -2 : base64_val(clean[i + 2]);
        int d = (clean[i + 3] == '=') ? -2 : base64_val(clean[i + 3]);

        if (a < 0 || b < 0 || c < -1 || d < -1) {
            return 0;
        }

        out[o++] = (char)((a << 2) | (b >> 4));
        if (clean[i + 2] != '=') {
            if (o >= out_len - 1) {
                break;
            }
            out[o++] = (char)(((b & 0x0F) << 4) | (c >> 2));
        }
        if (clean[i + 3] != '=') {
            if (o >= out_len - 1) {
                break;
            }
            out[o++] = (char)(((c & 0x03) << 6) | d);
        }
    }
    out[o] = '\0';
    return 1;
}

static int decode_binary_to_buf(const char *in, char *out, int out_len) {
    int o = 0;
    int bit_count = 0;
    int value = 0;

    for (int i = 0; in[i] != '\0'; ++i) {
        char c = in[i];
        if (c == '0' || c == '1') {
            value = (value << 1) | (c - '0');
            bit_count++;
            if (bit_count == 8) {
                if (o >= out_len - 1) {
                    break;
                }
                out[o++] = (char)value;
                value = 0;
                bit_count = 0;
            }
        } else if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            continue;
        } else {
            return 0;
        }
    }

    if (o == 0 || bit_count != 0) {
        return 0;
    }

    out[o] = '\0';
    return 1;
}

static char morse_lookup(const char *token) {
    static const char *codes[] = {
        ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--",
        "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",
        ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", "-----"
    };
    static const char values[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'
    };

    int n = (int)(sizeof(values) / sizeof(values[0]));
    for (int i = 0; i < n; ++i) {
        if (str_eq(token, codes[i])) {
            return values[i];
        }
    }
    return '?';
}

static int decode_morse_to_buf(const char *in, char *out, int out_len) {
    char token[16];
    int t = 0;
    int o = 0;
    int wrote = 0;

    for (int i = 0;; ++i) {
        char c = in[i];
        int end = (c == '\0');

        if (c == '.' || c == '-') {
            if (t < (int)sizeof(token) - 1) {
                token[t++] = c;
            }
            continue;
        }

        if (t > 0) {
            token[t] = '\0';
            if (o < out_len - 1) {
                out[o++] = morse_lookup(token);
                wrote = 1;
            }
            t = 0;
        }

        if (c == '/' && o < out_len - 1) {
            out[o++] = ' ';
            wrote = 1;
        }

        if (end) {
            break;
        }
    }

    out[o] = '\0';
    return wrote;
}

static int decode_bacon_to_buf(const char *in, char *out, int out_len) {
    char clean[512];
    int n = 0;

    for (int i = 0; in[i] != '\0' && n < (int)sizeof(clean) - 1; ++i) {
        char c = in[i];
        if (c >= 'a' && c <= 'z') {
            c = (char)(c - ('a' - 'A'));
        }
        if (c == 'A' || c == 'B') {
            clean[n++] = c;
        }
    }
    clean[n] = '\0';

    if (n < 5) {
        return 0;
    }

    int o = 0;
    int wrote = 0;
    for (int i = 0; i + 4 < n && o < out_len - 1; i += 5) {
        int val = 0;
        for (int j = 0; j < 5; ++j) {
            val <<= 1;
            if (clean[i + j] == 'B') {
                val |= 1;
            }
        }
        if (val >= 0 && val < 26) {
            out[o++] = (char)('A' + val);
        } else {
            out[o++] = '?';
        }
        wrote = 1;
    }

    out[o] = '\0';
    return wrote;
}

static int decode_railfence_to_buf(const char *in, int rails, char *out, int out_len) {
    int n = str_len(in);
    if (n <= 0 || rails <= 1 || rails > 8 || n >= 512) {
        return 0;
    }

    static uint8_t mark[8][512];
    for (int r = 0; r < rails; ++r) {
        for (int c = 0; c < n; ++c) {
            mark[r][c] = 0;
        }
    }

    int row = 0;
    int dir = 1;
    for (int col = 0; col < n; ++col) {
        mark[row][col] = 1;
        if (row == 0) {
            dir = 1;
        } else if (row == rails - 1) {
            dir = -1;
        }
        row += dir;
    }

    char grid[8][512];
    for (int r = 0; r < rails; ++r) {
        for (int c = 0; c < n; ++c) {
            grid[r][c] = 0;
        }
    }

    int idx = 0;
    for (int r = 0; r < rails; ++r) {
        for (int c = 0; c < n; ++c) {
            if (mark[r][c] && idx < n) {
                grid[r][c] = in[idx++];
            }
        }
    }

    row = 0;
    dir = 1;
    int o = 0;
    for (int col = 0; col < n && o < out_len - 1; ++col) {
        out[o++] = grid[row][col];
        if (row == 0) {
            dir = 1;
        } else if (row == rails - 1) {
            dir = -1;
        }
        row += dir;
    }
    out[o] = '\0';
    return 1;
}

static void decode_atbash_to_buf(const char *in, char *out, int out_len) {
    int o = 0;
    for (int i = 0; in[i] != '\0' && o < out_len - 1; ++i) {
        char c = in[i];
        if (c >= 'A' && c <= 'Z') {
            out[o++] = (char)('Z' - (c - 'A'));
        } else if (c >= 'a' && c <= 'z') {
            out[o++] = (char)('z' - (c - 'a'));
        } else {
            out[o++] = c;
        }
    }
    out[o] = '\0';
}

static void to_lower_copy(const char *src, char *dst, int max_len) {
    int i = 0;
    while (src[i] != '\0' && i < max_len - 1) {
        char c = src[i];
        if (c >= 'A' && c <= 'Z') {
            dst[i] = (char)(c + ('a' - 'A'));
        } else {
            dst[i] = c;
        }
        ++i;
    }
    dst[i] = '\0';
}

static int is_number_str(const char *s) {
    if (*s == '\0') {
        return 0;
    }
    for (int i = 0; s[i] != '\0'; ++i) {
        if (s[i] < '0' || s[i] > '9') {
            return 0;
        }
    }
    return 1;
}

static void decrypt_caesar_to_buf(const char *in, int shift, char *out, int out_len) {
    int o = 0;
    int norm_shift = shift % 26;

    for (int i = 0; in[i] != '\0' && o < out_len - 1; ++i) {
        char c = in[i];
        if (c >= 'A' && c <= 'Z') {
            int p = (c - 'A' - norm_shift + 26) % 26;
            out[o++] = (char)('A' + p);
        } else if (c >= 'a' && c <= 'z') {
            int p = (c - 'a' - norm_shift + 26) % 26;
            out[o++] = (char)('a' + p);
        } else {
            out[o++] = c;
        }
    }
    out[o] = '\0';
}

static void decrypt_vigenere_to_buf(const char *in, const char *key, char *out, int out_len) {
    int o = 0;
    int k = 0;
    int key_len = str_len(key);

    if (key_len <= 0) {
        out[0] = '\0';
        return;
    }

    for (int i = 0; in[i] != '\0' && o < out_len - 1; ++i) {
        char c = in[i];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            char kc = key[k % key_len];
            if (kc >= 'a' && kc <= 'z') {
                kc = (char)(kc - ('a' - 'A'));
            }
            int shift = (kc >= 'A' && kc <= 'Z') ? (kc - 'A') : 0;

            if (c >= 'A' && c <= 'Z') {
                out[o++] = (char)('A' + ((c - 'A' - shift + 26) % 26));
            } else {
                out[o++] = (char)('a' + ((c - 'a' - shift + 26) % 26));
            }
            k++;
        } else {
            out[o++] = c;
        }
    }
    out[o] = '\0';
}

static int is_hex_text(const char *s) {
    int n = str_len(s);
    if (n <= 0) {
        return 0;
    }
    for (int i = 0; i < n; ++i) {
        if (hex_val(s[i]) < 0) {
            return 0;
        }
    }
    return 1;
}

static uint32_t rotr32(uint32_t x, uint32_t n) {
    return (x >> n) | (x << (32 - n));
}

static void sha256_to_hex(const char *input, char *out_hex, int out_hex_len) {
    static const uint32_t k[64] = {
        0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
        0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
        0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
        0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
        0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
        0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
        0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
        0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
    };

    uint32_t h[8] = {
        0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
        0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19
    };

    uint8_t msg[1024];
    int n = str_len(input);
    if (n > 960) {
        n = 960;
    }

    int mlen = 0;
    for (int i = 0; i < n; ++i) {
        msg[mlen++] = (uint8_t)input[i];
    }
    msg[mlen++] = 0x80;
    while ((mlen % 64) != 56) {
        msg[mlen++] = 0x00;
    }

    uint64_t bitlen = (uint64_t)n * 8;
    for (int i = 7; i >= 0; --i) {
        msg[mlen++] = (uint8_t)((bitlen >> (i * 8)) & 0xFF);
    }

    for (int off = 0; off < mlen; off += 64) {
        uint32_t w[64];
        for (int i = 0; i < 16; ++i) {
            int j = off + (i * 4);
            w[i] = ((uint32_t)msg[j] << 24) | ((uint32_t)msg[j + 1] << 16) |
                   ((uint32_t)msg[j + 2] << 8) | (uint32_t)msg[j + 3];
        }
        for (int i = 16; i < 64; ++i) {
            uint32_t s0 = rotr32(w[i - 15], 7) ^ rotr32(w[i - 15], 18) ^ (w[i - 15] >> 3);
            uint32_t s1 = rotr32(w[i - 2], 17) ^ rotr32(w[i - 2], 19) ^ (w[i - 2] >> 10);
            w[i] = w[i - 16] + s0 + w[i - 7] + s1;
        }

        uint32_t a = h[0], b = h[1], c = h[2], d = h[3];
        uint32_t e = h[4], f = h[5], g = h[6], hh = h[7];

        for (int i = 0; i < 64; ++i) {
            uint32_t S1 = rotr32(e, 6) ^ rotr32(e, 11) ^ rotr32(e, 25);
            uint32_t ch = (e & f) ^ ((~e) & g);
            uint32_t temp1 = hh + S1 + ch + k[i] + w[i];
            uint32_t S0 = rotr32(a, 2) ^ rotr32(a, 13) ^ rotr32(a, 22);
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t temp2 = S0 + maj;

            hh = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        h[0] += a; h[1] += b; h[2] += c; h[3] += d;
        h[4] += e; h[5] += f; h[6] += g; h[7] += hh;
    }

    static const char hex[] = "0123456789abcdef";
    int p = 0;
    for (int i = 0; i < 8 && p + 8 < out_hex_len; ++i) {
        for (int s = 28; s >= 0; s -= 4) {
            out_hex[p++] = hex[(h[i] >> s) & 0xF];
        }
    }
    out_hex[p] = '\0';
}

static int looks_like_english_text(const char *text) {
    char lower[512];
    to_lower_copy(text, lower, 512);

    if (strncmp(lower, "you", 3) == 0 || strncmp(lower, "this", 4) == 0) {
        return 1;
    }

    return str_contains(lower, " the ") || str_contains(lower, " you ") || str_contains(lower, " this ");
}

static int file_create(const char *path);
static int file_find(const char *path);
static int dir_find(const char *path);
static void resolve_path(const char *input, char *out);
static int is_direct_child(const char *parent, const char *child);
static int is_root_path(const char *path);
static const char *base_name(const char *path);

static void mission_apply_actions(const struct mission_action *actions, int count) {
    for (int i = 0; i < count; ++i) {
        int idx;
        if (!file_create(actions[i].path)) {
            continue;
        }
        idx = file_find(actions[i].path);
        if (idx >= 0) {
            str_copy(fs_files[idx].content, actions[i].content, 512);
        }
    }
}

static void mission_start_current(void) {
    if (mission_current < 0 || mission_current >= mission_count) {
        return;
    }
    mission_apply_actions(missions[mission_current].on_start, missions[mission_current].on_start_count);
}

static int mission_command_allowed(const char *cmd) {
    const struct mission_def *m;
    if (mission_current < 0 || mission_current >= mission_count) {
        return 1;
    }
    m = &missions[mission_current];
    for (int i = 0; i < m->command_count; ++i) {
        if (str_eq(cmd, m->commands[i])) {
            return 1;
        }
    }
    return 0;
}

static void mission_print_available_commands(void) {
    const struct mission_def *m;
    if (mission_current < 0 || mission_current >= mission_count) {
        return;
    }
    m = &missions[mission_current];
    puts("Comandos liberados: ");
    for (int i = 0; i < m->command_count; ++i) {
        puts(m->commands[i]);
        if (i + 1 < m->command_count) {
            puts(", ");
        }
    }
    putchar('\n');
}

static int starts_with(const char *text, const char *prefix) {
    int n = str_len(prefix);
    if (n == 0) {
        return 1;
    }
    return strncmp(text, prefix, n) == 0;
}

static void redraw_prompt_with_buffer(const char *buffer) {
    putchar('\n');
    puts("INITIUM:");
    puts(current_path);
    puts("$ ");
    puts(buffer);
}

static int path_autocomplete(char *buffer, int length, int max_length, int token_start) {
    char token[128];
    char fixed_prefix[128];
    char parent_input[128];
    char parent_abs[128];
    char name_prefix[128];
    const char *match_names[64];
    char suggestions[64][128];

    int token_len = length - token_start;
    int last_slash = -1;
    int fixed_len = 0;
    int prefix_len = 0;
    int match_count = 0;
    int common_len = 0;
    int single_is_dir = 0;
    int single_name_len = 0;

    if (token_len <= 0 || token_len >= (int)sizeof(token)) {
        return length;
    }

    for (int i = 0; i < token_len; ++i) {
        token[i] = buffer[token_start + i];
    }
    token[token_len] = '\0';

    for (int i = token_len - 1; i >= 0; --i) {
        if (token[i] == '/') {
            last_slash = i;
            break;
        }
    }

    if (last_slash >= 0) {
        fixed_len = last_slash + 1;
        for (int i = 0; i < fixed_len && i < (int)sizeof(fixed_prefix) - 1; ++i) {
            fixed_prefix[i] = token[i];
        }
        fixed_prefix[fixed_len] = '\0';

        if (last_slash == 0) {
            str_copy(parent_input, "/", 128);
        } else {
            for (int i = 0; i < last_slash && i < (int)sizeof(parent_input) - 1; ++i) {
                parent_input[i] = token[i];
            }
            parent_input[last_slash] = '\0';
        }

        str_copy(name_prefix, token + fixed_len, 128);
        resolve_path(parent_input, parent_abs);
    } else {
        fixed_prefix[0] = '\0';
        str_copy(name_prefix, token, 128);
        str_copy(parent_abs, current_path, 128);
    }

    prefix_len = str_len(name_prefix);
    if (dir_find(parent_abs) < 0) {
        return length;
    }

    for (int pass = 0; pass < 2; ++pass) {
        int limit = (pass == 0) ? MAX_DIRS : MAX_FILES;
        for (int i = 0; i < limit; ++i) {
            int used = (pass == 0) ? fs_dirs[i].used : fs_files[i].used;
            const char *path = (pass == 0) ? fs_dirs[i].path : fs_files[i].path;
            const char *name;

            if (!used || (pass == 0 && is_root_path(path))) {
                continue;
            }
            if (!is_direct_child(parent_abs, path)) {
                continue;
            }

            name = base_name(path);
            if (!starts_with(name, name_prefix)) {
                continue;
            }

            if (match_count < (int)(sizeof(match_names) / sizeof(match_names[0]))) {
                int si = 0;
                int sj = 0;
                while (fixed_prefix[si] != '\0' && sj < (int)sizeof(suggestions[0]) - 1) {
                    suggestions[match_count][sj++] = fixed_prefix[si++];
                }
                for (int k = 0; name[k] != '\0' && sj < (int)sizeof(suggestions[0]) - 1; ++k) {
                    suggestions[match_count][sj++] = name[k];
                }
                if (pass == 0 && sj < (int)sizeof(suggestions[0]) - 1) {
                    suggestions[match_count][sj++] = '/';
                }
                suggestions[match_count][sj] = '\0';
                match_names[match_count] = name;
                match_count++;
            }

            if (match_count == 1) {
                common_len = str_len(name);
                single_is_dir = (pass == 0);
                single_name_len = common_len;
            } else {
                int j = 0;
                while (j < common_len && name[j] == match_names[0][j] && name[j] != '\0') {
                    ++j;
                }
                common_len = j;
            }
        }
    }

    if (match_count == 0) {
        return length;
    }

    int new_len = length;
    if (common_len > prefix_len) {
        int target_len = token_start + fixed_len + common_len;
        if (target_len >= max_length) {
            target_len = max_length - 1;
        }

        int idx = token_start;
        for (int i = 0; i < fixed_len && idx < target_len; ++i) {
            buffer[idx++] = fixed_prefix[i];
        }
        for (int i = 0; i < common_len && idx < target_len; ++i) {
            buffer[idx++] = match_names[0][i];
        }
        new_len = idx;
        buffer[new_len] = '\0';

        for (int i = length; i < new_len; ++i) {
            putchar(buffer[i]);
        }
    }

    if (match_count == 1) {
        if (single_is_dir) {
            int full_dir_len = token_start + fixed_len + single_name_len;
            if (new_len == full_dir_len && new_len < max_length - 1) {
                buffer[new_len++] = '/';
                buffer[new_len] = '\0';
                putchar('/');
            }
        } else if (new_len < max_length - 1) {
            buffer[new_len++] = ' ';
            buffer[new_len] = '\0';
            putchar(' ');
        }
        return new_len;
    }

    if (new_len > length) {
        return new_len;
    }

    putchar('\n');
    for (int i = 0; i < match_count; ++i) {
        puts(suggestions[i]);
        if (i + 1 < match_count) {
            puts("  ");
        }
    }
    redraw_prompt_with_buffer(buffer);
    return new_len;
}

static int command_autocomplete(char *buffer, int length, int max_length) {
    if (length <= 0) {
        return length;
    }

    int token_start = length;
    while (token_start > 0 && buffer[token_start - 1] != ' ' && buffer[token_start - 1] != '\t') {
        --token_start;
    }

    if (token_start > 0) {
        return path_autocomplete(buffer, length, max_length, token_start);
    }

    const char *matches[64];
    int match_count = 0;
    int first_len = 0;
    int common_len = 0;

    int total_cmds = (int)(sizeof(all_shell_commands) / sizeof(all_shell_commands[0]));
    for (int i = 0; i < total_cmds; ++i) {
        const char *cmd = all_shell_commands[i];
        if (!mission_command_allowed(cmd) && !str_eq(cmd, "exit")) {
            continue;
        }
        if (!starts_with(cmd, buffer)) {
            continue;
        }

        if (match_count < (int)(sizeof(matches) / sizeof(matches[0]))) {
            matches[match_count++] = cmd;
        }

        if (match_count == 1) {
            first_len = str_len(cmd);
            common_len = first_len;
        } else {
            int j = 0;
            while (j < common_len && cmd[j] == matches[0][j] && cmd[j] != '\0') {
                ++j;
            }
            common_len = j;
        }
    }

    if (match_count == 0) {
        return length;
    }

    int new_len = length;
    if (common_len > length) {
        while (new_len < common_len && new_len < max_length - 1) {
            buffer[new_len] = matches[0][new_len];
            putchar(buffer[new_len]);
            ++new_len;
        }
        buffer[new_len] = '\0';
    }

    if (match_count == 1) {
        if (new_len == first_len && new_len < max_length - 1) {
            buffer[new_len++] = ' ';
            putchar(' ');
            buffer[new_len] = '\0';
        }
        return new_len;
    }

    if (new_len > length) {
        return new_len;
    }

    putchar('\n');
    for (int i = 0; i < match_count; ++i) {
        puts(matches[i]);
        if (i + 1 < match_count) {
            puts("  ");
        }
    }
    redraw_prompt_with_buffer(buffer);
    return new_len;
}

static void mission_print_hints(int only_index) {
    const struct mission_def *m;
    if (mission_current < 0 || mission_current >= mission_count) {
        puts("Sem missao ativa.\n");
        return;
    }
    m = &missions[mission_current];

    if (only_index > 0) {
        int idx = only_index - 1;
        if (idx < 0 || idx >= m->hint_count) {
            puts("hint invalido\n");
            return;
        }
        puts("Hint ");
        print_uint((uint32_t)only_index);
        puts(": ");
        puts(m->hints[idx]);
        putchar('\n');
        return;
    }

    for (int i = 0; i < m->hint_count; ++i) {
        puts("Hint ");
        print_uint((uint32_t)(i + 1));
        puts(": ");
        puts(m->hints[i]);
        putchar('\n');
    }
}

static int mission_submit_flag(const char *flag, char *msg, int msg_len) {
    const struct mission_def *m;
    if (mission_current < 0 || mission_current >= mission_count) {
        str_copy(msg, "todas as missoes concluidas", msg_len);
        return 0;
    }

    m = &missions[mission_current];
    if (!str_eq(flag, m->flag)) {
        str_copy(msg, "flag incorreta", msg_len);
        return 0;
    }

    mission_score += m->points;
    mission_apply_actions(m->on_complete, m->on_complete_count);
    mission_current++;

    if (mission_current < mission_count) {
        mission_start_current();
        str_copy(msg, "flag correta! nova missao desbloqueada", msg_len);
    } else {
        str_copy(msg, "parabens, todas as missoes concluidas", msg_len);
    }

    return 1;
}

static int is_root_path(const char *path) {
    return path[0] == '/' && path[1] == '\0';
}

static void trim_trailing_slash(char *path) {
    int len = str_len(path);
    while (len > 1 && path[len - 1] == '/') {
        path[len - 1] = '\0';
        --len;
    }
}

static const char *base_name(const char *path) {
    const char *p = path;
    const char *last = path;
    while (*p) {
        if (*p == '/') {
            last = p + 1;
        }
        ++p;
    }
    return last;
}

static void parent_path(const char *path, char *out) {
    if (is_root_path(path)) {
        str_copy(out, "/", 128);
        return;
    }

    str_copy(out, path, 128);
    trim_trailing_slash(out);
    int len = str_len(out);
    while (len > 1 && out[len - 1] != '/') {
        out[len - 1] = '\0';
        --len;
    }
    if (len > 1 && out[len - 1] == '/') {
        out[len - 1] = '\0';
    }
    if (out[0] == '\0') {
        str_copy(out, "/", 128);
    }
}

static void resolve_path(const char *input, char *out) {
    if (input == 0 || *input == '\0') {
        str_copy(out, current_path, 128);
        return;
    }

    if (strcmp(input, ".") == 0) {
        str_copy(out, current_path, 128);
        return;
    }

    if (strcmp(input, "..") == 0) {
        parent_path(current_path, out);
        return;
    }

    if (input[0] == '/') {
        str_copy(out, input, 128);
    } else {
        if (is_root_path(current_path)) {
            str_copy(out, "/", 128);
            str_concat(out, input, 128);
        } else {
            str_copy(out, current_path, 128);
            str_concat(out, "/", 128);
            str_concat(out, input, 128);
        }
    }

    trim_trailing_slash(out);
}

static int dir_find(const char *path) {
    for (int i = 0; i < MAX_DIRS; ++i) {
        if (fs_dirs[i].used && strcmp(fs_dirs[i].path, path) == 0) {
            return i;
        }
    }
    return -1;
}

static int file_find(const char *path) {
    for (int i = 0; i < MAX_FILES; ++i) {
        if (fs_files[i].used && strcmp(fs_files[i].path, path) == 0) {
            return i;
        }
    }
    return -1;
}

static int is_direct_child(const char *parent, const char *child) {
    int p_len = str_len(parent);

    if (is_root_path(parent)) {
        if (child[0] != '/' || child[1] == '\0') {
            return 0;
        }
        for (int i = 1; child[i] != '\0'; ++i) {
            if (child[i] == '/') {
                return 0;
            }
        }
        return 1;
    }

    if (strncmp(child, parent, p_len) != 0 || child[p_len] != '/') {
        return 0;
    }

    for (int i = p_len + 1; child[i] != '\0'; ++i) {
        if (child[i] == '/') {
            return 0;
        }
    }

    return child[p_len + 1] != '\0';
}

static int str_contains(const char *text, const char *pattern) {
    if (*pattern == '\0') {
        return 1;
    }

    for (int i = 0; text[i] != '\0'; ++i) {
        int j = 0;
        while (pattern[j] != '\0' && text[i + j] == pattern[j]) {
            ++j;
        }
        if (pattern[j] == '\0') {
            return 1;
        }
    }
    return 0;
}

static void history_add(const char *line) {
    if (line[0] == '\0') {
        return;
    }

    if (history_count < MAX_HISTORY) {
        str_copy(command_history[history_count], line, 128);
        history_count++;
        return;
    }

    for (int i = 1; i < MAX_HISTORY; ++i) {
        str_copy(command_history[i - 1], command_history[i], 128);
    }
    str_copy(command_history[MAX_HISTORY - 1], line, 128);
}

static void print_tree(const char *dir, int depth) {
    for (int i = 0; i < MAX_DIRS; ++i) {
        if (fs_dirs[i].used && !is_root_path(fs_dirs[i].path) && is_direct_child(dir, fs_dirs[i].path)) {
            for (int d = 0; d < depth; ++d) {
                puts("  ");
            }
            puts("|- ");
            puts(base_name(fs_dirs[i].path));
            puts("/\n");
            print_tree(fs_dirs[i].path, depth + 1);
        }
    }

    for (int i = 0; i < MAX_FILES; ++i) {
        if (fs_files[i].used && is_direct_child(dir, fs_files[i].path)) {
            for (int d = 0; d < depth; ++d) {
                puts("  ");
            }
            puts("|- ");
            puts(base_name(fs_files[i].path));
            putchar('\n');
        }
    }
}

static int dir_create(const char *path) {
    if (dir_find(path) >= 0) {
        return 0;
    }

    for (int i = 0; i < MAX_DIRS; ++i) {
        if (!fs_dirs[i].used) {
            fs_dirs[i].used = 1;
            str_copy(fs_dirs[i].path, path, 128);
            return 1;
        }
    }
    return 0;
}

static int file_create(const char *path) {
    if (file_find(path) >= 0) {
        return 1;
    }

    for (int i = 0; i < MAX_FILES; ++i) {
        if (!fs_files[i].used) {
            fs_files[i].used = 1;
            str_copy(fs_files[i].path, path, 128);
            fs_files[i].content[0] = '\0';
            return 1;
        }
    }
    return 0;
}

static int dir_is_empty(const char *path) {
    for (int i = 0; i < MAX_DIRS; ++i) {
        if (fs_dirs[i].used && is_direct_child(path, fs_dirs[i].path)) {
            return 0;
        }
    }
    for (int i = 0; i < MAX_FILES; ++i) {
        if (fs_files[i].used && is_direct_child(path, fs_files[i].path)) {
            return 0;
        }
    }
    return 1;
}

static void fs_init(void) {
    for (int i = 0; i < MAX_DIRS; ++i) {
        fs_dirs[i].used = 0;
    }
    for (int i = 0; i < MAX_FILES; ++i) {
        fs_files[i].used = 0;
    }

    dir_create("/");
    dir_create("/home");
    dir_create("/home/guest");
    dir_create("/tmp");
    dir_create("/var");
    dir_create("/var/log");
    dir_create("/usr");
    dir_create("/usr/bin");
    dir_create("/root");

    file_create("/README.txt");
    file_create("/notes.txt");
    int readme = file_find("/README.txt");
    int notes = file_find("/notes.txt");
    if (readme >= 0) {
        str_copy(fs_files[readme].content, "INITIUM-OS shell minimal em execucao.\n", 512);
    }
    if (notes >= 0) {
        str_copy(fs_files[notes].content, "Use help para listar comandos basicos.\n", 512);
    }

    history_count = 0;
    log_count = 0;
    users_init();
    mission_current = 0;
    mission_score = 0;
    mission_start_current();
    log_event("SYSTEM BOOT");
}

static void clear_screen(void) {
    uint16_t blank = ' ' | (default_color << 8);
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i) {
        VGA[i] = blank;
    }
    cursor_pos = 0;
    serial_puts_raw("\x1b[2J\x1b[3J\x1b[H\x1b[0m");
    print_boot_logo();
    cursor_pos = UI_HEADER_LINES * VGA_WIDTH;
}

static void system_poweroff(void) {
    puts("desligando sistema...\n");
    serial_puts_raw("BOOT: poweroff request\n");

    __asm__ volatile ("cli");

    /* QEMU/Bochs ACPI */
    outw(0x604, 0x2000);
    /* Legacy APM/QEMU variants */
    outw(0xB004, 0x2000);
    outw(0x4004, 0x3400);
    outw(0x600, 0x34);
    /* QEMU debug-exit (works when enabled by VM args). */
    outb(0xF4, 0x00);

    /* If poweroff did not happen, halt CPU indefinitely. */
    for (;;) {
        __asm__ __volatile__("hlt");
    }
}

static void print_boot_logo(void) {
    puts("+------------------------------------------------------------------------------+\n");
    puts("| o--o  o--o                      INITIUM OS                        o--o  o--o |\n");
    puts("|                                                                              |\n");
    puts("|                         .----------------------------.                       |\n");
    puts("|                         |            >_              |                       |\n");
    puts("|                         '----------------------------'                       |\n");
    puts("|                                                                              |\n");
    puts("|          S E C U R E   C T F   K E R N E L   E N V I R O N M E N T           |\n");
    puts("+------------------------------------------------------------------------------+\n\n");
}

static void set_gdt_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt_entries[index].base_low = base & 0xFFFF;
    gdt_entries[index].base_middle = (base >> 16) & 0xFF;
    gdt_entries[index].base_high = (base >> 24) & 0xFF;
    gdt_entries[index].limit_low = limit & 0xFFFF;
    gdt_entries[index].granularity = (limit >> 16) & 0x0F;
    gdt_entries[index].granularity |= granularity & 0xF0;
    gdt_entries[index].access = access;
}

static void init_gdt(void) {
    set_gdt_entry(0, 0, 0, 0, 0);
    set_gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    set_gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    gdt_ptr.limit = sizeof(gdt_entries) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    __asm__ volatile ("lgdt (%0)" :: "r"(&gdt_ptr));
    __asm__ volatile (
        "mov $0x10, %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        "ljmp $0x08, $1f\n"
        "1:\n"
        ::: "ax"
    );
}

static void set_idt_gate(int n, uint32_t handler, uint16_t sel, uint8_t flags) {
    idt_entries[n].base_lo = handler & 0xFFFF;
    idt_entries[n].sel = sel;
    idt_entries[n].always0 = 0;
    idt_entries[n].flags = flags;
    idt_entries[n].base_hi = (handler >> 16) & 0xFFFF;
}

static void load_idt(void) {
    idt_ptr.limit = sizeof(idt_entries) - 1;
    idt_ptr.base = (uint32_t)&idt_entries;
    __asm__ volatile ("lidtl (%0)" :: "r"(&idt_ptr));
}

static void pic_remap(void) {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0xFC);
    outb(0xA1, 0xFF);
}

static void init_pit(void) {
    uint16_t divisor = 1193; /* ~100Hz */
    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
}

static char scancode_map[128] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/', 0, '*', 0, ' ',
};

static volatile char keyboard_buffer[256];
static volatile uint8_t kb_head = 0;
static volatile uint8_t kb_tail = 0;
static volatile uint32_t timer_ticks = 0;
static volatile uint32_t worker_cycles = 0;

#define TASK_COUNT 3
#define TASK_STACK_SIZE 1024
uint32_t task_esp[TASK_COUNT];
uint32_t task_stack[TASK_COUNT][TASK_STACK_SIZE];
uint32_t current_task = 0;

static void create_task(int index, void (*entry)(void)) {
    uint32_t *stack = task_stack[index] + TASK_STACK_SIZE;

    *--stack = 0x202;
    *--stack = 0x08;
    *--stack = (uint32_t)entry;

    *--stack = 0; /* EAX */
    *--stack = 0; /* ECX */
    *--stack = 0; /* EDX */
    *--stack = 0; /* EBX */
    *--stack = 0; /* ESP dummy */
    *--stack = 0; /* EBP */
    *--stack = 0; /* ESI */
    *--stack = 0; /* EDI */

    task_esp[index] = (uint32_t)stack;
}

static void idle_task(void) {
    for (;;) {
        __asm__ volatile ("hlt");
    }
}

static void worker_task(void) {
    for (;;) {
        worker_cycles++;
    }
}

static void task_init(void) {
    current_task = 0;
    create_task(1, idle_task);
    create_task(2, worker_task);
}

uint32_t schedule(uint32_t current_esp) {
    task_esp[current_task] = current_esp;
    current_task = (current_task + 1) % TASK_COUNT;
    return task_esp[current_task];
}

uint32_t syscall_handler(uint32_t *regs) {
    uint32_t syscall_no = regs[7];

    if (syscall_no == 1) {
        return timer_ticks;
    }

    if (syscall_no == 2) {
        const char *msg = (const char *)regs[4];
        if (msg) {
            puts(msg);
        }
        return 0;
    }

    return (uint32_t)-1;
}

void timer_handler(void) {
    timer_ticks++;
}

void keyboard_handler(void) {
    uint8_t scancode = inb(0x60);
    if (scancode & 0x80) {
        return;
    }

    if (scancode < sizeof(scancode_map)) {
        char c = scancode_map[scancode];
        if (c) {
            keyboard_buffer[kb_head] = c;
            kb_head = (kb_head + 1) & 0xFF;
        }
    }
}

static char input_read_char(void) {
    for (;;) {
        if (kb_tail != kb_head) {
            char c = keyboard_buffer[kb_tail];
            kb_tail = (kb_tail + 1) & 0xFF;
            return c;
        }

        if (serial_has_data()) {
            char c = (char)inb(SERIAL_PORT);
            if (c == '\r') {
                return '\n';
            }
            if (c == 0x7f) {
                return '\b';
            }
            return c;
        }

        __asm__ volatile ("hlt");
    }
}

static void read_line(char *buffer, int max_length) {
    int length = 0;
    buffer[0] = '\0';

    while (length < max_length - 1) {
        char c = input_read_char();

        if (c == '\n') {
            putchar(c);
            break;
        }

        if (c == '\b') {
            if (length > 0) {
                --length;
                buffer[length] = '\0';
                putchar('\b');
                putchar(' ');
                putchar('\b');
            }
            continue;
        }

        if (c == '\t') {
            length = command_autocomplete(buffer, length, max_length);
            continue;
        }

        putchar(c);
        buffer[length++] = c;
        buffer[length] = '\0';
    }

    buffer[length] = '\0';
}

static void shell(void) {
    char line[128];
    int sudo_restore_user = -1;

    puts("Bem-vindo ao INITIUM-OS shell.\n");
    puts("Digite 'help' para ver os comandos.\n\n");

    while (1) {
        if (sudo_restore_user >= 0) {
            current_user = sudo_restore_user;
            sudo_restore_user = -1;
        }

        puts("INITIUM:");
        puts(current_path);
        puts("$ ");
        read_line(line, sizeof(line));
        history_add(line);

reparse_line:
        char cmd_name[32];
        int ci = 0;
        const char *cp = skip_spaces(line);
        while (*cp != '\0' && *cp != ' ' && *cp != '\t' && ci < 31) {
            cmd_name[ci++] = *cp++;
        }
        cmd_name[ci] = '\0';

        if (cmd_name[0] == '\0') {
            continue;
        }

        if (!mission_command_allowed(cmd_name) && !str_eq(cmd_name, "exit")) {
            puts("Comando bloqueado neste nivel.\n");
            mission_print_available_commands();
            continue;
        }

        if (strcmp(line, "exit") == 0) {
            puts("Saindo do shell...\n");
            break;
        }

        if (strcmp(line, "help") == 0) {
            if (mission_current < mission_count) {
                puts("Missao atual: ");
                print_uint((uint32_t)missions[mission_current].id);
                puts(" - ");
                puts(missions[mission_current].name);
                putchar('\n');
                puts(missions[mission_current].description);
                putchar('\n');
                puts("Pontos da missao: ");
                print_uint((uint32_t)missions[mission_current].points);
                putchar('\n');
            } else {
                puts("Missao atual: concluido\n");
            }
            puts("Pontuacao total: ");
            print_uint((uint32_t)mission_score);
            putchar('\n');
            mission_print_available_commands();
            puts("Comandos disponiveis:\n");
            puts("  help  - Mostrar esta ajuda\n");
            puts("  clear - Limpar a tela\n");
            puts("  echo  - Imprimir texto\n");
            puts("  pwd   - Diretorio atual\n");
            puts("  ls    - Listar conteudo (ls [dir])\n");
            puts("  cd    - Trocar diretorio (cd <dir>)\n");
            puts("  cat   - Mostrar arquivo (cat <arquivo>)\n");
            puts("  touch - Criar arquivo vazio\n");
            puts("  mkdir - Criar diretorio\n");
            puts("  rmdir - Remover diretorio vazio\n");
            puts("  rm    - Remover arquivo\n");
            puts("  write - Escrever arquivo (write <arq> <txt>)\n");
            puts("  append- Anexar texto (append <arq> <txt>)\n");
            puts("  cp    - Copiar arquivo (cp <src> <dst>)\n");
            puts("  mv    - Renomear arquivo (mv <src> <dst>)\n");
            puts("  history - Historico de comandos\n");
            puts("  find  - Buscar nome (find <termo>)\n");
            puts("  tree  - Arvore de diretorios\n");
            puts("  login - Entrar com usuario/senha\n");
            puts("  logout- Voltar para guest\n");
            puts("  passwd- Alterar senha (passwd <u> <p>)\n");
            puts("  su    - Trocar usuario (su <u>)\n");
            puts("  sudo  - Executar com privilegio\n");
            puts("  who   - Sessao ativa\n");
            puts("  whoami- Usuario atual\n");
            puts("  uname - Info do sistema\n");
            puts("  ps    - Processos do sistema\n");
            puts("  logread - Ler logs do sistema\n");
            puts("  manual - Ajuda rapida de comando\n");
            puts("  decode/decrypt - Decodificacao e decriptacao\n");
            puts("  hash  - Hash generate/analyze\n");
            puts("  stego - Esteganografia (extract)\n");
            puts("  analyze - Analise de arquivo para pistas\n");
            puts("  exploit - Explorar script vulneravel\n");
            puts("  submit - Enviar flag\n");
            puts("  ticks - Mostrar contagem de ticks do PIT\n");
            puts("  tasks - Mostrar estado do scheduler\n");
            puts("  syscall - Testar int 0x80\n");
            puts("  hint  - Mostrar dicas da missao (hint [n])\n");
            puts("  shutdown - Encerrar shell\n");
            puts("  exit  - Sair do shell\n");
            continue;
        }

        if (strncmp(line, "hint", 4) == 0) {
            const char *arg = skip_spaces(line + 4);
            int idx = 0;
            if (*arg >= '0' && *arg <= '9') {
                while (*arg >= '0' && *arg <= '9') {
                    idx = idx * 10 + (*arg - '0');
                    ++arg;
                }
            }
            mission_print_hints(idx);
            continue;
        }

        if (strcmp(line, "clear") == 0) {
            clear_screen();
            continue;
        }

        if (strcmp(line, "ticks") == 0) {
            print_uint(timer_ticks);
            putchar('\n');
            continue;
        }

        if (strcmp(line, "pwd") == 0) {
            puts(current_path);
            putchar('\n');
            continue;
        }

        if (strncmp(line, "ls", 2) == 0) {
            char target[128];
            const char *arg = skip_spaces(line + 2);

            if (*arg == '\0') {
                str_copy(target, current_path, 128);
            } else {
                resolve_path(arg, target);
            }

            if (dir_find(target) < 0) {
                puts("diretorio nao encontrado\n");
                continue;
            }

            for (int i = 0; i < MAX_DIRS; ++i) {
                if (fs_dirs[i].used && !is_root_path(fs_dirs[i].path) && is_direct_child(target, fs_dirs[i].path)) {
                    puts(base_name(fs_dirs[i].path));
                    puts("/\n");
                }
            }
            for (int i = 0; i < MAX_FILES; ++i) {
                if (fs_files[i].used && is_direct_child(target, fs_files[i].path)) {
                    puts(base_name(fs_files[i].path));
                    putchar('\n');
                }
            }
            continue;
        }

        if (strncmp(line, "cd", 2) == 0) {
            char target[128];
            const char *arg = skip_spaces(line + 2);
            if (*arg == '\0') {
                str_copy(current_path, "/", 128);
                continue;
            }

            resolve_path(arg, target);
            if (dir_find(target) >= 0) {
                str_copy(current_path, target, 128);
            } else {
                puts("diretorio nao encontrado\n");
            }
            continue;
        }

        if (strncmp(line, "login", 5) == 0) {
            char u[32];
            char p[32];
            if (!parse_two_tokens(line + 5, u, 32, p, 32)) {
                puts("uso: login <usuario> <senha>\n");
                continue;
            }
            int idx = user_find(u);
            if (idx >= 0 && str_eq(users[idx].pass, p)) {
                current_user = idx;
                puts("login ok\n");
                log_event("LOGIN OK");
            } else {
                puts("credenciais invalidas\n");
                log_event("LOGIN FAIL");
            }
            continue;
        }

        if (strcmp(line, "logout") == 0) {
            int guest = user_find("guest");
            if (guest >= 0) {
                current_user = guest;
            }
            puts("logout ok\n");
            log_event("LOGOUT");
            continue;
        }

        if (str_eq(cmd_name, "su")) {
            char u[32];
            char p[32];
            const char *arg = skip_spaces(line + 2);
            if (!parse_two_tokens(arg, u, 32, p, 32)) {
                puts("Usage: su <username> <password>\n");
                continue;
            }

            if (str_eq(users[current_user].name, u)) {
                puts("Already logged in as that user\n");
                continue;
            }

            int idx = user_find(u);
            if (idx >= 0 && str_eq(users[idx].pass, p)) {
                current_user = idx;
                puts("Switched to ");
                puts(u);
                putchar('\n');
            } else {
                puts("Authentication failed\n");
            }
            continue;
        }

        if (strncmp(line, "passwd", 6) == 0) {
            char a[32], b[32], c[32];
            int ai = 0, bi = 0, ci2 = 0;
            const char *pcur = skip_spaces(line + 6);

            while (*pcur != '\0' && *pcur != ' ' && *pcur != '\t' && ai < 31) {
                a[ai++] = *pcur++;
            }
            a[ai] = '\0';
            pcur = skip_spaces(pcur);
            while (*pcur != '\0' && *pcur != ' ' && *pcur != '\t' && bi < 31) {
                b[bi++] = *pcur++;
            }
            b[bi] = '\0';
            pcur = skip_spaces(pcur);
            while (*pcur != '\0' && *pcur != ' ' && *pcur != '\t' && ci2 < 31) {
                c[ci2++] = *pcur++;
            }
            c[ci2] = '\0';

            if (a[0] == '\0' || b[0] == '\0') {
                puts("Usage: passwd <current> <new> <confirm>\n");
                puts("   or: passwd <user> <new> (admin)\n");
                continue;
            }

            if (c[0] != '\0') {
                if (!str_eq(users[current_user].pass, a)) {
                    puts("Current password incorrect\n");
                    continue;
                }
                if (!str_eq(b, c)) {
                    puts("Passwords do not match\n");
                    continue;
                }
                str_copy(users[current_user].pass, b, 32);
                puts("Password changed successfully\n");
                log_event("PASSWD CHANGE SELF");
                continue;
            }

            if (!users[current_user].is_admin) {
                puts("permission denied\n");
                continue;
            }

            int idx = user_find(a);
            if (idx < 0) {
                puts("usuario inexistente\n");
                continue;
            }
            str_copy(users[idx].pass, b, 32);
            puts("senha atualizada\n");
            log_event("PASSWD CHANGE ADMIN");
            continue;
        }

        if (strncmp(line, "sudo", 4) == 0) {
            const char *cmd = skip_spaces(line + 4);
            if (!users[current_user].is_admin) {
                puts("sudo: usuario sem privilegios\n");
                continue;
            }
            if (*cmd == '\0') {
                puts("uso: sudo <comando>\n");
            } else {
                int root_idx = user_find("root");
                if (root_idx < 0) {
                    puts("sudo: root indisponivel\n");
                    continue;
                }

                sudo_restore_user = current_user;
                current_user = root_idx;
                str_copy(line, cmd, sizeof(line));
                history_add(line);
                goto reparse_line;
            }
            continue;
        }

        if (strcmp(line, "who") == 0) {
            puts(users[current_user].name);
            puts(" ttyS0\n");
            continue;
        }

        if (strcmp(line, "whoami") == 0) {
            puts(users[current_user].name);
            putchar('\n');
            continue;
        }

        if (strcmp(line, "uname") == 0) {
            puts("INITIUM-OS 0.1 i386\n");
            continue;
        }

        if (strncmp(line, "echo", 4) == 0) {
            const char *msg = skip_spaces(line + 4);
            puts(msg);
            putchar('\n');
            continue;
        }

        if (strncmp(line, "cat", 3) == 0) {
            char path[128];
            const char *name = skip_spaces(line + 3);
            if (*name == '\0') {
                puts("uso: cat <nome_arquivo>\n");
                continue;
            }

            resolve_path(name, path);
            int idx = file_find(path);
            if (idx < 0) {
                puts("arquivo nao encontrado\n");
                continue;
            }

            puts(fs_files[idx].content);
            continue;
        }

        if (strncmp(line, "touch", 5) == 0) {
            char path[128];
            char parent[128];
            const char *name = skip_spaces(line + 5);
            if (*name == '\0') {
                puts("uso: touch <arquivo>\n");
                continue;
            }
            resolve_path(name, path);
            parent_path(path, parent);
            if (dir_find(parent) < 0) {
                puts("diretorio pai inexistente\n");
                continue;
            }
            if (!file_create(path)) {
                puts("nao foi possivel criar arquivo\n");
            }
            continue;
        }

        if (strncmp(line, "mkdir", 5) == 0) {
            char path[128];
            char parent[128];
            const char *name = skip_spaces(line + 5);
            if (*name == '\0') {
                puts("uso: mkdir <diretorio>\n");
                continue;
            }
            resolve_path(name, path);
            parent_path(path, parent);
            if (dir_find(parent) < 0) {
                puts("diretorio pai inexistente\n");
                continue;
            }
            if (!dir_create(path)) {
                puts("nao foi possivel criar diretorio\n");
            }
            continue;
        }

        if (strncmp(line, "rmdir", 5) == 0) {
            char path[128];
            const char *name = skip_spaces(line + 5);
            if (*name == '\0') {
                puts("uso: rmdir <diretorio>\n");
                continue;
            }
            resolve_path(name, path);
            int idx = dir_find(path);
            if (idx < 0 || is_root_path(path)) {
                puts("diretorio invalido\n");
                continue;
            }
            if (!dir_is_empty(path)) {
                puts("diretorio nao vazio\n");
                continue;
            }
            fs_dirs[idx].used = 0;
            continue;
        }

        if (strncmp(line, "rm", 2) == 0) {
            char path[128];
            const char *name = skip_spaces(line + 2);
            if (*name == '\0') {
                puts("uso: rm <arquivo>\n");
                continue;
            }
            resolve_path(name, path);
            int idx = file_find(path);
            if (idx < 0) {
                puts("arquivo nao encontrado\n");
                continue;
            }
            fs_files[idx].used = 0;
            continue;
        }

        if (strncmp(line, "write", 5) == 0 || strncmp(line, "append", 6) == 0) {
            int is_append = (line[0] == 'a');
            const char *cursor = skip_spaces(line + (is_append ? 6 : 5));
            char name[128];
            char path[128];
            char parent[128];
            int n = 0;

            while (*cursor != '\0' && *cursor != ' ' && *cursor != '\t' && n < 127) {
                name[n++] = *cursor++;
            }
            name[n] = '\0';
            cursor = skip_spaces(cursor);

            if (name[0] == '\0' || *cursor == '\0') {
                puts(is_append ? "uso: append <arquivo> <texto>\n" : "uso: write <arquivo> <texto>\n");
                continue;
            }

            resolve_path(name, path);
            parent_path(path, parent);
            if (dir_find(parent) < 0) {
                puts("diretorio pai inexistente\n");
                continue;
            }

            if (!file_create(path)) {
                puts("nao foi possivel criar arquivo\n");
                continue;
            }

            int idx = file_find(path);
            if (!is_append) {
                str_copy(fs_files[idx].content, cursor, 512);
            } else {
                if (str_len(fs_files[idx].content) > 0) {
                    str_concat(fs_files[idx].content, "\n", 512);
                }
                str_concat(fs_files[idx].content, cursor, 512);
            }
            continue;
        }

        if (strncmp(line, "cp", 2) == 0 || strncmp(line, "mv", 2) == 0) {
            int is_move = (line[0] == 'm');
            const char *cursor = skip_spaces(line + 2);
            char src_name[128];
            char dst_name[128];
            char src[128];
            char dst[128];
            char dst_parent[128];
            int a = 0;
            int b = 0;

            while (*cursor != '\0' && *cursor != ' ' && *cursor != '\t' && a < 127) {
                src_name[a++] = *cursor++;
            }
            src_name[a] = '\0';
            cursor = skip_spaces(cursor);
            while (*cursor != '\0' && *cursor != ' ' && *cursor != '\t' && b < 127) {
                dst_name[b++] = *cursor++;
            }
            dst_name[b] = '\0';

            if (src_name[0] == '\0' || dst_name[0] == '\0') {
                puts(is_move ? "uso: mv <origem> <destino>\n" : "uso: cp <origem> <destino>\n");
                continue;
            }

            resolve_path(src_name, src);
            resolve_path(dst_name, dst);
            int src_idx = file_find(src);
            if (src_idx < 0) {
                puts("arquivo origem nao encontrado\n");
                continue;
            }

            parent_path(dst, dst_parent);
            if (dir_find(dst_parent) < 0) {
                puts("diretorio destino inexistente\n");
                continue;
            }

            if (!file_create(dst)) {
                puts("nao foi possivel criar destino\n");
                continue;
            }

            int dst_idx = file_find(dst);
            str_copy(fs_files[dst_idx].content, fs_files[src_idx].content, 512);
            if (is_move) {
                fs_files[src_idx].used = 0;
            }
            continue;
        }

        if (strcmp(line, "history") == 0) {
            for (int i = 0; i < history_count; ++i) {
                print_uint((uint32_t)(i + 1));
                puts("  ");
                puts(command_history[i]);
                putchar('\n');
            }
            continue;
        }

        if (strncmp(line, "find", 4) == 0) {
            const char *term = skip_spaces(line + 4);
            int found = 0;

            if (*term == '\0') {
                puts("uso: find <termo>\n");
                continue;
            }

            for (int i = 0; i < MAX_DIRS; ++i) {
                if (fs_dirs[i].used && !is_root_path(fs_dirs[i].path) && str_contains(base_name(fs_dirs[i].path), term)) {
                    puts(fs_dirs[i].path);
                    puts("/\n");
                    found = 1;
                }
            }

            for (int i = 0; i < MAX_FILES; ++i) {
                if (fs_files[i].used && str_contains(base_name(fs_files[i].path), term)) {
                    puts(fs_files[i].path);
                    putchar('\n');
                    found = 1;
                }
            }

            if (!found) {
                puts("nenhum resultado\n");
            }
            continue;
        }

        if (strncmp(line, "tree", 4) == 0) {
            char target[128];
            const char *arg = skip_spaces(line + 4);
            if (*arg == '\0') {
                str_copy(target, current_path, 128);
            } else {
                resolve_path(arg, target);
            }

            if (dir_find(target) < 0) {
                puts("diretorio nao encontrado\n");
                continue;
            }

            puts(target);
            puts("\n");
            print_tree(target, 0);
            continue;
        }

        if (strcmp(line, "ps") == 0) {
            puts("PID  NAME\n");
            puts("1    kernel\n");
            puts("2    shell\n");
            puts("3    worker\n");
            continue;
        }

        if (strncmp(line, "logread", 7) == 0) {
            const char *arg = skip_spaces(line + 7);

            if (log_count == 0) {
                puts("No logs available.\n");
                continue;
            }

            if (*arg == '\0') {
                for (int i = 0; i < log_count; ++i) {
                    puts(system_logs[i]);
                    putchar('\n');
                }
                continue;
            }

            int only_digits = 1;
            for (int i = 0; arg[i] != '\0'; ++i) {
                if (arg[i] < '0' || arg[i] > '9') {
                    only_digits = 0;
                    break;
                }
            }

            if (only_digits) {
                int n = 0;
                for (int i = 0; arg[i] != '\0'; ++i) {
                    n = n * 10 + (arg[i] - '0');
                }
                if (n <= 0) {
                    puts("No matching logs.\n");
                    continue;
                }
                int start = log_count - n;
                if (start < 0) {
                    start = 0;
                }
                for (int i = start; i < log_count; ++i) {
                    puts(system_logs[i]);
                    putchar('\n');
                }
                continue;
            }

            int found = 0;
            for (int i = 0; i < log_count; ++i) {
                char lower_line[128];
                char lower_arg[64];
                to_lower_copy(system_logs[i], lower_line, 128);
                to_lower_copy(arg, lower_arg, 64);
                if (str_contains(lower_line, lower_arg)) {
                    puts(system_logs[i]);
                    putchar('\n');
                    found = 1;
                }
            }
            if (!found) {
                puts("No matching logs.\n");
            }
            continue;
        }

        if (strncmp(line, "manual", 6) == 0) {
            const char *arg = skip_spaces(line + 6);
            if (*arg == '\0') {
                puts("=== CICADA-3301 Command Manual ===\n");
                puts("cat     Syntax: cat <arquivo>\n");
                puts("cd      Syntax: cd <diretorio>\n");
                puts("clear   Syntax: clear\n");
                puts("decode  Syntax: decode <tipo> <conteudo> | decode <arquivo>\n");
                puts("decrypt Syntax: decrypt <arquivo> | decrypt <caesar|vigenere> <arquivo> <chave>\n");
                puts("hash    Syntax: hash <generate|analyze> <texto|arquivo>\n");
                puts("help    Syntax: help\n");
                puts("history Syntax: history\n");
                puts("login   Syntax: login <usuario> <senha>\n");
                puts("logout  Syntax: logout\n");
                puts("logread Syntax: logread [n_linhas|filtro]\n");
                puts("ls      Syntax: ls [diretorio]\n");
                puts("manual  Syntax: manual [comando]\n");
                puts("passwd  Syntax: passwd <usuario> <nova_senha>\n");
                puts("ps      Syntax: ps\n");
                puts("pwd     Syntax: pwd\n");
                puts("shutdown Syntax: shutdown\n");
                puts("stego   Syntax: stego extract <imagem>\n");
                puts("su      Syntax: su <usuario>\n");
                puts("submit  Syntax: submit <flag>\n");
                puts("sudo    Syntax: sudo <comando>\n");
                puts("who     Syntax: who\n");
                puts("whoami  Syntax: whoami\n");
            } else {
                puts("Command: ");
                puts(arg);
                puts("\nUse 'help' para lista de comandos e contexto da missao.\n");
            }
            continue;
        }

        if (strncmp(line, "decode", 6) == 0 || strncmp(line, "decrypt", 7) == 0) {
            int is_decrypt_cmd = str_eq(cmd_name, "decrypt");
            int cmd_len = is_decrypt_cmd ? 7 : 6;
            const char *arg = skip_spaces(line + cmd_len);
            char out[512];
            char path[128];
            const char *input = arg;

            if (*arg == '\0') {
                if (is_decrypt_cmd) {
                    puts("uso: decrypt <texto|arquivo> | decrypt <shift> <texto|arquivo>\n");
                    puts("     decrypt caesar <arquivo> <shift>\n");
                    puts("     decrypt vigenere <arquivo> <chave>\n");
                } else {
                    puts("uso: decode <texto|arquivo>\n");
                    puts("     decode <auto|base64|hex|binary|morse|atbash|railfence|bacon> <conteudo|arquivo>\n");
                }
                continue;
            }

            if (is_decrypt_cmd) {
                char tok1[32];
                char tok2[128];
                char tok3[32];
                int a = 0;
                int b = 0;
                int c = 0;
                const char *cursor = arg;

                while (*cursor != '\0' && *cursor != ' ' && *cursor != '\t' && a < (int)sizeof(tok1) - 1) {
                    tok1[a++] = *cursor++;
                }
                tok1[a] = '\0';
                cursor = skip_spaces(cursor);

                while (*cursor != '\0' && *cursor != ' ' && *cursor != '\t' && b < (int)sizeof(tok2) - 1) {
                    tok2[b++] = *cursor++;
                }
                tok2[b] = '\0';
                cursor = skip_spaces(cursor);

                while (*cursor != '\0' && *cursor != ' ' && *cursor != '\t' && c < (int)sizeof(tok3) - 1) {
                    tok3[c++] = *cursor++;
                }
                tok3[c] = '\0';

                if (str_eq(tok1, "caesar")) {
                    if (tok2[0] == '\0' || tok3[0] == '\0' || !is_number_str(tok3)) {
                        puts("uso: decrypt caesar <arquivo> <shift>\n");
                        continue;
                    }

                    resolve_path(tok2, path);
                    int fidx = file_find(path);
                    if (fidx < 0) {
                        puts("arquivo nao encontrado\n");
                        continue;
                    }

                    int shift = 0;
                    for (int i = 0; tok3[i] != '\0'; ++i) {
                        shift = shift * 10 + (tok3[i] - '0');
                    }
                    decrypt_caesar_to_buf(fs_files[fidx].content, shift, out, 512);
                    puts(out);
                    putchar('\n');
                    continue;
                }

                if (str_eq(tok1, "vigenere")) {
                    if (tok2[0] == '\0' || tok3[0] == '\0') {
                        puts("uso: decrypt vigenere <arquivo> <chave>\n");
                        continue;
                    }

                    resolve_path(tok2, path);
                    int fidx = file_find(path);
                    if (fidx < 0) {
                        puts("arquivo nao encontrado\n");
                        continue;
                    }

                    decrypt_vigenere_to_buf(fs_files[fidx].content, tok3, out, 512);
                    puts(out);
                    putchar('\n');
                    continue;
                }

                if (is_number_str(tok1)) {
                    int shift = 0;
                    for (int i = 0; tok1[i] != '\0'; ++i) {
                        shift = shift * 10 + (tok1[i] - '0');
                    }

                    const char *target = skip_spaces(arg + str_len(tok1));
                    if (*target == '\0') {
                        puts("uso: decrypt <shift> <texto|arquivo>\n");
                        continue;
                    }

                    resolve_path(target, path);
                    int fidx = file_find(path);
                    input = (fidx >= 0) ? fs_files[fidx].content : target;
                    decrypt_caesar_to_buf(input, shift, out, 512);
                    puts(out);
                    putchar('\n');
                    continue;
                }

                resolve_path(arg, path);
                int fidx = file_find(path);
                input = (fidx >= 0) ? fs_files[fidx].content : arg;

                int found = 0;
                for (int shift = 1; shift <= 25; ++shift) {
                    decrypt_caesar_to_buf(input, shift, out, 512);
                    if (looks_like_english_text(out)) {
                        puts("[CAESAR shift=");
                        print_uint((uint32_t)shift);
                        puts("] ");
                        puts(out);
                        putchar('\n');
                        found = 1;
                        break;
                    }
                }

                if (!found) {
                    puts("Nao foi possivel detectar shift automaticamente.\n");
                    puts("Tente: decrypt <shift> <texto|arquivo>\n");
                }
                continue;
            }

            char mode[16];
            int m = 0;
            const char *cursor = arg;
            while (*cursor != '\0' && *cursor != ' ' && *cursor != '\t' && m < (int)sizeof(mode) - 1) {
                mode[m++] = *cursor++;
            }
            mode[m] = '\0';
            const char *rest = skip_spaces(cursor);

            int explicit_mode = str_eq(mode, "auto") || str_eq(mode, "base64") ||
                                str_eq(mode, "hex") || str_eq(mode, "binary") ||
                                str_eq(mode, "morse") || str_eq(mode, "atbash") ||
                                str_eq(mode, "railfence") || str_eq(mode, "bacon");

            if (explicit_mode) {
                if (*rest == '\0') {
                    puts("uso: decode <auto|base64|hex|binary|morse|atbash|railfence|bacon> <conteudo|arquivo>\n");
                    continue;
                }

                resolve_path(rest, path);
                int fidx = file_find(path);
                input = (fidx >= 0) ? fs_files[fidx].content : rest;
            } else {
                resolve_path(arg, path);
                int fidx = file_find(path);
                input = (fidx >= 0) ? fs_files[fidx].content : arg;
                str_copy(mode, "auto", 16);
            }

            if (str_eq(mode, "base64")) {
                if (decode_base64_to_buf(input, out, 512)) {
                    puts(out);
                    putchar('\n');
                } else {
                    puts("entrada base64 invalida\n");
                }
                continue;
            }

            if (str_eq(mode, "hex")) {
                if (decode_hex_to_buf(input, out, 512)) {
                    puts(out);
                    putchar('\n');
                } else {
                    puts("entrada hex invalida\n");
                }
                continue;
            }

            if (str_eq(mode, "binary")) {
                if (decode_binary_to_buf(input, out, 512)) {
                    puts(out);
                    putchar('\n');
                } else {
                    puts("entrada binary invalida\n");
                }
                continue;
            }

            if (str_eq(mode, "atbash")) {
                decode_atbash_to_buf(input, out, 512);
                puts(out);
                putchar('\n');
                continue;
            }

            if (str_eq(mode, "morse")) {
                if (decode_morse_to_buf(input, out, 512)) {
                    puts(out);
                    putchar('\n');
                } else {
                    puts("entrada morse invalida\n");
                }
                continue;
            }

            if (str_eq(mode, "railfence")) {
                if (decode_railfence_to_buf(input, 3, out, 512)) {
                    puts(out);
                    putchar('\n');
                } else {
                    puts("entrada railfence invalida\n");
                }
                continue;
            }

            if (str_eq(mode, "bacon")) {
                if (decode_bacon_to_buf(input, out, 512)) {
                    puts(out);
                    putchar('\n');
                } else {
                    puts("entrada bacon invalida\n");
                }
                continue;
            }

            if (decode_base64_to_buf(input, out, 512)) {
                puts("[BASE64] ");
                puts(out);
                putchar('\n');
                continue;
            }

            if (decode_hex_to_buf(input, out, 512)) {
                puts("[HEX] ");
                puts(out);
                putchar('\n');
                continue;
            }

            if (decode_binary_to_buf(input, out, 512)) {
                puts("[BINARY] ");
                puts(out);
                putchar('\n');
                continue;
            }

            if (decode_morse_to_buf(input, out, 512) && !str_eq(out, input)) {
                puts("[MORSE] ");
                puts(out);
                putchar('\n');
                continue;
            }

            decode_atbash_to_buf(input, out, 512);
            if (!str_eq(out, input)) {
                puts("[ATBASH] ");
                puts(out);
                putchar('\n');
                continue;
            }

            if (decode_railfence_to_buf(input, 3, out, 512) && !str_eq(out, input)) {
                puts("[RAILFENCE] ");
                puts(out);
                putchar('\n');
                continue;
            }

            if (decode_bacon_to_buf(input, out, 512) && !str_eq(out, input)) {
                puts("[BACON] ");
                puts(out);
                putchar('\n');
                continue;
            }

            puts("Nao foi possivel decodificar automaticamente.\n");
            puts("Dica: para cifra de Cesar use 'decrypt <texto|arquivo>'.\n");
            continue;
        }

        if (strncmp(line, "hash", 4) == 0) {
            const char *arg = skip_spaces(line + 4);
            char mode[16];
            int m = 0;
            while (arg[m] != '\0' && arg[m] != ' ' && arg[m] != '\t' && m < (int)sizeof(mode) - 1) {
                mode[m] = arg[m];
                m++;
            }
            mode[m] = '\0';
            const char *value = skip_spaces(arg + m);

            if (mode[0] == '\0' || value[0] == '\0') {
                puts("Usage: hash <generate | analyze> <text | file>\n");
                continue;
            }

            char path[128];
            resolve_path(value, path);
            int idx = file_find(path);
            const char *content = (idx >= 0) ? fs_files[idx].content : value;

            if (str_eq(mode, "generate")) {
                char sha_hex[65];
                sha256_to_hex(content, sha_hex, 65);
                puts("sha256: ");
                puts(sha_hex);
                putchar('\n');
                continue;
            }

            if (str_eq(mode, "analyze")) {
                char lower[512];
                to_lower_copy(content, lower, 512);
                if (str_len(lower) == 64 && is_hex_text(lower)) {
                    puts("Identified hash type: SHA256\n");
                } else {
                    puts("Identified hash type: Unknown\n");
                }
                continue;
            }

            puts("Invalid mode. Use 'generate' or 'analyze'.\n");
            continue;
        }

        if (strncmp(line, "stego", 5) == 0) {
            const char *arg = skip_spaces(line + 5);
            char mode[16];
            char name[128];
            int mi = 0;

            while (*arg != '\0' && *arg != ' ' && *arg != '\t' && mi < (int)sizeof(mode) - 1) {
                mode[mi++] = *arg++;
            }
            mode[mi] = '\0';
            arg = skip_spaces(arg);

            int ni = 0;
            while (*arg != '\0' && *arg != ' ' && *arg != '\t' && ni < (int)sizeof(name) - 1) {
                name[ni++] = *arg++;
            }
            name[ni] = '\0';

            if (mode[0] == '\0' || name[0] == '\0') {
                puts("Usage: stego extract <image>\n");
                continue;
            }

            if (!str_eq(mode, "extract")) {
                puts("Invalid mode. Use 'extract'.\n");
                continue;
            }

            char path[128];
            resolve_path(name, path);
            int idx = file_find(path);
            if (idx < 0) {
                puts("File not found: ");
                puts(name);
                putchar('\n');
                continue;
            }

            const char *content = fs_files[idx].content;
            if (starts_with(content, "STEGOMSG:")) {
                puts(content + 9);
                putchar('\n');
            } else {
                puts("No hidden message found.\n");
            }
            continue;
        }

        if (strncmp(line, "analyze", 7) == 0) {
            const char *arg = skip_spaces(line + 7);
            if (*arg == '\0') {
                puts("Usage: analyze <file>\n");
                continue;
            }

            char path[128];
            resolve_path(arg, path);
            int idx = file_find(path);
            if (idx < 0) {
                puts("File not found: ");
                puts(arg);
                putchar('\n');
                continue;
            }

            char lower[512];
            to_lower_copy(fs_files[idx].content, lower, 512);

            if (str_contains(lower, "failed login root") && str_contains(lower, "successful login analyst")) {
                puts("Hidden directory unlocked: /usr/bin\n");
                puts("The suspicious log entry points to /usr/bin/backup.sh\n");
                if (file_create("/usr/bin/backup.sh")) {
                    int bidx = file_find("/usr/bin/backup.sh");
                    if (bidx >= 0) {
                        str_copy(fs_files[bidx].content, "#!/bin/bash\necho 'Running backup...'\n", 512);
                    }
                }
            } else {
                puts("No obvious hidden clues found.\n");
            }
            continue;
        }

        if (strncmp(line, "exploit", 7) == 0) {
            const char *arg = skip_spaces(line + 7);
            if (*arg == '\0') {
                puts("Usage: exploit <script>\n");
                continue;
            }

            char target[128];
            str_copy(target, arg, 128);
            int tlen = str_len(target);
            for (int i = tlen - 1; i >= 0; --i) {
                if (target[i] == '/') {
                    str_copy(target, target + i + 1, 128);
                    break;
                }
            }

            if (!str_eq(target, "backup.sh")) {
                puts("Exploit failed. Vulnerable script not found.\n");
                continue;
            }

            int root_idx = user_find("root");
            if (current_user != root_idx) {
                puts("Exploit failed. Root privileges are required.\n");
                continue;
            }

            file_create("/root/final.key");
            int fidx = file_find("/root/final.key");
            if (fidx >= 0) {
                str_copy(fs_files[fidx].content,
                         "-----BEGIN MESSAGE-----\nV2VsY29tZSB0byB0aGUgaW5uZXIgY2lyY2xlLg==\n-----END MESSAGE-----\n",
                         512);
            }

            puts("Privilege escalation successful. You are now root.\n");
            puts("A final root-only key file has been created at /root/final.key\n");
            log_event("EXPLOIT SUCCESS");

            if (sudo_restore_user >= 0) {
                sudo_restore_user = -1;
            }
            continue;
        }

        if (strncmp(line, "submit", 6) == 0) {
            const char *flag = skip_spaces(line + 6);
            char msg[96];
            if (*flag == '\0') {
                puts("uso: submit <flag>\n");
                continue;
            }

            if (mission_submit_flag(flag, msg, 96)) {
                puts(msg);
                putchar('\n');
                puts("Pontuacao total: ");
                print_uint((uint32_t)mission_score);
                putchar('\n');
                log_event("FLAG ACCEPTED");
            } else {
                puts(msg);
                putchar('\n');
            }
            continue;
        }

        if (strcmp(line, "shutdown") == 0) {
            system_poweroff();
            continue;
        }

        if (strcmp(line, "tasks") == 0) {
            puts("task_atual=");
            print_uint(current_task);
            puts(" worker_cycles=");
            print_uint(worker_cycles);
            putchar('\n');
            continue;
        }

        if (strcmp(line, "syscall") == 0) {
            uint32_t ticks_via_syscall;
            const char *sys_msg = "Mensagem via syscall 0x80\n";

            __asm__ volatile ("int $0x80" : : "a"(2), "b"(sys_msg) : "memory");
            __asm__ volatile ("int $0x80" : "=a"(ticks_via_syscall) : "a"(1) : "memory");

            puts("retorno_syscall_ticks=");
            print_uint(ticks_via_syscall);
            putchar('\n');
            continue;
        }

        if (line[0] != '\0') {
            puts("Comando nao encontrado: ");
            puts(line);
            puts("\n");
        }
    }
}

void kernel_main(void) {
    serial_init();
    serial_puts_raw("BOOT:0 enter kernel_main\n");
    fs_init();
    serial_puts_raw("BOOT:0.5 fs ok\n");

    init_gdt();
    serial_puts_raw("BOOT:1 gdt ok\n");

    pic_remap();
    serial_puts_raw("BOOT:2 pic ok\n");

    task_init();
    serial_puts_raw("BOOT:3 tasks ok\n");

    set_idt_gate(32, (uint32_t)irq0_handler, 0x08, 0x8E);
    set_idt_gate(33, (uint32_t)irq1_handler, 0x08, 0x8E);
    set_idt_gate(128, (uint32_t)isr80_handler, 0x08, 0x8E);
    load_idt();
    serial_puts_raw("BOOT:4 idt ok\n");

    init_pit();
    serial_puts_raw("BOOT:5 pit ok\n");

    __asm__ volatile ("sti");
    serial_puts_raw("BOOT:6 sti on\n");

    clear_screen();
    puts("INITIUM-OS kernel carregado.\n");
    puts("Kernel minimo com PIT, IDT e teclado por IRQ.\n\n");
    serial_puts_raw("BOOT:7 shell start\n");
    shell();

    system_poweroff();
}
