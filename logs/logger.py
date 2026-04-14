"""Module for the logger component of the CICADA-3301 application."""

import os
from datetime import datetime

class Logger:
    """Logger handles application event logging to the system log file."""

    def __init__(self, log_file="logs/system.log"):
        """Initialize the object state."""

        self.log_file = log_file
        os.makedirs(os.path.dirname(log_file), exist_ok=True)

    def log(self, message):
        """log function."""

        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        with open(self.log_file, "a") as f:
            f.write(f"[{timestamp}] {message}\n")

    def log_login(self, username, success):
        """log_login function."""

        status = "SUCCESS" if success else "FAILED"
        self.log(f"LOGIN {status}: {username}")

    def log_command(self, user, command):
        """log_command function."""

        self.log(f"COMMAND: {user} executed '{command}'")

    def log_logout(self, user):
        """log_logout function."""

        self.log(f"LOGOUT: {user}")

    def log_shutdown(self):
        """log_shutdown function."""

        self.log("SYSTEM SHUTDOWN")
