# Image
The Image is a type of data that doesn't do much on its own. it needs to be utilized by a sprite to be deemed useful

Namespace: Gigi

# Composition
## Properties
- width
    - type: uint8_t
    - description: the width of the image
- height
    - type: uint8_t
    - description: the height of the image
- <ins>palette</ins>
    - type: vector\<sf::Color\>
    - description: where all available colors are stored
- alpha
    - type: bool
    - description: whether the image supports transparency or not
- alphaColor
    - type: sf::Color
    - description: the color to be treated as transparent

- pixels
    - type: uint8_t*
    - description: a buffer of the image's pixels
- <ins>pixelsSize</ins>
    - type: size_t
    - description: the size of the pixels buffer

## Functions
- Gigi_Image(uint8_t* data8b, size_t size = 0)
    - description: default constructor