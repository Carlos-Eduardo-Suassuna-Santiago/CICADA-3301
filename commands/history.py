"""Module for the history component of the CICADA-3301 application."""

from commands.base_command import BaseCommand

class HistoryCommand(BaseCommand):
    """Command implementation for the history command."""

    name = "history"
    description = "Show command history"
    usage = "history"

    def execute(self, terminal, args):
        """Execute the operation for this component."""

        for i, cmd in enumerate(terminal.history):
            print(f"{i + 1}: {cmd}")
