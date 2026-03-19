import getpass
from commands.base_command import BaseCommand

class Colors:
    GREEN = '\033[92m'
    RED = '\033[91m'
    END = '\033[0m'

class SuCommand(BaseCommand):

    name = "su"
    description = "Switch to another user"
    usage = "su <username>"

    def execute(self, terminal, args):

        if len(args) == 0:
            print("Usage: su <username>")
            return

        username = args[0]
        old_user = terminal.auth.get_current_user()

        if username == old_user:
            print("Already logged in as that user")
            return

        password = getpass.getpass(f"Password for {username}: ")

        if terminal.auth.login(username, password):
            terminal.auth.current_user = username
            terminal.vfs.current_path = f"/home/{username}"
            print(f"{Colors.GREEN}Switched to {username}{Colors.END}")
            terminal.logger.log(f"USER SWITCH: {old_user} -> {username}")
        else:
            print(f"{Colors.RED}Authentication failed{Colors.END}")
            terminal.logger.log(f"USER SWITCH FAILED: {old_user} -> {username}")