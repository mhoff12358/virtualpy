import sys
virtualpyloc = sys.argv[1]
sys.path.append(virtualpyloc+'Debug')
import virtualpy

import math
import virtualpy_loader
import resource_loader

def startup():
    virtualpyloc = sys.argv[1]
    resourcesloc = virtualpyloc+'resources\\'
    
    virtualpy.set_color(0.392, 0.584, 0.929)
    virtualpy.push_state()
    virtualpy.set_resources_location(resourcesloc)
    virtualpy.spawn_thread('directx')
    virtualpy_loader.debug("thread spawned")
    
    a = virtualpy.PrimitiveType.triangle_list
    
    color_vertex_type = virtualpy.VertexType(("location", "POSITION", 3), ("color", "COLOR", 4))
    texture_vertex_type = virtualpy.VertexType(("location", "POSITION", 3), ("texture", "TEXCOORD", 2))
    color_vertex_type_with_normal = virtualpy.VertexType(("location", "POSITION", 3), ("normal", "NORMAL", 3), ("color", "COLOR", 4))

    virtualpy_loader.debug("initializing laoder")
    loader = resource_loader.resource_loader()
    color_shader = loader.load_shader("shaders.hlsl", color_vertex_type)
    color_override_shader = loader.load_shader("override_colors.hlsl", color_vertex_type)
    color_override_by_pixel_shader = loader.load_shader("override_colors_by_pixel.hlsl", color_vertex_type)
    texture_shader = loader.load_shader("texture_shaders.hlsl", texture_vertex_type)
    color_norm_shader = loader.load_shader("color_normals.hlsl", color_vertex_type_with_normal)
    color_lighting_shader = loader.load_shader("color_lighting.hlsl", color_vertex_type_with_normal)

    virtualpy_loader.debug("loading textures")
    groundtex = loader.load_texture("ground.png")
    ceilingtex = loader.load_texture("ceiling.png")

    virtualpy_loader.debug("model")
    virtualpy.begin_model(texture_vertex_type)
    virtualpy_loader.debug("adding vertex")
    virtualpy.add_vertex(location=(-1, 0, 1), texture=(0, 0))
    virtualpy.add_vertex(location=(-1, 0, -1), texture=(0, 1))
    virtualpy.add_vertex(location=( 1, 0, 1), texture=(1, 0))
    virtualpy.add_vertex(location=( 1, 0, -1), texture=(1, 1))
    virtualpy_loader.debug("end model")
    texsqmod = virtualpy.end_model()

    virtualpy_loader.debug("create textured entity")
    #texsq = virtualpy.create_textured_entity(texsqmod, texture_shader, groundtex)
    #ceilsq = virtualpy.create_textured_entity(texsqmod, texture_shader, ceilingtex)
    
    # load model
    virtualpy_loader.debug("loading model")
    #entity = loader.load_model(resourcesloc + "Inosaedron_Wires_Linked.obj", virtualpy_loader.COLOR_VERTEX, color_shader)
    texture = loader.load_texture("babylonstation.jpg")

    override_color_bundle = virtualpy.create_render_bundle(1)
    teapot = loader.load_model(resourcesloc + "wt_teapot.obj.txt", virtualpy_loader.COLOR_VERTEX, color_override_shader)
    entity = loader.load_model(resourcesloc + "babylon station.obj", virtualpy_loader.TEXTURE_VERTEX, texture_shader, texture = texture)
    
    
    camera_location = [0, 0, 0]
    theta = 0

    virtualpy_loader.debug("camera")
    virtualpy.set_camera_location(camera_location)
    virtualpy_loader.debug("startup end")
    
    return locals().copy()
