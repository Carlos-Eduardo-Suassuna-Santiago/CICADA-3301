import os
from commands.base_command import BaseCommand

class ClearCommand(BaseCommand):

    name = "clear"
    description = "Clear the screen"
    usage = "clear"

    def execute(self, terminal, args):

        if os.name == 'nt':
            os.system('cls')
        else:
            os.system('clear')
        terminal._print_banner()