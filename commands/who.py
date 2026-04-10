"""Module for the who component of the CICADA-3301 application."""

import time
from commands.base_command import BaseCommand

class WhoCommand(BaseCommand):
    """Command implementation for the who command."""

    name = "who"
    description = "Show logged-in users and session info"
    usage = "who"

    def execute(self, terminal, args):
        """Execute the operation for this component."""

        user = terminal.auth.get_current_user()

        if terminal.session_start:

            login_time = time.ctime(terminal.session_start)

            elapsed = time.time() - terminal.session_start

            minutes = int(elapsed // 60)

            seconds = int(elapsed % 60)

            print(f"{user}  pts/0  {login_time} ({minutes}:{seconds:02d})")

        else:

            print("No active session")

        terminal.logger.log(f"WHO: {user}")
