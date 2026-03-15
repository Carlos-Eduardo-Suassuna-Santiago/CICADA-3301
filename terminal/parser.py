class CommandParser:

    def __init__(self, kernel=None):
        self.kernel = kernel

    def parse(self, command):

        parts = command.strip().split()

        if len(parts) == 0:
            return "", []
        
        cmd = parts[0]
        args = parts[1:]

        return cmd, args