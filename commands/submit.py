from commands.base_command import BaseCommand    

class SubmitCommand(BaseCommand):
    name = "submit"
    description = "Submit a flag"
    usage = "submit <flag>"

    def execute(self, terminal, args):

        if len(args) < 1:
            print("Usage: submit <flag>")
            return

        flag = args[0]

        if terminal.kernel.ctf.submit_flag(flag):

            print("Flag correct")
            print("Next challenge unlocked")
            
        else:
            print("Flag incorrect!")