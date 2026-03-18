class MissionsEngine:

    def __init__(self, vfs):
        self.vfs = vfs

    def execute_actions(self, actions):

        for action in actions:

            if action["action"] == "create_file":

                self.vfs.create_file(action["path"], action["content"])