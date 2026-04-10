import json
import os
from datetime import datetime
from terminal.terminal import Terminal
from filesystem.virtual_fs import VirtualFileSystem
from auth.user_manager import UserManager
from logs.logger import Logger
from ctf.challenge_manager import ChallengeManager

class Kernel:

    def __init__(self):

        self.system_name = "initium"
        self.version = "0.1"
        self.load_users()
        self.load_filesystem()
        self.vfs = VirtualFileSystem(self.filesystem)
        self.user_manager = UserManager(self.users)
        self.logger = Logger()
        self.ctf = ChallengeManager(self.vfs)

    def boot(self):

        self.logger.log("SYSTEM BOOT")
        terminal = Terminal(self)
        terminal.start()

    def _print_banner(self):

        print("=" * 40)
        print(f"{self.system_name} v {self.version}")
        print("Cyber Security Simulation Environment")
        print("=" * 40)
        print("\n")

    def load_users(self):

        path = "system_data/users.json"

        if os.path.exists(path):
            with open(path) as f:
                self.users = json.load(f)
        else:
            print("User database not found")
            self.users = {}

    def load_filesystem(self):

        path = "system_data/filesystem.json"

        if os.path.exists(path):
            with open(path) as f:
                self.filesystem = json.load(f)
        else:
            print("Filesystem database not found")
            self.filesystem = {
                "/": {
                    "type": "dir",
                    "content": {},
                    "owner": "root",
                    "perm": "755"
                }
            }

    def log(self, message):

        with open("logs/system.log", "a") as f:
            f.write(f"{datetime.now()} : {message}\n")