from commands.base_command import BaseCommand
from security.stego_engine import StegoEngine

class StegoCommand(BaseCommand):

    name = "stego"

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

        path = f"assets/{filename}"

        result = self.engine.extract_lsb(path)

        print(result)