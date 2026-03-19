from commands.base_command import BaseCommand   

class LogoutCommand(BaseCommand):

    name = "logout"
    description = "Logout current session"
    usage = "logout"
    description = "Logout of the current session"
    usage = "logout"

    def execute(self, terminal, args):
        
        terminal.logger.log_logout(terminal.auth.get_current_user())
        terminal.auth.current_user = "guest"
        terminal.session_start = None
        terminal.running = False

        print("Logging out...")

    