from commands.base_command import BaseCommand

class HistoryCommand(BaseCommand):

    name = "history"
    description = "Show command history"
    usage = "history"

    def execute(self, terminal, args):

        for i, cmd in enumerate(terminal.command_history):
            print(f"{i + 1}: {cmd}")