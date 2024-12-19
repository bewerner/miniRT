![Title_Image](docs/title.png)

# Contents

[Introduction](#introduction) \
[Features](#features) \
[Rendered Scenes](#rendered-scenes) \
[Dependencies](#dependencies) \
[Installation](#installation) \
[Usage](#usage) \
[Controls](#controls) \
[Scene format](scene-format) \
[Assets](#assets)

# Introduction

This project started out as a team project at [42 Heilbronn Coding School](https://www.42heilbronn.de/) together with [nmihaile](https://github.com/nmihaile).
While the original subject asks for a simple CPU Raytracer, we wanted to learn more about graphics programming and try to create something more complex with openGL for the GPU.

After concluding the team project, I continued to work on making it more physically accurate using [Blender](https://www.blender.org) [Cycles](https://www.blender.org/features/rendering/) as a reference, restructure things, add more features and create new scenes. I probably spent the most time on my multiple importance sampling implementation for the HDRi environment maps. It turned out presentable, capable of rendering nice looking scenes, but it's very much a naive and hacky implementation. The same goes for many of the more complex parts of the program, but I'm happy with how it turned out in the end and looking forward to learn more in the future.

# Features

- HDRi Enviromnemnt Maps illuminate the scene using multiple importance sampling.
- Material properties (Roughness, Metallic, Base Color) can be values or textures
- Normal Maps
- Point Lights (with radius)
- Primitives: Sphere, Cylinder, Hyperboloid and Plane
	<br><br>
	<img src="docs/primitives__80439_samples.png" width="97%" />
	<br><br>
- AgX color mapping (I created my own 3D LUT using Blender)
	<br><br>
	<img src="docs/color_agx.png" width="32%" />
	<img src="docs/color_standard.png" width="32%" /> 
	<img src="docs/color_raw.png" width="32%" />
	<br><br>
- Reflections and Bounce Light (glossy/diffuse bounces)
	<br><br>
	<img src="docs/box_1__59302_samples.png" width="32%" />
	<img src="docs/box_2__84323_samples.png" width="32%" /> 
	<img src="docs/box_3__95201_samples.png" width="32%" />
	<br><br>
- Viewport Shading Modes: Render, Solid and Normal (with Blender-like Gizmo)
	<br><br>
	<img src="docs/poker2__10753_samples.png" width="32%" />
	<img src="docs/poker2__solid.png" width="32%" /> 
	<img src="docs/poker2__normal.png" width="32%" />
	<br><br>
- Adaptive renderscale during viewport navigation to ensure smooth FPS
	<br><br>
	<img src="docs/renderscale.gif" width="97%" />
	<br><br>


# Rendered Scenes

`road.rt`\
![road](docs/road__2323_samples.png)

`metal.rt`\
![metal](docs/metal__62213_samples.png)

`ocean.rt`\
![ocean](docs/ocean__16513_samples.png)

`poker.rt`\
![poker](docs/poker__11227_samples.png)

`poker2.rt`\
![poker2](docs/poker2__10753_samples.png)

`wireframe_cube.rt`\
![wireframe_cube](docs/wireframe_cube__31029_samples.png)

`infinity_mirror.rt`\
![infinity_mirror](docs/infinity_mirror__562_samples.png)

`random_materials.rt`\
![random_materials](docs/random_materials__16124_samples.png)

`rainbow.rt`\
![rainbow](docs/rainbow__13005_samples.png)

`checkerboard.rt`\
![checkerboard](docs/checkerboard__192139_samples.png)

`soft_light.rt`\
![soft_light](docs/soft_light__7372_samples.png)

`caleidoscope.rt` \
![caleidoscope](docs/caleidoscope.gif)
&nbsp;&nbsp;&nbsp;( *hold <kbd>↑</kbd> to animate* )
<br><br>

# Comparing with Blender Cycles

### Roughness from `0.0` to `1.0`

Cycles<br>
<img src="docs/compare/blender/roughness_000.png" width="15%" />
<img src="docs/compare/blender/roughness_020.png" width="15%" />
<img src="docs/compare/blender/roughness_040.png" width="15%" />
<img src="docs/compare/blender/roughness_060.png" width="15%" />
<img src="docs/compare/blender/roughness_080.png" width="15%" />
<img src="docs/compare/blender/roughness_100.png" width="15%" /><br>
miniRT<br>
<img src="docs/compare/miniRT/roughness_000__18518_samples.png" width="15%" />
<img src="docs/compare/miniRT/roughness_020__18507_samples.png" width="15%" />
<img src="docs/compare/miniRT/roughness_040__18129_samples.png" width="15%" />
<img src="docs/compare/miniRT/roughness_060__18413_samples.png" width="15%" />
<img src="docs/compare/miniRT/roughness_080__17660_samples.png" width="15%" />
<img src="docs/compare/miniRT/roughness_100__17876_samples.png" width="15%" /><br>
<br>


### Metallic from `0.0` to `1.0`
Cycles<br>
<img src="docs/compare/blender/metallic_000.png" width="15%" />
<img src="docs/compare/blender/metallic_020.png" width="15%" />
<img src="docs/compare/blender/metallic_040.png" width="15%" />
<img src="docs/compare/blender/metallic_060.png" width="15%" />
<img src="docs/compare/blender/metallic_080.png" width="15%" />
<img src="docs/compare/blender/metallic_100.png" width="15%" /><br>
miniRT<br>
<img src="docs/compare/miniRT/metallic_000__15348_samples.png" width="15%" />
<img src="docs/compare/miniRT/metallic_020__15343_samples.png" width="15%" />
<img src="docs/compare/miniRT/metallic_040__15339_samples.png" width="15%" />
<img src="docs/compare/miniRT/metallic_060__15147_samples.png" width="15%" />
<img src="docs/compare/miniRT/metallic_080__15316_samples.png" width="15%" />
<img src="docs/compare/miniRT/metallic_100__18569_samples.png" width="15%" /><br>
<span><br></span>

### IOR (Index of Refraction) from `1.0` to `2.0`
Cycles<br>
<img src="docs/compare/blender/ior_100.png" width="15%" />
<img src="docs/compare/blender/ior_120.png" width="15%" />
<img src="docs/compare/blender/ior_140.png" width="15%" />
<img src="docs/compare/blender/ior_160.png" width="15%" />
<img src="docs/compare/blender/ior_180.png" width="15%" />
<img src="docs/compare/blender/ior_200.png" width="15%" /><br>
miniRT<br>
<img src="docs/compare/miniRT/ior_100__15512_samples.png" width="15%" />
<img src="docs/compare/miniRT/ior_120__15519_samples.png" width="15%" />
<img src="docs/compare/miniRT/ior_140__15514_samples.png" width="15%" />
<img src="docs/compare/miniRT/ior_160__15514_samples.png" width="15%" />
<img src="docs/compare/miniRT/ior_180__15536_samples.png" width="15%" />
<img src="docs/compare/miniRT/ior_200__15534_samples.png" width="15%" /><br>
<br>

# Dependencies

#### Debian / Ubuntu
```
sudo apt update
sudo apt install build-essential libx11-dev libglfw3-dev libglfw3 xorg-dev
```

#### Arch (x11)
```
sudo pacman -S glfw-x11
```

#### Arch (wayland)
```
sudo pacman -S glfw-wayland
```

#### MacOS
```
brew install glfw
```

# Installation

```
git clone https://github.com/bewerner/FdF.git
cd FdF
make
```

# Usage

```
./miniRT path_to_scene_file.rt
```

# Controls

# Scene format

The `.rt` scene format describes the scene with identifiers followed by it's specific parameters.
Parameters marked with `*` are optional.

Positions and directions are specified as `vec3`.
Colors can be specified as `u8vec3` (RGB values beween 0-255). Most color parameters can alternatively be mapped to a texture.
Others are specified as `float`.

### Camera

```
id    position    view-direction    field of view (horizontal)    f-stop*    focus-distance*
C     vec3        vec3              float                         float      float
```

### Ambient

```
id    strength    color     
A     float       u8vec3/texture
```

# Assets
