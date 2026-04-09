from commands.base_command import BaseCommand

class SudoCommand(BaseCommand):

    name = "sudo"
    description = "Execute a command as another user"
    usage = "sudo <command>"

    def execute(self, terminal, args):

        if len(args) == 0:
            print("Usage: sudo <command>")
            return

        original_user = terminal.auth.get_current_user()
        terminal.auth.set_current_user("root")

        command_name = args[0]
        command_args = args[1:]

        command = terminal.commands.get(command_name)

        keep_root = False
        if command:
            result = command.execute(terminal, command_args)
            keep_root = bool(result)
        else:
            print(f"Command not found: {command_name}")

        if not keep_root:
            terminal.auth.set_current_user(original_user)