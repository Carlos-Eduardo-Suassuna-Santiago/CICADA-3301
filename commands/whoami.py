from commands.base_command import BaseCommand

class WhoamiCommand(BaseCommand):

    name = "whoami"
    description = "Display current user"
    usage = "whoami"

    def execute(self, terminal, args):
        
        print(terminal.auth.get_current_user())