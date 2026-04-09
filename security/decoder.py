import base64

class Decoder:

    def __init__(self):
        # Morse code dictionary
        self.morse_dict = {
            '.-': 'A', '-...': 'B', '-.-.': 'C', '-..': 'D', '.': 'E',
            '..-.': 'F', '--.': 'G', '....': 'H', '..': 'I', '.---': 'J',
            '-.-': 'K', '.-..': 'L', '--': 'M', '-.': 'N', '---': 'O',
            '.--.': 'P', '--.-': 'Q', '.-.': 'R', '...': 'S', '-': 'T',
            '..-': 'U', '...-': 'V', '.--': 'W', '-..-': 'X', '-.--': 'Y',
            '--..': 'Z', '.----': '1', '..---': '2', '...--': '3', '....-': '4',
            '.....': '5', '-....': '6', '--...': '7', '---..': '8', '----.': '9',
            '-----': '0'
        }

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
        
    def decode_morse(self, data):
        try:
            words = data.split(' / ')
            decoded_words = []
            for word in words:
                letters = word.split()
                decoded_word = ''.join([self.morse_dict.get(letter, '?') for letter in letters])
                decoded_words.append(decoded_word)
            return ' '.join(decoded_words)
        except:
            return None
        
    def decode_atbash(self, data):
        try:
            result = []
            for char in data.upper():
                if char.isalpha():
                    # A=65, Z=90 in ASCII
                    result.append(chr(155 - ord(char)))
                else:
                    result.append(char)
            return ''.join(result)
        except:
            return None
        
    def decode_railfence(self, data, rails=3):
        try:
            if rails <= 1:
                return data
            
            # Create the rail pattern
            pattern = []
            for rail in range(rails):
                rail_pattern = []
                pos = rail
                direction = 1 if rail == 0 else -1
                
                while pos < len(data):
                    rail_pattern.append(pos)
                    if rail == 0 or rail == rails - 1:
                        pos += 2 * (rails - 1)
                    else:
                        pos += 2 * rail if direction == 1 else 2 * (rails - 1 - rail)
                        direction *= -1
                pattern.append(rail_pattern)
            
            # Read in rail order
            result = [''] * len(data)
            data_pos = 0
            for rail in range(rails):
                for pos in pattern[rail]:
                    if data_pos < len(data):
                        result[pos] = data[data_pos]
                        data_pos += 1
            
            return ''.join(result)
        except:
            return None
        
    def decode_bacon(self, data):
        try:
            # Bacon cipher: A=AAAAA, B=AAAAB, etc.
            bacon_dict = {}
            for i in range(26):
                binary = format(i, '05b').replace('0', 'A').replace('1', 'B')
                bacon_dict[binary] = chr(65 + i)
            
            # Remove spaces and convert to uppercase
            data = data.replace(' ', '').upper()
            
            # Split into 5-character groups
            result = []
            for i in range(0, len(data), 5):
                chunk = data[i:i+5]
                if len(chunk) == 5:
                    result.append(bacon_dict.get(chunk, '?'))
            
            return ''.join(result)
        except:
            return None
        
    def auto_decode(self, data):
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

        # Morse
        result = self.decode_morse(data)
        if result and result != data:
            return "[Morse] " + result

        # Atbash
        result = self.decode_atbash(data)
        if result and result != data:
            return "[Atbash] " + result

        return "Unable to decode the input data."