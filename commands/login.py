from commands.base_command import BaseCommand

class LoginCommand(BaseCommand):

    name = "login"

    def execute(self, terminal, args):

        username = input("Username: ")
        password = input("Password: ")

        success = terminal.auth.login(username, password)

        if success:
            print(f"Logged in as {username}")
        else:
            print("Invalid username or password")