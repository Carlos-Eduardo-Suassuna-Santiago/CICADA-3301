class MissionsEngine:

    def __init__(self, vfs):
        self.vfs = vfs

    def execute_actions(self, actions):

        for action in actions:

            if action["action"] == "create_file":
                owner = action.get("owner", "root")
                perm = action.get("perm", "644")
                self.vfs.create_file(action["path"], action["content"], owner, perm)