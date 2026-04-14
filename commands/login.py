"""Module for the login component of the CICADA-3301 application."""

from commands.base_command import BaseCommand
import getpass

class LoginCommand(BaseCommand):
    """Command implementation for the login command."""

    name = "login"
    description = "Login to the system"
    usage = "login"

    def execute(self, terminal, args):
        """Execute the operation for this component."""

        username = input("Username: ")
        password = getpass.getpass("Password: ")

        success = terminal.auth.login(username, password)

        if success:
            print(f"Logged in as {username}")
        else:
            print("Invalid username or password")
