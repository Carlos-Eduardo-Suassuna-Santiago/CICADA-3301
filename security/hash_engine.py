import hashlib
import re

class HashEngine:

    def generate(self, data):

        return{
            "md5": hashlib.md5(data.encode()).hexdigest(),
            "sha1": hashlib.sha1(data.encode()).hexdigest(),
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

        if length == 32:
            return "MD5"
        elif length == 40:
            return "SHA1"
        elif length == 64:
            return "SHA256"

        return "Unknown"
        