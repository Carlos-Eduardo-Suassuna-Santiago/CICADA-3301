"""Module for the logout component of the CICADA-3301 application."""

from commands.base_command import BaseCommand   

class LogoutCommand(BaseCommand):
    """Command implementation for the logout command."""

    name = "logout"
    description = "Logout of the current session"
    usage = "logout"

    def execute(self, terminal, args):
        """Execute the operation for this component."""
        
        terminal.logger.log_logout(terminal.auth.get_current_user())
        terminal.auth.logout()
        terminal.session_start = None
        terminal.running = False

        print("Logging out...")

    
