from commands.base_command import BaseCommand

class PsCommand(BaseCommand):

    name = "ps"
    description = "List running processes"
    usage = "ps"

    def execute(self, terminal, args):
        
        processes = terminal.process_manager.list_process()

        print("PID USER NAME")

        for p in processes:
            print(f"{p['pid']}  {p['user']}  {p['name']}")