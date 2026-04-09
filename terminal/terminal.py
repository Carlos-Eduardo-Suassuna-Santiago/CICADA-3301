import os
import getpass
import signal
import time

class Colors:
    GREEN = '\033[92m'
    BLUE = '\033[94m'
    RED = '\033[91m'
    YELLOW = '\033[93m'
    CYAN = '\033[96m'
    MAGENTA = '\033[95m'
    WHITE = '\033[97m'
    END = '\033[0m'

from system_data.process_manager import ProcessManager
from terminal.parser import CommandParser
from terminal.input_handler import InputHandler
from commands.help import HelpCommand
from commands.ls import LsCommand
from commands.cd import CdCommand
from commands.pwd import PwdCommand
from commands.cat import CatCommand
from commands.clear import ClearCommand
from commands.login import LoginCommand
from commands.logout import LogoutCommand
from commands.whoami import WhoamiCommand
from commands.decode import DecodeCommand
from commands.hash import HashCommand
from commands.stego import StegoCommand
from commands.decrypt import DecryptCommand
from commands.shutdown import ShutdownCommand
from commands.passwd import PasswdCommand
from commands.su import SuCommand
from commands.who import WhoCommand
from commands.submit import SubmitCommand
from commands.history import HistoryCommand
from commands.sudo import SudoCommand
from commands.ps import PsCommand
from commands.logread import LogreadCommand
from commands.analyze import AnalyzeCommand
from commands.exploit import ExploitCommand
from commands.manual import ManualCommand


class Terminal:

    def __init__(self, kernel):

        self.process_manager = ProcessManager()
        self.input_handler = InputHandler(self)
        self.history = []
        self.kernel = kernel
        self.vfs = kernel.vfs
        self.parser = CommandParser(kernel)
        self.auth = kernel.user_manager
        self.ctf = kernel.ctf
        self.current_dir = "/"
        self.running = True
        self.exiting = False
        self.logger = kernel.logger
        self.timeout = 300  # 5 minutes
        self.session_start = None
        self.commands = {
            "ls": LsCommand(),
            "cd": CdCommand(),
            "pwd": PwdCommand(),
            "cat": CatCommand(),
            "help": HelpCommand(),
            "login": LoginCommand(),
            "logout": LogoutCommand(),
            "passwd": PasswdCommand(),
            "su": SuCommand(),
            "shutdown": ShutdownCommand(),
            "whoami": WhoamiCommand(),
            "who": WhoCommand(),
            "clear": ClearCommand(),
            "decode": DecodeCommand(),
            "hash": HashCommand(),
            "stego": StegoCommand(),
            "decrypt": DecryptCommand(),
            "analyze": AnalyzeCommand(),
            "exploit": ExploitCommand(),
            "submit": SubmitCommand(),
            "history": HistoryCommand(),
            "sudo": SudoCommand(),
            "ps": PsCommand(),
            "logread": LogreadCommand(),
            "manual": ManualCommand()
        }

    def login_screen(self):

        while True:

            self.clear_screen()
            self._print_banner()

            print("\n=== LOGIN ===")

            username = input("Username: ")
            password = getpass.getpass("Password: ")

            if self.auth.login(username, password):

                self.vfs.current_path = f"/home/{username}"
                self.session_start = time.time()
                self.logger.log_login(username, True)
                print(f"{Colors.GREEN}Login successful!{Colors.END}")
                print("Press Enter to continue...")
                return

            else:
                self.logger.log_login(username, False)
                print(f"{Colors.RED}Invalid credentials.{Colors.END}")
                print("Press Enter to try again...")

    def _print_banner(self):

        print(f"{Colors.CYAN}{'=' * 40}{Colors.END}")
        print(f"{Colors.GREEN}{self.kernel.system_name} v {self.kernel.version}{Colors.END}")
        print(f"{Colors.YELLOW}Cyber Security Simulation Environment{Colors.END}")
        print(f"{Colors.CYAN}{'=' * 40}{Colors.END}")
        print("\n")

    def clear_screen(self):

        os.system('cls' if os.name == 'nt' else 'clear')

    def run_session(self):

        self.running = True

        def timeout_handler(signum, frame):

            print(f"\n{Colors.YELLOW}Session timed out due to inactivity.{Colors.END}")
            self.logger.log(f"SESSION TIMEOUT: {self.auth.get_current_user()}")
            self.running = False

        # Setup signal handler only on Unix/Linux/Mac (not on Windows)
        if os.name != 'nt':
            signal.signal(signal.SIGALRM, timeout_handler)

        self.clear_screen()
        self._print_banner()

        while self.running and not self.exiting:

            if os.name != 'nt':
                signal.alarm(self.timeout)

            try:

                prompt = f"{Colors.GREEN}{self.auth.get_current_user()}{Colors.END}@{Colors.BLUE}{self.kernel.system_name}{Colors.END}:{Colors.CYAN}{self.vfs.get_pwd()}{Colors.END}$ "
                command = self.input_handler.process_input(prompt)
                
                if os.name != 'nt':
                    signal.alarm(0)  # cancel alarm

                self.history.append(command)
                self.execute(command)
                self.input_handler.buff = []
                self.input_handler.cursor = 0

            except KeyboardInterrupt:

                # timeout occurred, running already set to False
                pass
        

    def start(self):

        while not self.exiting:

            self.login_screen()
            self.run_session()
    
    def execute(self, command):
        
       cmd, args = self.parser.parse(command)

       if cmd == "":
           return
       
       self.logger.log_command(self.auth.get_current_user(), command)
       
       available_cmds = self.ctf.get_available_commands()
       
       if cmd not in available_cmds:
           print(f"{Colors.RED}Command '{cmd}' not available at this level.{Colors.END}")
           print(f"{Colors.YELLOW}Available commands: {', '.join(available_cmds)}{Colors.END}")
           return
       
       if cmd in self.commands:
          self.commands[cmd].execute(self, args) 
       else:
          print(f"{Colors.RED}Command not found: {cmd}{Colors.END}")

    def help(self):

        print("\nAvailable commands:")
        print("help - Show commands")
        print("clear - Clear the terminal")
        print("exit - Close terminal")