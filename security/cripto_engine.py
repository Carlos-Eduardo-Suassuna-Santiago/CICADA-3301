"""Module for the cripto_engine component of the CICADA-3301 application."""

class CryptoEngine:
    """Provide cipher decryption routines used by the decrypt command."""

    # =========================
    # CAESAR
    # =========================

    def decrypt_caesar(self, text, shift):
        """decrypt_caesar function."""

        result = "" 

        for char in text:

            if char.isalpha():

                base = ord('A') if char.isupper() else ord('a')
                result += chr((ord(char) - base - shift) % 26 + base)
            else:
                result += char

        return result

    # =========================
    # VIGENERE
    # =========================

    def decrypt_vigenere(self, text, key):
        """decrypt_vigenere function."""

        result = ""
        key_index = 0

        for char in text:

            if char.isalpha():

                base = ord('A') if char.isupper() else ord('a')
                shift = ord(key[key_index % len(key)]) - ord('A')
                result += chr((ord(char) - base - shift) % 26 + base)
                key_index += 1
            else:
                result += char

        return result
