class VirtualFileSystem:

    def __init__(self, fs_data):

        self.fs = fs_data
        self.current_path = "/home/guest"

    def _resolve_path(self, path):

        if path.startswith("/"):
            parts = path.strip("/").split("/")
        else:
            parts = self.current_path.strip("/").split("/") + path.split("/")
        return [p for p in parts if p]

    def _get_node(self, path_parts):

        node = self.fs["/"]

        for part in path_parts:
            if part in node:
                node = node[part]
            else:
                return None
        return node
    
    def list_dir(self, path=None):

        if path is None:
            path = self.current_path

        parts = self._resolve_path(path)
        node = self._get_node(parts)

        if isinstance(node, dict):
            return list(node.keys())
        return []
    
    def change_dir(self, path):

        parts = self._resolve_path(path)
        node = self._get_node(parts)

        if isinstance(node, dict):
            self.current_path = "/" + "/".join(parts)
            return True
        return False

    # backward compatibility
    cahnge_dir = change_dir

    def read_file(self, filename):

        parts = self._resolve_path(filename)
        node = self._get_node(parts)

        if isinstance(node, str):
            return node
        return None 
    
    def get_pwd(self):

        return self.current_path
