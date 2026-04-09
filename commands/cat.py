from commands.base_command import BaseCommand

class Colors:
    RED = '\033[91m'
    END = '\033[0m'

class CatCommand(BaseCommand):

    name = "cat"
    description = "Display file contents"
    usage = "cat <file>"

    def execute(self, terminal, args):

        if len(args) == 0:
            print("usage: cat <file>")
            return

        filename = args[0]

        content = terminal.vfs.read_file(filename, terminal.auth.get_current_user())

        if content is None:
            print(f"{Colors.RED}file not found or permission denied{Colors.END}")
            return

        # pega o nó do arquivo no filesystem
        node = terminal.vfs._get_node(
            terminal.vfs._resolve_path(filename)
        )

        user = terminal.auth.get_current_user()

        if terminal.vfs.has_permission(node, user):

            print(content)

        else:

            print(f"{Colors.RED}permission denied{Colors.END}")