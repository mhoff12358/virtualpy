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

virtualpy.set_color(0, 0, 0)
virtualpy.push_state()
virtualpy.set_resources_location(virtualpyloc+'resources\\')
virtualpy.spawn_thread('directx_oculus')


vertex_type = virtualpy.VertexType(("location", "POSITION", 3), ("color", "COLOR", 4))

color_shader = virtualpy.load_shader("shaders.hlsl", vertex_type)

virtualpy.begin_model(vertex_type, virtualpy.PrimitiveType.triangle_strip)
a = {"location":(1, -1, -1), "color":(0.5, 0, 0, 1)}
b = {"location":(0, -1, 1), "color":(0, 0.5, 0, 1)}
c = {"location":(-1, -1, -1), "color":(0, 0, 0.5, 1)}
d = {"location":(0, 1, 0), "color":(0.5, 0.5, 0.5, 1)}
virtualpy.add_vertex(**a)
virtualpy.add_vertex(**c)
virtualpy.add_vertex(**b)
virtualpy.add_vertex(**d)
virtualpy.add_vertex(**a)

pyramid_model = virtualpy.end_model()

def create_pyramid():
	return virtualpy.create_modeled_entity(pyramid_model, color_shader)

p1 = create_pyramid()
p2 = create_pyramid()
	
i = 0

camera_location = [0, 0, 0]

prev_time = time.perf_counter()
curr_time = prev_time


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
	
	virtualpy.show_model(p2, (0, 0, -5), (1, 1, 1), virtualpy.Quaternion((0, 0, 0, 1)))
	
	#virtualpy.show_model(entity, position=(0, 0, -3), scale=(1, 1, 1), rotation=virtualpy.Quaternion((math.sin(theta/2), 0, 0, math.cos(theta/2))))
	virtualpy.push_state()
	
	i = 1-i
	time.sleep(0.5)

	prev_time = curr_time
	curr_time = time.perf_counter()