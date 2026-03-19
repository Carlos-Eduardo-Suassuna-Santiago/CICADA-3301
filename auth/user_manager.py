class UserManager:

    def __init__(self, users_data):

        self.users = users_data
        self.current_user = "guest"

    def login(self, username, password):

        if username not in self.users:
            return False
        
        users = self.users[username]

        if users["password"] == password:
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

        return self.users[self.current_user]["role"]