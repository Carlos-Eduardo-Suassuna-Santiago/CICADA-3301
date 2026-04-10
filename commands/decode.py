from commands.base_command import BaseCommand
from security.decoder import Decoder

class DecodeCommand(BaseCommand):

    name = "decode"
    description = "Decode encoded text"
    usage = "decode <type> <content> or decode <file>"

    def __init__(self):
        self.decoder = Decoder()

    def execute(self, terminal, args):

        if len(args) < 1:
            print("Usage: decode <type> <content> or decode <file>")
            print("Types: base64, hex, binary, morse, atbash, railfence, bacon, auto")
            return
        
        # If only one argument, try to read from file and auto-decode
        if len(args) == 1:
            content = terminal.vfs.read_file(args[0], terminal.auth.get_current_user())
            if content is None:
                print("File not found or empty.")
                return
            result = self.decoder.auto_decode(content)
            print(result)
            return
        
        # If two or more arguments, first is type, rest is content
        decode_type = args[0].lower()
        content = ' '.join(args[1:])
        
        try:
            if decode_type == "base64":
                result = self.decoder.decode_base64(content)
            elif decode_type == "hex":
                result = self.decoder.decode_hex(content)
            elif decode_type == "binary":
                result = self.decoder.decode_binary(content)
            elif decode_type == "morse":
                result = self.decoder.decode_morse(content)
            elif decode_type == "atbash":
                result = self.decoder.decode_atbash(content)
            elif decode_type == "railfence":
                # Default 3 rails, can be extended to accept rails as parameter
                result = self.decoder.decode_railfence(content, 3)
            elif decode_type == "bacon":
                result = self.decoder.decode_bacon(content)
            elif decode_type == "auto":
                result = self.decoder.auto_decode(content)
            else:
                print(f"Unknown decode type: {decode_type}")
                print("Available types: base64, hex, binary, morse, atbash, railfence, bacon, auto")
                return
            
            if result:
                print(f"[{decode_type.upper()}] {result}")
            else:
                print("Failed to decode.")
                
        except Exception as e:
            print(f"Error decoding: {e}")