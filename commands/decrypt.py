from commands.base_command import BaseCommand
from security.cripto_engine import CryptoEngine

class DecryptCommand(BaseCommand):

    name = "decrypt"
    description = "Decrypt encrypted text"
    usage = "decrypt <text>"

    def __init__(self):
        self.engine = CryptoEngine()

    def execute(self, terminal, args):

        if len(args) < 2:
            print("Usage: decrypt <cesar | vigenere> <file> [key]")
            return

        mode = args[0]
        file_name = args[1]

        content = terminal.vfs.read_file(file_name)

        if not content:
            print(f"File '{file_name}' not found.")
            return

        if mode == "caesar":
            if len(args) < 3:
                print("Usage: decrypt cesar <file> <shift>")
                return
            
            try:
                shift = int(args[2])
            except ValueError:
                print("Shift must be a number.")
                return
            result = self.engine.decrypt_caesar(content, shift)
            print(f"Decrypted content:\n{result}")

        elif mode == "vigenere":

            if len(args) < 3:
                print("Usage: decrypt vigenere <file> <key>")
                return
            
            key = args[2]
            result = self.engine.decrypt_vigenere(content, key)
            print(f"Decrypted content:\n{result}")

        else:
            print("Invalid mode. Use 'cesar' or 'vigenere'.")