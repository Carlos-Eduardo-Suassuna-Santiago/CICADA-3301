"""Module for the shutdown component of the CICADA-3301 application."""

from commands.base_command import BaseCommand
from time import sleep  

class ShutdownCommand(BaseCommand):
    """Command implementation for the shutdown command."""

    name = "shutdown"
    description = "Shutdown the system"
    usage = "shutdown"
    description = "Shutdown the system"
    usage = "shutdown"

    def execute(self, terminal, args):
        """Execute the operation for this component."""
        
        terminal.logger.log_shutdown()

        print("Desligando em 3 segundos...")
        sleep(1)
        print("2...")
        sleep(1)
        print("1...")
        sleep(1)

        terminal.exiting = True
