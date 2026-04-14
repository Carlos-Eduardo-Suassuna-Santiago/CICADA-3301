"""Module for the embed_stego component of the CICADA-3301 application."""

from PIL import Image


def embed_message(input_image, output_image, message):
    """embed_message function."""

    img = Image.open(input_image)
    pixels = list(img.getdata())

    binary = ''.join(format(ord(c), '08b') for c in message) + '00100011'  # #

    new_pixels = []

    data_index = 0

    for pixel in pixels:

        new_pixel = list(pixel)

        for i in range(3):

            if data_index < len(binary):

                new_pixel[i] = (new_pixel[i] & ~1) | int(binary[data_index])
                data_index += 1

        new_pixels.append(tuple(new_pixel))

    img.putdata(new_pixels)
    img.save(output_image)


embed_message("assets/secret.jpg", "assets/secret.png", "find the logs")
