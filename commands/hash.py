"""Module for the hash component of the CICADA-3301 application."""

from commands.base_command import BaseCommand
from security.hash_engine import HashEngine

class HashCommand(BaseCommand):
    """Command implementation for the hash command."""

    name = "hash"
    description = "Compute hash of text"
    usage = "hash <text>"

    autocomplete = { 1: ["generate", "analyze"]}

    def __init__(self):
        """Initialize the object state."""
        self.engine = HashEngine()

    def execute(self, terminal, args):
        """Execute the operation for this component."""

        if len(args) < 2:
            print("Usage: hash <generate |analyze> <file>")
            return
        
        mode = args[0]
        filename = args[1]

        content = terminal.vfs.read_file(filename, terminal.auth.get_current_user())

        if content is None:
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
