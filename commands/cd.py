"""Module for the cd component of the CICADA-3301 application."""

from commands.base_command import BaseCommand

class Colors:
    """Colors class."""
    RED = '\033[91m'
    END = '\033[0m'

class CdCommand(BaseCommand):
    """Command implementation for the cd command."""

    name = "cd"
    description = "Change directory"
    usage = "cd <directory>"

    def execute(self, terminal, args):
        """Execute the operation for this component."""

        if len(args) == 0:
            return
        
        success = terminal.vfs.change_dir(args[0], terminal.auth.get_current_user())

        if not success:
            print(f"{Colors.RED}Directory '{args[0]}' not found or permission denied.{Colors.END}")
