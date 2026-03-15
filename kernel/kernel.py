import json
import os
from datetime import datetime

class Kernel:

    def __init__(self):

        self.system_name = "INITIUM OS"
        self.version = "0.1"
        self.users = {}
        self.filesystem = {}

    def boot(self):

        self._print_banner()
        self.load_users()
        self.load_filesystem()
        self.log("\nSystem ready.\n")

    def _print_banner(self):

        print("=" * 40)
        print(f"{self.system_name} v {self.version}")
        print("Cyber Security Simulation Environment")
        print("=" * 40)

    def load_users(self):

        path = "system_data/users.json"

        if os.path.exists(path):
            with open(path) as f:
                self.users = json.load(f)
        else:
            print("User database not found")

    def load_filesystem(self):

        path = "system_data/filesystem.json"

        if os.path.exists(path):
            with open(path) as f:
                self.filesystem = json.load(f)
        else:
            print("Filesystem database not found")

    def log(self, message):

        with open("logs/system.log", "a") as f:
            f.write(f"{datetime.now()} : {message}\n")