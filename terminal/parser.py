"""Module for the parser component of the CICADA-3301 application."""

class CommandParser:
    """Parse raw command input into command names and arguments."""

    def __init__(self, kernel=None):
        """Initialize the object state."""
        self.kernel = kernel

    def parse(self, command):
        """parse function."""

        parts = command.strip().split()

        if len(parts) == 0:
            return "", []
        
        cmd = parts[0]
        args = parts[1:]

        return cmd, args
