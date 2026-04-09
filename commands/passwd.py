import getpass
import bcrypt
import json
from commands.base_command import BaseCommand

class Colors:
    RED = '\033[91m'
    GREEN = '\033[92m'
    END = '\033[0m'

class PasswdCommand(BaseCommand):

    name = "passwd"
    description = "Change user password"
    usage = "passwd"

    def execute(self, terminal, args):

        user = terminal.auth.get_current_user()

        current = getpass.getpass("Current password: ")

        if not terminal.auth.login(user, current):
            print(f"{Colors.RED}Current password incorrect{Colors.END}")
            terminal.logger.log(f"PASSWORD CHANGE FAILED: {user} - wrong current password")
            return

        new = getpass.getpass("New password: ")
        confirm = getpass.getpass("Confirm new password: ")

        if new != confirm:
            print(f"{Colors.RED}Passwords do not match{Colors.END}")
            terminal.logger.log(f"PASSWORD CHANGE FAILED: {user} - passwords do not match")
            return

        hashed = bcrypt.hashpw(new.encode(), bcrypt.gensalt())

        # update in memory
        terminal.kernel.user_manager.users[user]["password"] = hashed.decode()

        # update file
        with open("system_data/users.json", "w") as f:
            json.dump(terminal.kernel.user_manager.users, f, indent=4)

        print(f"{Colors.GREEN}Password changed successfully{Colors.END}")
        terminal.logger.log(f"PASSWORD CHANGE SUCCESS: {user}")