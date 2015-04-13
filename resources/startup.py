import time
import sys
import math
virtualpyloc = sys.argv[1]
sys.path.append(virtualpyloc+'Debug')

import virtualpy

a = virtualpy.PrimitiveType.triangle_list

import pdb

#from model_loader import load_model
import model_loader

virtualpy.set_color(1, .5, 0)
virtualpy.push_state()
virtualpy.set_resources_location(virtualpyloc+'resources\\')
virtualpy.spawn_thread('directx')

color_vertex_type = virtualpy.VertexType(("location", "POSITION", 3), ("color", "COLOR", 4))
texture_vertex_type = virtualpy.VertexType(("location", "POSITION", 3), ("texture", "TEXCOORD", 2))
color_vertex_type_with_normal = virtualpy.VertexType(("location", "POSITION", 3), ("normal", "NORMAL", 3), ("color", "COLOR", 4))

color_shader = virtualpy.load_shader("shaders.hlsl", color_vertex_type)
color_override_shader = virtualpy.load_shader("override_colors.hlsl", color_vertex_type)
texture_shader = virtualpy.load_shader("texture_shaders.hlsl", texture_vertex_type)
color_norm_shader = virtualpy.load_shader("color_normals.hlsl", color_vertex_type_with_normal)

virtualpy.begin_model(color_vertex_type_with_normal, virtualpy.PrimitiveType.triangle_strip)

virtualpy.add_vertex(location=(1,  1, 0), normal=(0, 0, 1), color=(0.5, 0, 0, 1))
virtualpy.add_vertex(location=(-1,  1, 0), normal=(0, 0, 1), color=(0, 0.5, 0, 1))
virtualpy.add_vertex(location=( 1, -1, 0), normal=(0, 0, 1), color=(0, 0, 0.5, 1))
virtualpy.add_vertex(location=(-1, -1, 0), normal=(0, 0, 1), color=(0.5, 0.5, 0.5, 1))

redsqmod = virtualpy.end_model()
redsq = virtualpy.create_modeled_entity(redsqmod, color_norm_shader)

virtualpy.begin_model(color_vertex_type)

virtualpy.add_vertex(location=(-1, -1, 0), color=(1, 1, 1, 1))
virtualpy.add_vertex(location=(-1,  1, 0), color=(1, 1, 1, 1))
virtualpy.add_vertex(location=( 1, -1, 0), color=(1, 1, 1, 1))
virtualpy.add_vertex(location=( 1,  1, 0), color=(1, 1, 1, 1))

whitesqmod = virtualpy.end_model()
whitesq = virtualpy.create_modeled_entity(whitesqmod, color_override_shader)
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

# load model
entity = model_loader.load_model(virtualpyloc + "resources\\" + "wt_teapot.obj.txt", color_vertex_type, color_shader)

i = 0

camera_location = [0, 0, 0]
theta = 0
random_color = [0, 0, 0, 1]

prev_time = time.perf_counter()
curr_time = prev_time

specific_color_rb = virtualpy.create_render_bundle(1)
virtualpy.update_render_bundle(specific_color_rb, ((1, 1, 1, 1),))

while True:
	time_diff = curr_time - prev_time

	(keys_at_state, keys_since_state) = virtualpy.get_keyboard_state()
	
	if keys_since_state[ord('W')]:
		camera_location[2] -= 0.25
	if keys_since_state[ord('S')]:
		camera_location[2] += 0.25
	if keys_since_state[ord('A')]:
		camera_location[0] -= 0.25
	if keys_since_state[ord('D')]:
		camera_location[0] += 0.25
	if keys_since_state[ord('Q')]:
		camera_location[1] -= 0.25
	if keys_since_state[ord('E')]:
		camera_location[1] += 0.25
	
	virtualpy.set_camera_location(camera_location)

	theta += 0.75*time_diff
	theta %= 2*3.14
	random_color[0] = (random_color[0]+time_diff)%1

	redsqheight = 0
	if keys_since_state[ord('F')]:
		redsqheight = 3

		
	virtualpy.update_render_bundle(specific_color_rb, (tuple(random_color),))

	virtualpy.show_model(redsq, (-5*math.sin(theta), redsqheight, -5*math.cos(theta)), (1, 1, 1), virtualpy.Quaternion((0, math.sin(theta/2), 0, math.cos(theta/2)))*virtualpy.Quaternion((1, 0, 0, 0)))
	
	virtualpy.show_model(entity, position=(0, 0, -3), scale=(1, 1, 1), rotation=virtualpy.Quaternion((math.sin(theta/2), 0, 0, math.cos(theta/2))))

	virtualpy.show_model(texsq, (0, -1, 0), (10, 1, 10), virtualpy.Quaternion((0, 0, 0, 1)))
	virtualpy.show_model(ceilsq, (0, -.5, 0), (10, 1, 10), virtualpy.Quaternion((0, 0, 0, 1)))
	virtualpy.show_model(rightwhitesq, position=(11, 0, -8), scale=(1, 3, 1), rotation=virtualpy.Quaternion((0, 0, 0, 1)))
	virtualpy.show_model_with_render_bundle(specific_color_rb, whitesq, position=(-1, 0, -8), scale=(1, 3, 1), rotation=virtualpy.Quaternion((0, 0, 0, 1)))

	virtualpy.push_state()
	i = 1-i
	if keys_since_state[ord('G')]:
		time.sleep(1)
	else:
		time.sleep(0.5)

	prev_time = curr_time
	curr_time = time.perf_counter()