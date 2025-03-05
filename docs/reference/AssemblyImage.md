# Assembly Image
Assembly versions of existing objects are used to generate seemingly identical objects to their non-Assembly counterparts that directly use the Data Memory to store their properties.

The Image is a type of data that doesn't do much on its own. it needs to be utilized by a sprite to be deemed useful

Namespace: Gigi::Assembly

# Composition
## Properties
- index
    - type: short*
    - description: the image's address in memory

- pixels
    - type: uint8_t*
    - description: a buffer of the image's pixels
- <ins>pixelsSize</ins>
    - type: size_t
    - description: the size of the pixels buffer

## Functions
- Gigi_Image(uint8_t* data8b, size_t size = 0)
    - description: default constructor

# Data formatting
|  index |  0 |  1  |   2  |     3     |  4  |     5    |
|--------|----|-----|------|-----------|-----|----------|
|property| img|width|height|paletteSize|alpha|alphaColor|

- img: image index in the image vector
- width: the width of the image
- height: the height of the image
- paletteSize: how many colors are in the image's palette
- alpha: whether the image supports transparency or not
- alphaColor: the color to be treated as transparent