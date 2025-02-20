# BuffersManager
What makes displaying the pixels to the screen possible

Namespace: Gigi

# Composition
## Properties
- firstBuffer
    - type: bool
    - description: the index of the front buffer
- texture
    - type: sf::Texture
    - description: the texture used to display the front buffer

## Functions
- getBuffer(bool firstb)
    - type: sf::Image*
    - description: gets the front buffer
- getBackBuffer()
    - type: sf::Image*
    - description: gets the back buffer
- updateTexture()
    - type: void
    - description: updates the texture to display the front buffer
- swapBuffers()
    - type: void
    - description: swaps the buffers

# Data formatting
| index | 0 | 1 | 2 | 3 |
|-------|---|---|---|---|
|property|txt| x | y | v |

- txt: image address in memory
- x: x coordinate
- y: y coordinate
- v: visible, true if its value is anything other than 0