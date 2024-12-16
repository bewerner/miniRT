![Title_Image](docs/title.png)

# Description

This project started out as a team project at [42 Heilbronn Coding School](https://www.42heilbronn.de/) together with [nmihaile](https://github.com/nmihaile).
While the original subject asks for a simple CPU Raytracer, we wanted to learn more about graphics programming and try to create something more complex with openGL for the GPU.

# Features

- HDRi Enviromnemnt Maps illuminate the scene using multiple importance sampling.
- Material properties (Roughness, Metallic, Base Color) can be values or textures
- Normal Maps
- Point Lights (with radius)
- Primitives: Sphere, Cylinder, Plane and Hyperboloid

  [*primitives.rt image*]
- AgX color view transform (I created my own 3D LUT using Blender)

  [*raw vs agx images*]
- Reflections and Bounce Light (glossy/diffuse bounces)
  <p float="left">
	  <img src="docs/box_1__59302_samples.png" width="32%" />
	  <img src="docs/box_2__84323_samples.png" width="32%" /> 
	  <img src="docs/box_3__95201_samples.png" width="32%" />
  </p>
- Viewport Shading Modes: Render, Solid and Normal (with Blender-like Gizmo showing orientation)
  <p float="left">
	  <img src="docs/road__16261_samples.png" width="32%" />
	  <img src="docs/solid.png" width="32%" /> 
	  <img src="docs/normal.png" width="32%" />
  </p>
- Adaptive renderscale during viewport navigation

  [*gif/video showing viewport navigation*]


# Renders

![road](docs/road__16261_samples.png)
![random_materials](docs/random_materials__16124_samples.png)
![metal](docs/metal__62213_samples.png)
![rainbow](docs/rainbow__13005_samples.png)
![checkerboard](docs/checkerboard__192139_samples.png)
![ocean](docs/ocean__16513_samples.png)


# Comparing with Blender Cycles

### Roughness from `0.0` to `1.0`
<p float="left">
	<span>Cycles<br></span>
	<img src="docs/compare/blender/roughness_000.png" width="15%" />
	<img src="docs/compare/blender/roughness_020.png" width="15%" />
	<img src="docs/compare/blender/roughness_040.png" width="15%" />
	<img src="docs/compare/blender/roughness_060.png" width="15%" />
	<img src="docs/compare/blender/roughness_080.png" width="15%" />
	<img src="docs/compare/blender/roughness_100.png" width="15%" />
	<span><br>miniRT‎<br></span>
	<img src="docs/compare/blender/roughness_000.png" width="15%" />
	<img src="docs/compare/blender/roughness_020.png" width="15%" />
	<img src="docs/compare/blender/roughness_040.png" width="15%" />
	<img src="docs/compare/blender/roughness_060.png" width="15%" />
	<img src="docs/compare/blender/roughness_080.png" width="15%" />
	<img src="docs/compare/blender/roughness_100.png" width="15%" />
</p>

<span><br></span>

### Metallic from `0.0` to `1.0`
<p float="left">
	<span>Cycles<br></span>
	<img src="docs/compare/blender/metallic_000.png" width="15%" />
	<img src="docs/compare/blender/metallic_020.png" width="15%" />
	<img src="docs/compare/blender/metallic_040.png" width="15%" />
	<img src="docs/compare/blender/metallic_060.png" width="15%" />
	<img src="docs/compare/blender/metallic_080.png" width="15%" />
	<img src="docs/compare/blender/metallic_100.png" width="15%" />
	<span><br>miniRT‎<br></span>
	<img src="docs/compare/blender/metallic_000.png" width="15%" />
	<img src="docs/compare/blender/metallic_020.png" width="15%" />
	<img src="docs/compare/blender/metallic_040.png" width="15%" />
	<img src="docs/compare/blender/metallic_060.png" width="15%" />
	<img src="docs/compare/blender/metallic_080.png" width="15%" />
	<img src="docs/compare/blender/metallic_100.png" width="15%" />
</p>

<span><br></span>

### IOR (Index of Refraction) from `1.0` to `2.0`
<p float="left">
	<span>Cycles<br></span>
	<img src="docs/compare/blender/ior_100.png" width="15%" />
	<img src="docs/compare/blender/ior_120.png" width="15%" />
	<img src="docs/compare/blender/ior_140.png" width="15%" />
	<img src="docs/compare/blender/ior_160.png" width="15%" />
	<img src="docs/compare/blender/ior_180.png" width="15%" />
	<img src="docs/compare/blender/ior_200.png" width="15%" />
	<span><br>miniRT‎<br></span>
	<img src="docs/compare/blender/ior_100.png" width="15%" />
	<img src="docs/compare/blender/ior_120.png" width="15%" />
	<img src="docs/compare/blender/ior_140.png" width="15%" />
	<img src="docs/compare/blender/ior_160.png" width="15%" />
	<img src="docs/compare/blender/ior_180.png" width="15%" />
	<img src="docs/compare/blender/ior_200.png" width="15%" />
</p>
