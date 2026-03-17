import os
from commands.base_command import BaseCommand

class ClearCommand(BaseCommand):

    name = "clear"

    def execute(self, terminal, args):

        if os.name == 'nt':
            os.system('cls')
        else:
            os.system('clear')