from commands.base_command import BaseCommand
from security.stego_engine import StegoEngine
import os

class StegoCommand(BaseCommand):

    name = "stego"
    description = "Steganography operations"
    usage = "stego <operation> <args>"

    def __init__(self):
        self.engine = StegoEngine()

    def execute(self, terminal, args):

        if len(args) < 2:
            print("Usage: stego extract <image>")
            return
        
        mode = args[0]
        filename = args[1]

        if mode != "extract":
            print("Invalid mode. Use 'extract'.")
            return

        filename = os.path.basename(filename)
        if ".." in filename or "/" in filename or "\\" in filename:
            print("Invalid filename.")
            return

        path = f"assets/{filename}"

        result = self.engine.extract_lsb(path)

        print(result)