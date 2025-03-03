# Sprite
The Sprite is the most basic drawable type in Gigibox. This means all regular drawables will inherit from the Sprite object

Namespace: Gigi

# Composition
## Properties
- x
    - type: int
    - description: the coordinates of the object on the x axis
- y
    - type: int
    - description: the coordinates of the object on the t axis
- visible
    - type: bool
    - description: whether the object is visible
- <ins>xHook</ins>
    - type: short*
    - description: the address at which the x coordinate is stored in the program's data
- <ins>yHook</ins>
    - type: short*
    - description: the address at which the y coordinate is stored in the program's data
- <ins>visibleHook</ins>
    - type: bool*
    - description: the address at which the visible property is stored in the program's data

- texture
    - type: Gigi_Image*
    - description: reference to the image the sprite is going to display

## Functions
- Gigi_Sprite(Gigi_Image& img)
    - description: default constructor
- Draw()
    - type: void
    - description: draws the sprite's pixels to the back buffer.