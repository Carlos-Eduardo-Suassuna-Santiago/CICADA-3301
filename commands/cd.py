from commands.base_command import BaseCommand

class Colors:
    RED = '\033[91m'
    END = '\033[0m'

class CdCommand(BaseCommand):

    name = "cd"
    description = "Change directory"
    usage = "cd <directory>"

    def execute(self, terminal, args):

        if len(args) == 0:
            return
        
        success = terminal.vfs.change_dir(args[0], terminal.auth.get_current_user())

        if not success:
            print(f"{Colors.RED}Directory '{args[0]}' not found or permission denied.{Colors.END}")