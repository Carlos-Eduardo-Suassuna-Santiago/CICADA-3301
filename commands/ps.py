"""Module for the ps component of the CICADA-3301 application."""

from commands.base_command import BaseCommand

class PsCommand(BaseCommand):
    """Command implementation for the ps command."""

    name = "ps"
    description = "List running processes"
    usage = "ps"

    def execute(self, terminal, args):
        """Execute the operation for this component."""
        
        processes = terminal.process_manager.list_process()

        print("PID USER NAME")

        for p in processes:
            print(f"{p['pid']}  {p['user']}  {p['name']}")
