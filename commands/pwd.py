"""Module for the pwd component of the CICADA-3301 application."""

from commands.base_command import BaseCommand

class PwdCommand(BaseCommand):
    """Command implementation for the pwd command."""
    
    name = "pwd"
    description = "Print working directory"
    usage = "pwd"
    
    def execute(self, terminal, args):
        """Execute the operation for this component."""
        print(terminal.vfs.get_pwd())
