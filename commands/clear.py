"""Module for the clear component of the CICADA-3301 application."""

import os
from commands.base_command import BaseCommand

class ClearCommand(BaseCommand):
    """Command implementation for the clear command."""

    name = "clear"
    description = "Clear the screen"
    usage = "clear"

    def execute(self, terminal, args):
        """Execute the operation for this component."""

        if os.name == 'nt':
            os.system('cls')
        else:
            os.system('clear')
        terminal._print_banner()
