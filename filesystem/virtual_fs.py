"""Module for the virtual_fs component of the CICADA-3301 application."""

class VirtualFileSystem:
    """In-memory virtual filesystem used by the simulation for file and directory operations."""

    def __init__(self, fs_data):
        """Initialize the object state."""

        self.fs = fs_data
        self.current_path = "/"

    def _resolve_path(self, path):
        """_resolve_path function."""

        if path.startswith("/"):
            parts = path.strip("/").split("/")
        else:
            parts = self.current_path.strip("/").split("/") + path.split("/")

        normalized = []
        for part in parts:
            if part == "" or part == ".":
                continue
            if part == "..":
                if normalized:
                    normalized.pop()
                continue
            normalized.append(part)

        return normalized

    def _get_node(self, parts):
        """_get_node function."""

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
    
    def list_dir(self, path=None, user=None):
        """List dir in the current virtual filesystem or session."""

        if path is None:
            path = self.current_path

        parts = self._resolve_path(path)
        node = self._get_node(parts)

        if node and node["type"] == "dir" and self.has_permission(node, user):
            return [(f, node["content"][f]["type"]) for f in node["content"].keys() if self.has_permission(node["content"][f], user)]
        return []
    
    def change_dir(self, path, user):
        """Change the dir within the current environment."""

        parts = self._resolve_path(path)

        node = self._get_node(parts)

        if node and node["type"] == "dir" and self.has_permission(node, user):

            self.current_path = "/" + "/".join(parts)

            if self.current_path == "/":
                self.current_path = "/"

            return True

        return False


    def read_file(self, filename, user):
        """Read the specified file from the virtual filesystem."""

        parts = self._resolve_path(filename)
        node = self._get_node(parts)

        if node and node["type"] == "file" and self.has_permission(node, user):
            # check parent directories permissions
            current = self.fs["/"]
            for p in parts[:-1]:
                if not self.has_permission(current, user):
                    return None
                current = current["content"][p]
            return node["content"]
        return None 
    
    def get_pwd(self):
        """Return the pwd from the current state."""

        return self.current_path

    def has_permission(self, node, user):
        """Check whether the permission is available or permitted."""
        # root tem acesso total
        if user == "root":
            return True

        # dono do arquivo
        if node["owner"] == user:
            return True

        # permissao de leitura para outros
        if int(node["perm"][2], 8) & 4:  # check read bit for others
            return True

        return False

    def create_file(self, path, content, owner="root", perm="644"):
        """Create a new resource inside the virtual filesystem."""

        parts = path.strip("/").split("/")
        filename = parts[-1]
        dir_path = parts[:-1]

        current = self.fs["/"]

        for p in dir_path:
            if p not in current["content"]:
                current["content"][p] = {"type": "dir", "content": {}, "owner": owner, "perm": perm}
            current = current["content"][p]

        current["content"][filename] = {
            "type": "file",
            "content": content,
            "owner": owner,
            "perm": perm,
        }

    def copy_file(self, source_path, dest_path, owner="root", perm="644", user="root"):
        """Copy a file from source to destination."""
        # Read source file
        source_content = self.read_file(source_path, user)
        if source_content is None:
            return False
        
        # Create destination file with same content
        self.create_file(dest_path, source_content, owner, perm)
        return True

    def list_dir_full(self, path_parts):
        """List dir_full in the current virtual filesystem or session."""

        node = self._get_node(path_parts)

        if node and node["type"] == "dir":
            return node["content"].keys()
        return []

    def resolve_partial(self, path):
        """resolve_partial function."""

        if path.startswith("/"):
            parts = path.strip("/").split("/")
        else:
            current = self.current_path.strip("/").split("/") if self.current_path != "/" else []
            parts = current + path.split("/")

        normalized = []
        for part in parts:
            if part == "" or part == ".":
                continue
            if part == "..":
                if normalized:
                    normalized.pop()
                continue
            normalized.append(part)

        return normalized
