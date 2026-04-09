import json
from ctf.missions_engine import MissionsEngine


class ChallengeManager:

    def __init__(self, vfs):

        with open("ctf/challenges.json") as f:
            self.data = json.load(f)

        self.current = 0
        self.engine = MissionsEngine(vfs)

        self.start_challenge()

    def get_current(self):
        return self.data["challenges"][self.current]

    def get_available_commands(self):
        """Retorna lista de comandos disponíveis para o nível atual."""
        challenge = self.get_current()
        return challenge.get("available_commands", [])

    def get_current_level(self):
        """Retorna o nível do desafio atual (0-6)."""
        return self.current

    def start_challenge(self):
        """Inicia o desafio atual executando ações on_start."""
        challenge = self.get_current()

        if "on_start" in challenge:
            self.engine.execute_actions(challenge["on_start"])

    def submit_flag(self, flag, username="guest"):

        challenge = self.get_current()

        if flag == challenge["flag"]:

            if "on_complete" in challenge:
                self.engine.execute_actions(challenge["on_complete"])

            progress_text = f"Challenge {challenge['id']} solved: {challenge['name']}\n"
            self.engine.vfs.create_file(
                f"/home/{username}/progress.txt",
                progress_text,
                owner=username,
                perm="644"
            )

            self.current += 1

            if self.current < len(self.data["challenges"]):
                self.start_challenge()

            return True

        return False