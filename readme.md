# Attribute-Based Toon Shading with Feature Lines
Created for CPSC 591 (Rendering) at the University of Calgary

Project Report: https://docs.google.com/document/d/1Dj_-SBweHE-X1OMhRRCxWBiqAyI7QMcfnIsm-22z0HU/edit?usp=sharing

Controls
========
**Light Controls:**  
W: Move light up  
A: Move light left  
S: Move light down  
D: Move light right  
Q: Move light backwards  
E: Move light forwards  
(NOTE: all directions are relative to the starting location of the camera)

**Texture Controls:**  
U: Switch to multiplicitive texture combination  
I: Switch to additive texture combination  
O: Switch to attribute-based texture only  
P: Switch to image-based texture only  
H: Toggle between colour and greyscale atribute-based textures  
J: Toggle between orientation- and depth-based textures  
K: Use previous attribute-based texture  
L: Use next attribute-based texture  
N: Decrease value of r (orientation-based texturing variable)  
M: Increase value of r (orientation-based texturing variable)

**Line Rendering Controls:**  
1: Decrease lower bound for contours  
2: Increase lower bound for contours  
3: Decrease upper bound for contours  
4: Increase upper bound for contours  
5: Decrease line thickness  
6: Increase line thickness  
(NOTE: due the the way the system works, these variables are per object and not global)  
C: Toggle line drawing on and off  

**Object Swapping Controls:**  
Z: Use previous object  
X: Use next object  

Compiling and Running
=====================
Runing the executable 'run' will clean, make, and run the project  
Command: ./run  
