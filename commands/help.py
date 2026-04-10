"""Module for the help component of the CICADA-3301 application."""

from commands.base_command import BaseCommand

class Colors:
    """Colors class."""
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    CYAN = '\033[96m'
    END = '\033[0m'

class HelpCommand(BaseCommand):
    """Command implementation for the help command."""

    name = "help"
    description = "List available commands"
    usage = "help"

    def execute(self, terminal, args):
        """Execute the operation for this component."""

        available_cmds = terminal.ctf.get_available_commands()

        if len(args) == 1:

            cmd = terminal.commands.get(args[0])

            if not cmd:
                print(f"Command not found: {args[0]}")
                return

            if args[0] not in available_cmds:
                print(f"Command '{args[0]}' not available at this level.")
                return

            print(f"\nCommand: {cmd.name}")
            print(f"Description: {cmd.description}")
            print(f"Usage: {cmd.usage}")
            return


        challenge = terminal.ctf.get_current()
        level = terminal.ctf.get_current_level() + 1
        
        print(f"\n{Colors.CYAN}=== CHALLENGE {level}/7 ==={Colors.END}")
        print(f"{Colors.GREEN}{challenge['name']}{Colors.END}")
        print(f"Description: {challenge['description']}")
        print(f"Difficulty: {challenge['difficulty']} | Points: {challenge['points']}\n")

        print(f"{Colors.YELLOW}Available commands at this level:{Colors.END}")

        for cmd_name in available_cmds:
            cmd = terminal.commands.get(cmd_name)
            if cmd:
                print(f"  {Colors.GREEN}{cmd.name:12}{Colors.END} - {cmd.description}")
        
        print(f"\n{Colors.YELLOW}Use 'help <command>' for more details on a specific command.{Colors.END}")
