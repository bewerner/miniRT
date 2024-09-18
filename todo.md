┌───────────┐
│ Mandatory │
└───────────┘

	- init_mini RT
		* only init textures or bindings
		* split it up and norminette it

	- parser
		* checker texture
			* IDX, SCALE, COL1, COL2
			* UBO (max 100)
				* SCALE, COL1, COL2
			# TEXTURES
			tex wood assets/textures/wood.png
			tex c1 checker	1.0 	0,0,34		05,255,255
			tex c2 checker	2.0 	0,0,0 		255,255,2550

			# MATERIALS
			#	name                          color             metallic       roughness      ior            transmission   emission_strength   emission_color
			mat	Material                      c1                c2             1.000000       1.450000       0.000000       0.000000            255,255,255

	- random
		* better random

	- bump-maps

	- Materails use all parameters
		* …	
		* better use of refelctions
			* better calculations
			* glossy bounce limit (nice to have)

	- norminette

	- merging of MANDATORY && BONUS

┌──────────────┐
│ Nice to have │
└──────────────┘

	- achsen gizmo

	- Diffuse Bounces

	- Depth of Field

	- Triangles
		* new object
		* get_hitpoint_triangle
		* UV Triangle
		* Vertex Normals (smooth shading)

	- Planes
		* additional UV Scalar parameter (optional)

	- Texture:
		* parsing / png loader
		* TBO (count 31 - 4 // evtl max. count 20)
		* use texture as color
			* use texture for all parameters

	- light gizmos
		* lightcolor indication
		* gizmos texture -> as sprite
			* use of vertex shader

	- Exporter
		* trigger ueber key_hook // UI-BTN
		* .ppm
		* .png

	- 3D Noise Texture

┌─────────────────┐
│ If we have time │
└─────────────────┘

	- selectable objects
		* render outline

	- hdri env maps (ambient)

	- BVH (Bounding volume hierarchy)

	- OBJ loader

	- UI

	- parser
		* BETTER ERROR MSG with line number and more context
	
	- render scale

	- viewport navigation like blender (rotate around anchor point)
