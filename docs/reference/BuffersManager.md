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