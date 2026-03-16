from terminal.parser import CommandParser
from commands.help import HelpCommand
from commands.ls import LsCommand
from commands.cd import CdCommand
from commands.pwd import PwdCommand
from commands.cat import CatCommand
#from commands.clear import ClearCommand
#from commands.exit import ExitCommand
from commands.login import LoginCommand
from commands.logout import LogoutCommand
from commands.whoami import WhoamiCommand

class Terminal:

    def __init__(self, kernel):

        self.kernel = kernel
        self.vfs = kernel.vfs
        self.parser = CommandParser(kernel)
        self.auth = kernel.user_manager
        self.current_user = "guest"
        self.current_dir = "/"
        self.running = True
        self.commands = {
            "ls": LsCommand(),
            "cd": CdCommand(),
            "pwd": PwdCommand(),
            "cat": CatCommand(),
            "help": HelpCommand(),
            "login": LoginCommand(),
            "logout": LogoutCommand(),
            "whoami": WhoamiCommand(),
        }

    def start(self):

        while self.running:

            prompt = f"{self.auth.get_current_user()}@{self.kernel.system_name}:{self.vfs.get_pwd()}$ "
            command = input(prompt)
            self.execute(command)
    
    def execute(self, command):
        
       cmd, args = self.parser.parse(command)

       if cmd == "":
           return
       if cmd == "exit":
          self.running = False
          return
       if cmd in self.commands:
          self.commands[cmd].execute(self, args) 
       else:
          print(f"Command not found: {cmd}")

    def help(self):

        print("\nAvailable commands:")
        print("help - Show commands")
        print("clear - Clear the terminal")
        print("exit - Close terminal")