from terminal.parser import CommandParser

class Terminal:

    def __init__(self, kernel):

        self.kernel = kernel
        self.vfs = kernel.vfs
        self.parser = CommandParser(kernel)
        self.current_user = "guest"
        self.current_dir = "/home/guest"
        self.running = True

    def start(self):

        while self.running:

            prompt = f"{self.current_user}@{self.kernel.system_name}:{self.vfs.get_pwd()}$ "
            command = input(prompt)
            self.execute(command)
    
    def execute(self, command):
        
        cmd, args = self.parser.parse(command)

        if cmd == "exit":
            self.running = False
            print("\nExiting terminal...")
        elif cmd == "help":
            self.help()
        elif cmd == "ls":
            files = self.vfs.list_dir()
            for f in files:
                print(f)
        elif cmd == "cd":
            if len(args) == 0:
                return
            success = self.vfs.change_dir(args[0])
            if not success:
                print(f"Directory '{args[0]}' not found")
        elif cmd == "pwd":
            print(self.vfs.get_pwd())
        elif cmd == "cat":
            if len(args) == 0:
                return
            content = self.vfs.read_file(args[0])
            if content:
                print(content)
            else:
                print(f"File '{args[0]}' not found")
        elif cmd == "clear":
            print("\n" * 50)

    def help(self):

        print("\nAvailable commands:")
        print("help - Show commands")
        print("clear - Clear the terminal")
        print("exit - Close terminal")