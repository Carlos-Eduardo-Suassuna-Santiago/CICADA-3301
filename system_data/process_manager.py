import random

class ProcessManager:

    def __init__(self):
    
        self.processes = []
        self._generate_processes()

    def _generate_processes(self):

        names = ["init", "sshd", "nginx", "mysql", "redis", "python", "java", "node", "go", "ruby"]

        for i in range(6):

            self.processes.append({
                "pid": random.randint(100, 999),
                "name": names[i],
                "user": "root" if i < 3 else "guest",
            })

    def list_process(self):
        
        return self.processes