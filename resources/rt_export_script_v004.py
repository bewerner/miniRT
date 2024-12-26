import bpy
import math
import mathutils

# Prepare the output file
output_file_path = bpy.path.abspath("/Users/bwerner/Documents/projects/rank04/miniRT/git_miniRT/scenes/scene.rt")

def get_material_info(material):
    # Default values if the material does not use nodes
    color = (204, 204, 204)
    metallic = 0.0
    roughness = 0.5
    ior = 1.45  # Default IOR for glass
    transmission = 0.0
    emission_strength = 0.0
    emission_color = (0, 0, 0)
    
    if material.node_tree:
        for node in material.node_tree.nodes:
            if node.type == 'BSDF_PRINCIPLED':
                # Get base color
                base_color = node.inputs['Base Color'].default_value
                color = (
                    int(base_color[0] * 255),
                    int(base_color[1] * 255),
                    int(base_color[2] * 255)
                )
                metallic = node.inputs['Metallic'].default_value
                roughness = node.inputs['Roughness'].default_value
                ior = node.inputs['IOR'].default_value
                # Handle transmission with safe access
                transmission = node.inputs[17].default_value
                emission_strength = node.inputs[27].default_value
                emission_color = node.inputs[26].default_value
                emission_color = (
                    int(emission_color[0] * 255),
                    int(emission_color[1] * 255),
                    int(emission_color[2] * 255)
                )
    
    return {
        'name': material.name.replace(' ', '_'),
        'color': color,
        'metallic': metallic,
        'roughness': roughness,
        'ior': ior,
        'transmission': transmission,
        'emission_strength': emission_strength,
        'emission_color': emission_color,
    }

def is_sphere(obj):
    return obj.type == 'MESH' and 'Sphere' in obj.name

def get_sphere_info(obj):
    location = (round(obj.location.x, 10), 
                round(obj.location.y, 10), 
                round(obj.location.z, 10))
    
    dimensions = obj.dimensions
    largest_dimension = max(dimensions)
    
    material_name = "None"
    
    if obj.active_material:
        material_name = obj.active_material.name
    
    return location, largest_dimension, material_name

def is_plane(obj):
    return obj.type == 'MESH' and 'Plane' in obj.name

def get_plane_info(obj):
    location = (round(obj.location.x, 10), 
                round(obj.location.y, 10), 
                round(obj.location.z, 10))
    
    normal = obj.matrix_world.to_3x3() @ mathutils.Vector((0.0, 0.0, 1.0))
    normal = (round(normal.x, 10), 
              round(normal.y, 10), 
              round(normal.z, 10))
    
    material_name = "None"
    
    if obj.active_material:
        material_name = obj.active_material.name
    
    return location, normal, material_name

def is_cylinder(obj):
    return obj.type == 'MESH' and 'Cylinder' in obj.name

def get_cylinder_info(obj):
    location = (round(obj.location.x, 10), 
                round(obj.location.y, 10), 
                round(obj.location.z, 10))
    
    diameter = obj.dimensions.x
    height = obj.dimensions.z
    
    orientation = obj.matrix_world.to_3x3() @ mathutils.Vector((0.0, 0.0, 1.0))
    orientation = (round(orientation.x, 10), 
                   round(orientation.y, 10), 
                   round(orientation.z, 10))
    
    material_name = "None"
    
    if obj.active_material:
        material_name = obj.active_material.name
    
    return location, orientation, diameter, height, material_name

def is_pointlight(obj):
    return obj.type == 'LIGHT' and obj.data.type == 'POINT'

def get_pointlight_info(obj):
    location = (round(obj.location.x, 10), 
                round(obj.location.y, 10), 
                round(obj.location.z, 10))
    
    power = obj.data.energy
    
    color = obj.data.color
    color = (
        int(color[0] * 255),
        int(color[1] * 255),
        int(color[2] * 255)
    )
        
    return location, power, color

def is_camera(obj):
    return obj.type == 'CAMERA' and obj.data.type == 'PERSP'

def get_camera_info(obj):
    location = (round(obj.location.x, 10), 
                round(obj.location.y, 10), 
                round(obj.location.z, 10))
    
    fov = math.degrees(obj.data.angle)
    
    direction = obj.matrix_world.to_3x3() @ mathutils.Vector((0.0, 0.0, -1.0))
    
    direction = (round(direction.x, 10), 
                 round(direction.y, 10), 
                 round(direction.z, 10))
        
    return location, fov, direction

def get_world_background_info():
    world = bpy.context.scene.world
    if not world or not world.node_tree:
        color = (0, 0, 0)
        strength = 0
        return color, strength
    
    nodes = world.node_tree.nodes
    for node in nodes:
        if node.type == 'BACKGROUND':
            color = node.inputs['Color'].default_value
            color = (
                int(color[0] * 255),
                int(color[1] * 255),
                int(color[2] * 255)
                )
            strength = node.inputs['Strength'].default_value
            return color, strength

    color = (0, 0, 0)
    strength = 0
    return color, strength

# Lists to hold object information
cameras = []
pointlights = []
spheres = []
planes = []
cylinders = []

with open(output_file_path, 'w') as file:
    # Write material information at the top of the file
    file.write(f"# MATERIALS\n")
    file.write(f"#	name                          color             metallic       roughness      ior            transmission   emission_strength   emission_color\n")
    for material in bpy.data.materials:
        if material.name != 'Dots Stroke':
            material_info = get_material_info(material)
            file.write(f"mat\t{material_info['name']:<30}"
                    f"{material_info['color'][0]:>3},{material_info['color'][1]:>3},{material_info['color'][2]:>3}       "
                    f"{material_info['metallic']:<15.6f}"
                    f"{material_info['roughness']:<15.6f}"
                    f"{material_info['ior']:<15.6f}"
                    f"{material_info['transmission']:<15.6f}"
                    f"{material_info['emission_strength']:<20.6f}"
                    f"{material_info['emission_color'][0]:>3},{material_info['emission_color'][1]:>3},{material_info['emission_color'][2]:>3}\n")

    # Collect object information
    for obj in bpy.context.scene.objects:
        if is_camera(obj):
            location, fov, direction = get_camera_info(obj)
            cameras.append(f"C\t{location[0]:>12.6f},{location[1]:>12.6f},{location[2]:>12.6f}      "
                           f"{direction[0]:>12.6f},{direction[1]:>12.6f},{direction[2]:>12.6f}             "
                           f"{fov:<15.6f}\n")
        elif is_pointlight(obj):
            location, power, color = get_pointlight_info(obj)
            pointlights.append(f"l\t{location[0]:>12.6f},{location[1]:>12.6f},{location[2]:>12.6f}          "
                               f"{power:<15.6f}"
                               f"{color[0]:>3},{color[1]:>3},{color[2]:>3}\n")
        elif is_sphere(obj):
            location, largest_dimension, material_name = get_sphere_info(obj)
            spheres.append(f"sp\t{location[0]:>12.6f},{location[1]:>12.6f},{location[2]:>12.6f}          "
                           f"{largest_dimension:<15.6f}"
                           f"{material_name:<20}\n")
        elif is_plane(obj):
            location, normal, material_name = get_plane_info(obj)
            planes.append(f"pl\t{location[0]:>12.6f},{location[1]:>12.6f},{location[2]:>12.6f}      "
                           f"{normal[0]:>12.6f},{normal[1]:>12.6f},{normal[2]:>12.6f}             "
                           f"{material_name:<20}\n")
        elif is_cylinder(obj):
            location, orientation, diameter, height, material_name = get_cylinder_info(obj)
            cylinders.append(f"cy\t{location[0]:>12.6f},{location[1]:>12.6f},{location[2]:>12.6f}      "
                             f"{orientation[0]:>12.6f},{orientation[1]:>12.6f},{orientation[2]:>12.6f}             "
                             f"{diameter:<15.6f}"
                             f"{height:<15.6f}"
                             f"{material_name:<20}\n")

    # Write sorted object information to file
    if spheres:
        file.write(f"\n# SPHERES")
        file.write(f"\n#\torigin                                          diameter       material\n")
        for sphere in spheres:
            file.write(sphere)
    if cylinders:
        file.write(f"\n# CYLINDERS")
        file.write(f"\n#\torigin                                          orientation                                    diameter       height         material\n")
        for cylinder in cylinders:
            file.write(cylinder)
    if planes:
        file.write(f"\n# PLANES")
        file.write(f"\n#\torigin                                          normal                                         material\n")
        for plane in planes:
            file.write(plane)
    if pointlights:
        file.write(f"\n# LIGHTS")
        file.write(f"\n#\torigin                                          power          color\n")
        for pointlight in pointlights:
            file.write(pointlight)
    if cameras:
        file.write(f"\n# CAMERA")
        file.write(f"\n#\torigin                                          orientation                                    fiew of view\n")
        for camera in cameras:
            file.write(camera)

    # World background information
    file.write(f"\n# AMBIENT")
    file.write(f"\n#\tstrength             color\n")
    color, strength = get_world_background_info()
    file.write(f"A\t{strength:<20.6f}{color[0]:>3},{color[1]:>3},{color[2]:>3}\n")
