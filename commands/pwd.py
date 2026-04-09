from commands.base_command import BaseCommand

class PwdCommand(BaseCommand):
    
    name = "pwd"
    description = "Print working directory"
    usage = "pwd"
    
    def execute(self, terminal, args):
        print(terminal.vfs.get_pwd())