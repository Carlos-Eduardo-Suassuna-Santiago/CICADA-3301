from commands.base_command import BaseCommand
import os

class LogreadCommand(BaseCommand):

    name = "logread"
    description = "Show system logs"
    usage = "logread [n_lines|filter]"

    def execute(self, terminal, args):

        log_file = "logs/system.log"

        # verifica se o arquivo existe
        if not os.path.exists(log_file):
            print("Log file not found.")
            return
        
        with open(log_file, "r") as f:
            lines = f.readlines()

        if not lines:
            print("No logs available.")
            return
        
        # ==============================
        # SEM ARGUMENTO --> MOSTRA TUDO
        # ==============================
        if len(args) == 0:

            for line in lines:
                print(line.strip())
            return

        arg = args[0]

        # ============================
        # NUMERO --> ÚLTIMAS N LINHAS
        # ============================
        if arg.isdigit():

            n = int(arg)

            for line in lines[-n:]:
                print(line.strip())
            return

        # ==============================
        # FILTRO --> BUSCA POR PALAVRA
        # ==============================
        keyword = arg.lower()

        filtered = [
            line for line in lines
            if keyword in line.lower()
        ]

        if not filtered:
            print("No matching logs.")
            return

        for line in filtered:
            print(line.strip())       
    