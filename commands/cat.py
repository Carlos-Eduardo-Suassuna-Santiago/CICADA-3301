from commands.base_command import BaseCommand

class CatCommand(BaseCommand):

    name = "cat"

    def execute(self, terminal, args):

        if len(args) == 0:
            print("usage: cat <file>")
            return

        filename = args[0]

        content = terminal.vfs.read_file(filename)

        if content is None:
            print("file not found")
            return

        # pega o nó do arquivo no filesystem
        node = terminal.vfs._get_node(
            terminal.vfs._resolve_path(filename)
        )

        user = terminal.auth.get_current_user()

        if terminal.vfs.has_permission(node, user):

            print(content)

        else:

            print("permission denied")