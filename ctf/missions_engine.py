"""Module for the missions_engine component of the CICADA-3301 application."""

class MissionsEngine:
    """Execute mission actions that modify the virtual filesystem for challenge progression."""

    def __init__(self, vfs):
        """Initialize the object state."""
        self.vfs = vfs

    def execute_actions(self, actions):
        """execute_actions function."""

        for action in actions:

            if action["action"] == "create_file":
                owner = action.get("owner", "root")
                perm = action.get("perm", "644")
                self.vfs.create_file(action["path"], action["content"], owner, perm)
            
            elif action["action"] == "copy_file":

                self.vfs.copy_file(action["source"], action["dest"])
