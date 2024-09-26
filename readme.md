# Mandatory

- init_miniRT.c
	- only init textures or bindings
	- split it up and norminette it

- parser
	- environment map
	- textures for metallic & roughness

- random
	- better random

- bumb-maps
	- material has new value for bumb_maps -> which are textures
	- also needs bumb strength parameter
	- gethitpoint normal do stuff
		if 2d tex
		if 3d tex

- Materails use all parameters
	- …	
	- better use of refelctions
		- better calculations
		- glossy bounce limit (nice to have)

- norminette

- merging of MANDATORY && BONUS

---
---
# Nice to have

- Diffuse Bounces

- Depth of Field

- Triangles
	- new object
	- get_hitpoint_triangle
	- UV Triangle
	- Vertex Normals (smooth shading)

- Planes
	- additional UV Scalar parameter (optional)

- Texture:
	- TBO (max. count 10) -> depends on env_map

- light gizmos
	- lightcolor indication
	- gizmos texture -> as sprite
		- use of vertex shader

- Exporter
	- trigger ueber key_hook // UI-BTN
	- .ppm
	- .png

- 3D Noise Texture

---
---
# If we have time

- selectable objects
	- render outline

- hdri env maps (ambient)

- BVH (Bounding volume hierarchy)

- OBJ loader

- UI

- parser
	- BETTER ERROR MSG with line number and more context

- render scale

- viewport navigation like blender (rotate around anchor point)

- grid floor in solid mode (like blender)
- viewport shading (mode) buttons (like blender)
