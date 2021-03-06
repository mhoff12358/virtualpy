import time
import sys
import math
import itertools
virtualpyloc = sys.argv[1]
sys.path.append(virtualpyloc+'Debug')

import virtualpy

import pdb

#from model_loader import load_model
import model_loader

virtualpy.set_color(0, 0, 0)
virtualpy.push_state()
virtualpy.set_resources_location(virtualpyloc+'resources\\')
virtualpy.spawn_thread('directx')


vertex_type = virtualpy.VertexType(("location", "POSITION", 3), ("color", "COLOR", 4))

color_shader = virtualpy.load_shader("shaders.hlsl", vertex_type)

virtualpy.begin_model(vertex_type, virtualpy.PrimitiveType.triangle_strip)
a = {"location":(0.5, 0, -0.5/math.sqrt(3)), "color":(0.5, 0, 0, 1)}
b = {"location":(0, 0, 1/math.sqrt(3)), "color":(0, 0.5, 0, 1)}
c = {"location":(-0.5, 0, -0.5/math.sqrt(3)), "color":(0, 0, 0.5, 1)}
d = {"location":(0, math.sqrt(2)/math.sqrt(3), 0), "color":(0.5, 0.5, 0.5, 1)}
virtualpy.add_vertex(**a)
virtualpy.add_vertex(**c)
virtualpy.add_vertex(**b)
virtualpy.add_vertex(**d)
virtualpy.add_vertex(**a)
virtualpy.add_vertex(**c)
pyramid_model = virtualpy.end_model()

class Pyramid(object):
	detail_offsets = [
		virtualpy.Position((0, math.sqrt(2)/math.sqrt(3), 0)),
		virtualpy.Position((0, 0, 1/math.sqrt(3))),
		virtualpy.Position((0.5, 0, -0.5/math.sqrt(3))),
		virtualpy.Position((-0.5, 0, -0.5/math.sqrt(3))),
	]

	def __init__(self, detail_level=0, detail_path=()):
		if len(detail_path) != detail_level:
			raise ValueError("Attempting to create a pyramid with a detail_path with the wrong detail level")
	
		self.entity = virtualpy.create_modeled_entity(pyramid_model, color_shader)
		self.detail_level = detail_level
		self.detail_path = tuple(detail_path)
		self.position = virtualpy.Position((0, 0, -2))
		for i in range(detail_level):
			self.position += Pyramid.detail_offsets[detail_path[i]]*math.pow(0.5, i+1)
		self.scale = (math.pow(0.5, self.detail_level),)*3
		self.orientation = virtualpy.Quaternion((0, 0, 0, 1))
		
	def display(self):
		virtualpy.show_model(self.entity, self.position.vector, self.scale, self.orientation)
	
	def shatter(self):
		# Returns an iterator of pyramids
		return (Pyramid(self.detail_level+1, self.detail_path+(i,)) for i in range(4))

pyramids = [Pyramid()]

camera_location = [0, 0, 0]

prev_time = time.perf_counter()
curr_time = prev_time
move_scale = 0.75
theta = 0

frequency_out = open('py_frequency', 'w')

(prev_keys_at_state, prev_keys_since_state) = virtualpy.get_keyboard_state()

while True:
	time_diff = curr_time - prev_time
	frequency_out.write(str(time_diff)+"\n")
	frequency_out.flush()

	(keys_at_state, keys_since_state) = virtualpy.get_keyboard_state()
	
	if keys_since_state[ord('W')]:
		camera_location[2] -= move_scale*time_diff
	if keys_since_state[ord('S')]:
		camera_location[2] += move_scale*time_diff
	if keys_since_state[ord('A')]:
		camera_location[0] -= move_scale*time_diff
	if keys_since_state[ord('D')]:
		camera_location[0] += move_scale*time_diff
	if keys_since_state[ord('Q')]:
		camera_location[1] -= move_scale*time_diff
	if keys_since_state[ord('E')]:
		camera_location[1] += move_scale*time_diff
		
	if keys_since_state[ord('P')] and not prev_keys_since_state[ord('P')]:
		pyramids = tuple(itertools.chain(*tuple(p.shatter() for p in pyramids)))
		print ('a')
		
	virtualpy.set_camera_location(camera_location)
	prev_keys_at_state = keys_at_state
	prev_keys_since_state = keys_since_state
	
	theta = (theta + time_diff)% (2*3.14)
	
	#virtualpy.show_model(p2, (0, 0, -2), (1, 1, 1), virtualpy.Quaternion((0, 0, 0, 1)))
	for pyramid in pyramids:
		pyramid.display()
	
	#virtualpy.show_model(entity, position=(0, 0, -3), scale=(1, 1, 1), rotation=virtualpy.Quaternion((math.sin(theta/2), 0, 0, math.cos(theta/2))))
	virtualpy.push_state()
	
	time.sleep(0.001)

	prev_time = curr_time
	curr_time = time.perf_counter()