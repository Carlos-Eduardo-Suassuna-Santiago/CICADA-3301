"""Module for the submit component of the CICADA-3301 application."""

from commands.base_command import BaseCommand    

class SubmitCommand(BaseCommand):
    """Command implementation for the submit command."""
    name = "submit"
    description = "Submit a flag"
    usage = "submit <flag>"

    def execute(self, terminal, args):
        """Execute the operation for this component."""

        if len(args) < 1:
            print("Usage: submit <flag>")
            return

        flag = args[0]
        user = terminal.auth.get_current_user()

        if terminal.ctf.submit_flag(flag, user):
            print("Flag correct")
            print("Next challenge unlocked")
        else:
            print("Flag incorrect!")
