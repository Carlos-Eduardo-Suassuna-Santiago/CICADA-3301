"""Module for the ls component of the CICADA-3301 application."""

from commands.base_command import BaseCommand

class Colors:
    """Colors class."""
    BLUE = '\033[94m'
    WHITE = '\033[97m'
    END = '\033[0m'

class LsCommand(BaseCommand):
    """Command implementation for the ls command."""

    name = "ls"
    description = "List directory contents"
    usage = "ls [directory]"

    def execute(self, terminal, args):
        """Execute the operation for this component."""

        files_with_types  = terminal.vfs.list_dir(None, terminal.auth.get_current_user())

        if not files_with_types:
            return

        # Color based on type
        colored_files = []
        for name, typ in files_with_types:
            if typ == 'dir':
                colored_files.append(f'{Colors.BLUE}{name}{Colors.END}')
            else:
                colored_files.append(f'{Colors.WHITE}{name}{Colors.END}')

        # Organize in columns
        files = colored_files
        cols = 4
        max_len = max(len(f) for f in files) + 2  # add some space

        for i in range(0, len(files), cols):
            row = files[i:i+cols]
            line = '  '.join(f'{f:<{max_len}}' for f in row)
            print(line)
