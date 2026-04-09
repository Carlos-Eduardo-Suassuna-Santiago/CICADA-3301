from commands.base_command import BaseCommand

class AnalyzeCommand(BaseCommand):

    name = "analyze"
    description = "Analyze a log or text file for hidden clues"
    usage = "analyze <file>"

    def execute(self, terminal, args):

        if len(args) == 0:
            print("Usage: analyze <file>")
            return

        filename = args[0]
        content = terminal.vfs.read_file(filename, terminal.auth.get_current_user())

        if not content:
            print(f"File not found: {filename}")
            return

        lower = content.lower()

        if "failed login root" in lower and "successful login analyst" in lower:
            print("Hidden directory unlocked: /usr/bin")
            print("The suspicious log entry points to /usr/bin/backup.sh")
        else:
            print("No obvious hidden clues found.")
