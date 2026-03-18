from commands.base_command import BaseCommand

class HelpCommand(BaseCommand):

    name = "help"
    description = "List available commands"
    usage = "help"

    def execute(self, terminal, args):

        if len(args) == 1:

            cmd = terminal.commands.get(args[0])

            if not cmd:
                print(f"Command not found: {args[0]}")
                return

            print(f"\nCommand: {cmd.name}")
            print(f"Description: {cmd.description}")
            print(f"Usage: {cmd.usage}")
            return


        print("\nAvailable commands:")

        for cmd in terminal.commands.values():
            print(f"{cmd.name:10} - {cmd.description}")
        
        print("\nUse 'help <command>' for more details on a specific command.")