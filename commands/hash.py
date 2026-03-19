from commands.base_command import BaseCommand
from security.hash_engine import HashEngine

class HashCommand(BaseCommand):

    name = "hash"
    description = "Compute hash of text"
    usage = "hash <text>"

    def __init__(self):
        self.engine = HashEngine()

    def execute(self, terminal, args):

        if len(args) < 2:
            print("Usage: hash <generate |analyze> <file>")
            return
        
        mode = args[0]
        filename = args[1]

        content = terminal.vfs.read_file(filename)

        if not content:
            print(f"File not found: {filename}")
            return

        if mode == "generate":
            
            hashes = self.engine.generate(content)

            for k, v in hashes.items():
                print(f"{k}: {v}")

        elif mode == "analyze":

            result = self.engine.identify(content)
            print(f"Identified hash type: {result}")
        else:
            print("Invalid mode. Use 'generate' or 'analyze'.")