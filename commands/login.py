from commands.base_command import BaseCommand
import getpass

class LoginCommand(BaseCommand):

    name = "login"
    description = "Login to the system"
    usage = "login"

    def execute(self, terminal, args):

        username = input("Username: ")
        password = getpass.getpass("Password: ")

        success = terminal.auth.login(username, password)

        if success:
            print(f"Logged in as {username}")
        else:
            print("Invalid username or password")