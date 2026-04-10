"""Module for the process_manager component of the CICADA-3301 application."""

import random

class ProcessManager:
    """Simulate a simple process table for the terminal ps command."""

    def __init__(self):
        """Initialize the object state."""
    
        self.processes = []
        self._generate_processes()

    def _generate_processes(self):
        """_generate_processes function."""

        names = ["init", "sshd", "nginx", "mysql", "redis", "python", "java", "node", "go", "ruby"]

        for i in range(6):

            self.processes.append({
                "pid": random.randint(100, 999),
                "name": names[i],
                "user": "root" if i < 3 else "guest",
            })

    def list_process(self):
        """List process in the current virtual filesystem or session."""
        
        return self.processes
