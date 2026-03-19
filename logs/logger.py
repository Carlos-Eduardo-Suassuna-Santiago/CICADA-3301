import os
from datetime import datetime

class Logger:

    def __init__(self, log_file="logs/system.log"):

        self.log_file = log_file
        os.makedirs(os.path.dirname(log_file), exist_ok=True)

    def log(self, message):

        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        with open(self.log_file, "a") as f:
            f.write(f"[{timestamp}] {message}\n")

    def log_login(self, username, success):

        status = "SUCCESS" if success else "FAILED"
        self.log(f"LOGIN {status}: {username}")

    def log_command(self, user, command):

        self.log(f"COMMAND: {user} executed '{command}'")

    def log_logout(self, user):

        self.log(f"LOGOUT: {user}")

    def log_shutdown(self):

        self.log("SYSTEM SHUTDOWN")