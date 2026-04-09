import hashlib
import re

class HashEngine:

    def generate(self, data):

        if isinstance(data, bytes):
            data = data.decode('utf-8', errors='ignore')
        return{
            "sha256": hashlib.sha256(data.encode()).hexdigest(),
        }
    
    def identify(self, hash_value):

        # limpeza forte
        hash_value = hash_value.strip().lower()

        # remove qualquer espaço interno (extra segurança)
        hash_value = hash_value.replace(" ", "")

        length = len(hash_value)

        # valida se é hexadecimal real
        if not re.fullmatch(r"[0-9a-f]+", hash_value):
            return "Unknown"

        if length == 64:
            return "SHA256"

        return "Unknown"
        