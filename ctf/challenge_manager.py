import json

class ChallengeManager:

    def __init__(self):

        with open("ctf/challenges.json") as f:
            self.data = json.load(f)

        self.current = 0

    def get_current(self):

        return self.data["challenges"][self.current]

    def submit_flag(self, flag):

        correct = self.get_current()["flag"]

        if flag == correct:

            self.current += 1
            return True
        
        return False