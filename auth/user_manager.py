import bcrypt

class UserManager:

    def __init__(self, users_data):

        self.users = users_data
        self.current_user = None

    def login(self, username, password):

        if username not in self.users:
            return False
        
        stored_hash = self.users[username]["password"]
        if bcrypt.checkpw(password.encode(), stored_hash.encode()):
            self.current_user = username
            return True
        return False
    
    def logout(self):

        self.current_user = "guest"

    def get_current_user(self):

        return self.current_user
    
    def set_current_user(self, user):

        self.current_user = user
    
    def get_role(self):

        if self.current_user not in self.users:
            return "guest"
        return self.users[self.current_user].get("role", "guest")

    def get_user(self):

        return self.current_user if self.current_user else "guest"