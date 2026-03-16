class VirtualFileSystem:

    def __init__(self, fs_data):

        self.fs = fs_data
        self.current_path = "/"

    def _resolve_path(self, path):

        if path.startswith("/"):
            parts = path.strip("/").split("/")
        else:
            parts = self.current_path.strip("/").split("/") + path.split("/")

        return [p for p in parts if p]

    def _get_node(self, parts):

        node = self.fs["/"]

        if len(parts) == 0:
            return node
        for part in parts:
            if "content" not in node:
                return None
            if part not in node["content"]:
                return None

            node = node["content"][part]

        return node
    
    def list_dir(self, path=None):

        if path is None:
            path = self.current_path

        parts = self._resolve_path(path)
        node = self._get_node(parts)

        if node and node["type"] == "dir":

            return list(node["content"].keys())

        return []
    
    def change_dir(self, path):

        parts = self._resolve_path(path)

        node = self._get_node(parts)

        if node and node["type"] == "dir":

            self.current_path = "/" + "/".join(parts)

            if self.current_path == "/":
                self.current_path = "/"

            return True

        return False


    def read_file(self, filename):

        parts = self._resolve_path(filename)
        node = self._get_node(parts)

        if node and node["type"] == "file":
            return node["content"]
        return None 
    
    def get_pwd(self):

        return self.current_path

    def has_permission(self, node, user):
        # root tem acesso total
        if user == "root":
            return True

        # dono do arquivo
        if node["owner"] == user:
            return True

        # permissao de leitura para outros
        if node["perm"][2] == "4":
            return True

        return False