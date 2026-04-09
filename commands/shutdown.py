from commands.base_command import BaseCommand
from time import sleep  

class ShutdownCommand(BaseCommand):

    name = "shutdown"
    description = "Shutdown the system"
    usage = "shutdown"
    description = "Shutdown the system"
    usage = "shutdown"

    def execute(self, terminal, args):
        
        terminal.logger.log_shutdown()
        terminal.exiting = True

        print("Shutting down...")
        sleep(3)