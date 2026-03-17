from commands.base_command import BaseCommand
from security.decoder import Decoder

class DecodeCommand(BaseCommand):

    name = "decode"

    def __init__(self):
        self.decoder = Decoder()

    def execute(self, terminal, args):

        if len(args) == 0:
            print("Usage: decode <file>")
            return
        
        content = terminal.vfs.read_file(args[0])

        if not content:
            print("File not found or empty.")
            return
        
        result = self.decoder.auto_decode(content)
        print(result)