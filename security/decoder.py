import base64

class Decoder:

    def decode_base64(self, data):

        try:
            return base64.b64decode(data).decode('utf-8')
        except (ValueError, UnicodeDecodeError):
            return None
        
    def decode_hex(self, data):

        try:
            return bytes.fromhex(data).decode('utf-8')
        except (ValueError, UnicodeDecodeError):
            return None
        
    def decode_binary(self, data):

        try:
            chars = data.split()
            return ''.join([chr(int(char, 2)) for char in chars])
        except (ValueError, UnicodeDecodeError):
            return None
        
    def auto_decode(self, data):

        if "-----BEGIN" in data and "-----END" in data:
            lines = [line.strip() for line in data.splitlines() if line.strip() and not line.strip().startswith("-----")]
            data = "".join(lines)

        # Base64
        result = self.decode_base64(data)
        if result:
            return "[Base64] " + result

        # Hex
        result = self.decode_hex(data)
        if result:
            return "[Hex] " + result

        # Binary
        result = self.decode_binary(data)
        if result:
            return "[Binary] " + result

        return "Unable to decode the input data."