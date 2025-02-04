# Introduction to Gigibox
Gigibox is a (half) optimized graphical interface capable of (may change as this project gets updated):
- Displaying the screen using two graphical buffers that switch per frame
- Creating displayable sprites from custom formatted images
- Interpreting pseudo-assembly code able to access all available tools provided

# What powers it?
The graphics side of the project (drawing to the window n' stuff) is powered by SFML3, but it may get switched over to SDL3 in the future.

# Limitations?
As of now, Gigibox is extremely limited and barebones, only being able to display sprites and moving them.

# Coming features
There are features that will be added as soon as possible. These are:
- Input support. The registers will expand to support limited input from either keyboard or controller
- Slight code cleaning. The code has been rushed and is riddled with commented out and unoptimized code blocks
- Image/Sprite rework. These were also rushed and need to be cleaned up (palette system needs to be moved to the sprites object and certain properties have to be joined)