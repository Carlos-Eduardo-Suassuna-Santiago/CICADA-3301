from PIL import Image


class StegoEngine:

    def extract_lsb(self, image_path):

        try:
            img = Image.open(image_path)
            pixels = list(img.getdata())

            binary_data = ""

            for pixel in pixels:

                for channel in pixel[:3]:  # RGB
                    binary_data += str(channel & 1)

            chars = []

            for i in range(0, len(binary_data), 8):

                byte = binary_data[i:i+8]

                if len(byte) < 8:
                    break

                char = chr(int(byte, 2))

                if char == "#":  # delimitador
                    break

                chars.append(char)

            return ''.join(chars)

        except Exception as e:
            return f"error: {str(e)}"