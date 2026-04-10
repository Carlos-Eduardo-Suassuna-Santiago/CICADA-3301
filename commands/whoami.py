"""Module for the whoami component of the CICADA-3301 application."""

from commands.base_command import BaseCommand

class WhoamiCommand(BaseCommand):
    """Command implementation for the whoami command."""

    name = "whoami"
    description = "Display current user"
    usage = "whoami"

    def execute(self, terminal, args):
        """Execute the operation for this component."""
        
        print(terminal.auth.get_current_user())
