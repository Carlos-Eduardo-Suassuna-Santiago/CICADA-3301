from commands.base_command import BaseCommand

class LsCommand(BaseCommand):

    name = "ls"

    def execute(self, terminal, args):

        files  = terminal.vfs.list_dir(None, terminal.auth.get_current_user())

        for f in files:
            print(f)