from terminal.parser import CommandParser

class Terminal:

    def __init__(self, kernel):

        self.kernel = kernel
        self.parser = CommandParser(kernel)
        self.current_user = "guest"
        self.current_dir = "/home/guest"
        self.running = True

    def start(self):

        while self.running:

            prompt = f"{self.current_user}@{self.kernel.system_name}:{self.current_dir}$ "
            command = input(prompt)
            self.execute(command)
    
    def execute(self, command):
        
        cmd, args = self.parser.parse(command)

        if cmd == "exit":
            self.running = False
            print("\nExiting terminal...")
        elif cmd == "help":
            self.help()
        elif cmd == "clear":
            print("\n" * 50)

    def help(self):

        print("\nAvailable commands:")
        print("help - Show commands")
        print("clear - Clear the terminal")
        print("exit - Close terminal")