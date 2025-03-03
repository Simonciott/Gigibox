# Sprite
Assembly versions of existing objects are used to generate seemingly identical objects to their non-Assembly counterparts that directly use the Data Memory to store their properties.

The Sprite is the most basic drawable type in Gigibox. This means all regular drawables will inherit from the Sprite object

Namespace: Gigi::Assembly

# Composition
## Properties
- spriteIndex
    - type: short*
    - description: the address of the sprite in memory

## Functions
- Gigi_Sprite(Gigi_Image& img)
    - description: default constructor
- Draw(<ins>vector\<Assembly::Image\>& images</ins>)
    - type: void
    - description: draws the sprite's pixels to the back buffer. <ins>images is used as a reference to storedImages in Assembly::Registers.</ins>

# Data formatting
| index  |  0 |  1  | 2 | 3 | 4 |
|--------|----|-----|---|---|---|
|property|txt | pal | x | y | v |

- txt: image address in memory
- pal: the palette address in memory
- x: x coordinate
- y: y coordinate
- v: visible, true if its value is anything other than 0