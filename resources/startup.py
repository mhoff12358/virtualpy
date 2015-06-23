import time
import sys
import math

import virtualpy

a = virtualpy.PrimitiveType.triangle_list

import pdb

if len(sys.argv) >= 2:
    virtualpyloc = sys.argv[1]
else:
    virtualpyloc = '.\\'

virtualpy.set_color(1, .5, 0)
virtualpy.push_state()
virtualpy.set_resources_location(virtualpyloc+'resources\\')
virtualpy.spawn_thread('directx')

color_vertex_type = virtualpy.VertexType(("location", "POSITION", 3), ("color", "COLOR", 4))
texture_vertex_type = virtualpy.VertexType(("location", "POSITION", 3), ("texture", "TEXCOORD", 2))
color_vertex_type_with_normal = virtualpy.VertexType(("location", "POSITION", 3), ("normal", "NORMAL", 3), ("color", "COLOR", 4))

object_pixel_shader = virtualpy.load_pixel_shader("objshader.hlsl")
object_vertex_shader = virtualpy.load_vertex_shader("objshader.hlsl", texture_vertex_type)

object_model = virtualpy.load_model_from_file("rhod.obj")

object_settings = virtualpy.create_object_settings()

object_entity = virtualpy.create_modeled_entity(object_model, object_vertex_shader, object_pixel_shader, object_settings);

virtualpy.show_model(entity_id = object_entity, position = (0, 0, -4), scale = (1, 1, 1), rotation = virtualpy.Quaternion((0, 0, 0, 1)))

while True:
    input()
    virtualpy.push_state()
