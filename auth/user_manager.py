"""Module for the user_manager component of the CICADA-3301 application."""

import bcrypt

class UserManager:
    """UserManager handles authentication and current user state."""

    def __init__(self, users_data):
        """Initialize the object state."""

        self.users = users_data
        self.current_user = None

    def login(self, username, password):
        """login function."""

        if username not in self.users:
            return False
        
        stored_hash = self.users[username]["password"]
        if bcrypt.checkpw(password.encode(), stored_hash.encode()):
            self.current_user = username
            return True
        return False
    
    def logout(self):
        """logout function."""

        self.current_user = "guest"

    def get_current_user(self):
        """Return the current_user from the current state."""

        return self.current_user
    
    def set_current_user(self, user):
        """Set the current_user in the current object state."""

        self.current_user = user
    
    def get_role(self):
        """Return the role from the current state."""

        if self.current_user not in self.users:
            return "guest"
        return self.users[self.current_user].get("role", "guest")

    def get_user(self):
        """Return the user from the current state."""

        return self.current_user if self.current_user else "guest"
