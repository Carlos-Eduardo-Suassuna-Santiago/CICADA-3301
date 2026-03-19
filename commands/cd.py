from commands.base_command import BaseCommand

class CdCommand(BaseCommand):

    name = "cd"

    def execute(self, terminal, args):

        if len(args) == 0:
            return
        
        success = terminal.vfs.change_dir(args[0], terminal.auth.get_current_user())

        if not success:
            print(f"Directory '{args[0]}' not found or permission denied.")