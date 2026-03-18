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

    def start_challenge(self):

        challenge = self.get_current()

        if "on_start" in challenge:
            self.engine.execute_actions(challenge["on_start"])

    def submit_flag(self, flag):

        challenge = self.get_current()

        if flag == challenge["flag"]:

            if "on_complete" in challenge:
                self.engine.execute_actions(challenge["on_complete"])

            self.current += 1

            if self.current < len(self.data["challenges"]):
                self.start_challenge()

            return True

        return False