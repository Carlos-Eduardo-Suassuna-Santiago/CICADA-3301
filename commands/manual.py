from commands.base_command import BaseCommand

class ManualCommand(BaseCommand):

    name = "manual"
    description = "Display detailed manual for all commands"
    usage = "manual"

    def execute(self, terminal, args):
        print("\n=== CICADA-3301 Command Manual ===\n")

        # Lista de exemplos práticos para cada comando
        examples = {
            "cat": "cat /home/guest/file.txt",
            "cd": "cd /home/guest",
            "clear": "clear",
            "decode": "decode /home/guest/encoded.txt",
            "decrypt": "decrypt caesar /home/guest/cipher.txt 3",
            "hash": "hash generate /home/guest/file.txt",
            "help": "help ls",
            "history": "history",
            "login": "login (automatic on startup)",
            "logout": "logout",
            "logread": "logread",
            "ls": "ls /home",
            "passwd": "passwd",
            "ps": "ps",
            "pwd": "pwd",
            "shutdown": "shutdown",
            "stego": "stego extract /home/guest/image.png",
            "su": "su admin",
            "submit": "submit flag{example_flag}",
            "sudo": "sudo cat /etc/secret",
            "who": "who",
            "whoami": "whoami",
            "manual": "manual"
        }

        for cmd_name, cmd in terminal.commands.items():
            print(f"Command: {cmd.name}")
            print(f"Description: {cmd.description}")
            print(f"Syntax: {cmd.usage}")
            print(f"Example: {examples.get(cmd_name, 'N/A')}")
            print("-" * 50)