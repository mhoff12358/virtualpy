import time
import sys
import math
virtualpyloc = sys.argv[1]
sys.path.append(virtualpyloc+'Debug')

import virtualpy

import pdb

virtualpy.set_color(1, .5, 0)
virtualpy.push_state()
virtualpy.set_resources_location(virtualpyloc+'resources\\')
virtualpy.spawn_thread('directx_oculus')

color_vertex_type = virtualpy.VertexType(("location", "POSITION", 3), ("color", "COLOR", 4))
texture_vertex_type = virtualpy.VertexType(("location", "POSITION", 3), ("texture", "TEXCOORD", 2))

color_shader = virtualpy.load_shader("shaders.hlsl", color_vertex_type)
texture_shader = virtualpy.load_shader("texture_shaders.hlsl", texture_vertex_type)

virtualpy.begin_model(color_vertex_type)

virtualpy.add_vertex(location=(1,  1, 0), color=(1, 0, 0, 1))
virtualpy.add_vertex(location=(-1,  1, 0), color=(0, 1, 0, 1))
virtualpy.add_vertex(location=( 1, -1, 0), color=(0, 0, 1, 1))
virtualpy.add_vertex(location=(-1, -1, 0), color=(1, 1, 1, 1))

redsqmod = virtualpy.end_model()
redsq = virtualpy.create_modeled_entity(redsqmod, color_shader)

virtualpy.begin_model(color_vertex_type)

virtualpy.add_vertex(location=(-1, -1, 0), color=(1, 1, 1, 1))
virtualpy.add_vertex(location=(-1,  1, 0), color=(1, 1, 1, 1))
virtualpy.add_vertex(location=( 1, -1, 0), color=(1, 1, 1, 1))
virtualpy.add_vertex(location=( 1,  1, 0), color=(1, 1, 1, 1))

whitesqmod = virtualpy.end_model()
whitesq = virtualpy.create_modeled_entity(whitesqmod, color_shader)
rightwhitesq = virtualpy.create_modeled_entity(whitesqmod, color_shader)

groundtex = virtualpy.load_texture("ground.png")
ceilingtex = virtualpy.load_texture("ceiling.png")

virtualpy.begin_model(texture_vertex_type)
virtualpy.add_vertex(location=(-1, 0, 1), texture=(0, 0))
virtualpy.add_vertex(location=(-1, 0, -1), texture=(0, 1))
virtualpy.add_vertex(location=( 1, 0, 1), texture=(1, 0))
virtualpy.add_vertex(location=( 1, 0, -1), texture=(1, 1))
texsqmod = virtualpy.end_model()

texsq = virtualpy.create_textured_entity(texsqmod, texture_shader, groundtex)
ceilsq = virtualpy.create_textured_entity(texsqmod, texture_shader, ceilingtex)

i = 0

camera_location = [0, 0, 0]

while True:
	(keys_at_state, keys_since_state) = virtualpy.get_keyboard_state()
	
	if keys_since_state[ord('W')]:
		camera_location[2] -= 0.25
	if keys_since_state[ord('S')]:
		camera_location[2] += 0.25
	if keys_since_state[ord('A')]:
		camera_location[0] -= 0.25
	if keys_since_state[ord('D')]:
		camera_location[0] += 0.25
	
	virtualpy.set_camera_location(camera_location)

	redsqheight = 0
	if keys_since_state[ord('F')]:
		redsqheight = 3

	if i%2 == 0:
		virtualpy.show_model(redsq, (0, redsqheight, -5), (1, 1, 1), virtualpy.Quaternion((0, 1/math.sqrt(2), 0, -1/math.sqrt(2)))*virtualpy.Quaternion((1, 0, 0, 0)))
	elif i%2 == 1:
		virtualpy.show_model(redsq, (0, redsqheight, -5), (1, 1, 1), virtualpy.Quaternion((1, 0, 0, 0)))

	virtualpy.show_model(whitesq, position=(-1, 0, -8), scale=(1, 3, 1), rotation=virtualpy.Quaternion((0, 0, 0, 1)))
	virtualpy.show_model(rightwhitesq, position=(11, 0, -8), scale=(1, 3, 1), rotation=virtualpy.Quaternion((0, 0, 0, 1)))
	virtualpy.show_model(texsq, (0, -1, 0), (10, 1, 10), virtualpy.Quaternion((0, 0, 0, 1)))
	virtualpy.show_model(ceilsq, (0, -.5, 0), (10, 1, 10), virtualpy.Quaternion((0, 0, 0, 1)))
	virtualpy.push_state()
	i = 1-i
	if keys_since_state[ord('G')]:
		time.sleep(1)
	else:
		time.sleep(0.5)
