# 3D wireframe renderer
Projects orthographic view of given 3D model on single colour OLED screen, written for personal use, but can be used for anything.

### WIP: 
- Perspective Projection is yet to be implemented after proper camera system is implemented.
- Shading faces can be inefficient, as the output is black and white, and provides small benefit in visual fidelity, so less likely to be implemented.
 
### Steps
- Copy the obj file to this folder,
- Use the given python script "```extract_from_obj.py```" to generate a c header file for the model, (already imported in index.cpp).
- Modify the scaling factor of the model in the ```project_orthographic()``` function file of index.cpp file to fit the object in the screen.
- Compile using cmake
- Copy the file .uf2 file to Raspberry Pi Pico

### Test without RPi Pico
[This repository](https://github.com/aharnishp/3D-to-2D-Projection) uses same architecture for drawing, only the printing is done on char buffer instead of oled buffer and printed in stdout.

### Credits
Used the great [pico_ssd](https://github.com/Harbys/pico-ssd1306) library as backend for interfacing with OLED screen, to set pixels to OLED. The line drawer even though provided, is written from scratch.
