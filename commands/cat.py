from commands.base_command import BaseCommand

class CatCommand(BaseCommand):
    
    name = "cat"

    def execute(self, terminal, args):

        if len(args) == 0:
            return
        
        content = terminal.vfs.read_file(args[0])

        if content:
            print(content)
        else:
            print(f"File '{args[0]}' not found.")