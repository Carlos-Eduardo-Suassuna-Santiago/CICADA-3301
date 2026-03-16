from commands.base_command import BaseCommand   

class LogoutCommand(BaseCommand):

    name = "logout"

    def execute(self, terminal, args):
        
        terminal.auth.logout()
        print("Logged out successfully.")