from commands.base_command import BaseCommand
from security.cripto_engine import CryptoEngine

class DecryptCommand(BaseCommand):

    name = "decrypt"
    description = "Decrypt encrypted text"
    usage = "decrypt <text>"

    autocomplete = { 1: ["cesar", "vigenere"]}

    def __init__(self):
        self.engine = CryptoEngine()

    def execute(self, terminal, args):

        if len(args) == 0:
            print("Usage: decrypt <file> | decrypt <caesar|vigenere> <file> [key|shift]")
            return

        if len(args) == 1:
            file_name = args[0]
            content = terminal.vfs.read_file(file_name, terminal.auth.get_current_user())

            if not content:
                print(f"File '{file_name}' not found.")
                return False

            for shift in range(1, 26):
                candidate = self.engine.decrypt_caesar(content, shift)
                lower = candidate.lower()
                if " the " in lower or lower.startswith("you") or lower.startswith("this"):
                    print(f"Decrypted content (shift {shift}):\n{candidate}")
                    return True

            print("Could not automatically detect Caesar shift. Try 'decrypt <file> <shift>'.")
            return False

        mode = args[0]

        if mode.isdigit() and len(args) == 2:
            file_name = args[1]
            try:
                shift = int(mode)
            except ValueError:
                print("Shift must be a number.")
                return False

            content = terminal.vfs.read_file(file_name, terminal.auth.get_current_user())
            if not content:
                print(f"File '{file_name}' not found.")
                return False

            result = self.engine.decrypt_caesar(content, shift)
            print(f"Decrypted content:\n{result}")
            return True

        file_name = args[1] if len(args) > 1 else None
        content = terminal.vfs.read_file(file_name, terminal.auth.get_current_user())

        if not content:
            print(f"File '{file_name}' not found.")
            return False

        if mode == "caesar":
            if len(args) < 3:
                print("Usage: decrypt caesar <file> <shift>")
                return False
            
            try:
                shift = int(args[2])
            except ValueError:
                print("Shift must be a number.")
                return False
            result = self.engine.decrypt_caesar(content, shift)
            print(f"Decrypted content:\n{result}")
            return True

        elif mode == "vigenere":
            if len(args) < 3:
                print("Usage: decrypt vigenere <file> <key>")
                return False
            
            key = args[2]
            result = self.engine.decrypt_vigenere(content, key)
            print(f"Decrypted content:\n{result}")
            return True

        else:
            print("Invalid mode. Use 'caesar' or 'vigenere'.")
            return False