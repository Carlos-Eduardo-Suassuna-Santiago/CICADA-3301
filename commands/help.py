from commands.base_command import BaseCommand

class HelpCommand(BaseCommand):

    name = "help"

    def execute(self, terminal, args):

        print("\nAvailable commands:")

        for cmd in terminal.commands:
            print(f"  {cmd.name}")