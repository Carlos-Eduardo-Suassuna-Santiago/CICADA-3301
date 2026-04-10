"""Module for the stego component of the CICADA-3301 application."""

from commands.base_command import BaseCommand
from security.stego_engine import StegoEngine
import base64
import os

class StegoCommand(BaseCommand):
    """Command implementation for the stego command."""

    name = "stego"
    description = "Steganography operations"
    usage = "stego extract <image>"

    autocomplete = { 1: ["extract"]}

    def __init__(self):
        """Initialize the object state."""
        self.engine = StegoEngine()

    def execute(self, terminal, args):
        """Execute the operation for this component."""

        if len(args) < 2:
            print("Usage: stego extract <image>")
            return
        
        mode = args[0]
        filename = args[1]

        if mode != "extract":
            print("Invalid mode. Use 'extract'.")
            return

        content = terminal.vfs.read_file(filename, terminal.auth.get_current_user())
        temp_path = None

        if content is None:
            print(f"File not found: {filename}")
            return

        # Handle stub text files with an embedded message
        if isinstance(content, bytes):
            if content.startswith(b"STEGOMSG:"):
                print(content.split(b"STEGOMSG:", 1)[1].decode("utf-8", errors="ignore"))
                return
            temp_path = "/tmp/stego_temp.png"
            with open(temp_path, "wb") as f:
                f.write(content)
        else:
            if content.startswith("STEGOMSG:"):
                print(content.split("STEGOMSG:", 1)[1])
                return

            try:
                image_data = base64.b64decode(content)
                temp_path = "/tmp/stego_temp.png"
                with open(temp_path, "wb") as f:
                    f.write(image_data)
            except Exception:
                temp_path = None

        if temp_path:
            path = temp_path
        else:
            filename = os.path.basename(filename)
            if ".." in filename or "/" in filename or "\\" in filename:
                print("Invalid filename.")
                return
            path = f"assets/{filename}"

        result = self.engine.extract_lsb(path)
        print(result)
