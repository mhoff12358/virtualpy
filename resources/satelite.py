import sys
virtualpyloc = sys.argv[1]
sys.path.append(virtualpyloc+'Debug')
import virtualpy

import math
import time
import virtualpy_loader
import resource_loader
import itertools

import satelite


class Satelite(object):
	def __init__(self, entity, position, velocity):
		self.entity = entity
		
		self.position = position
		self.velocity = velocity
		
	def timestep(self, deltat, rot_position, mu):
		#import pdb; pdb.set_trace()
		rel_position = self.position + (-1*rot_position)
		position_offset = deltat * self.velocity
		velocity_offset = -deltat*mu/rel_position.normsq()*rel_position/rel_position.norm()
		
		self.position += position_offset
		self.velocity += velocity_offset
		
	def draw(self):
		virtualpy.show_model(self.entity, position=(self.position+virtualpy.Position((0, 0, -4))).vector, scale = (1,1,1), rotation = virtualpy.Quaternion((0,0,0,1)))
		
		
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

virtualpy.begin_model(color_vertex_type, virtualpy.PrimitiveType.point_list)
virtualpy.add_vertex(location=(0,0,0), color=(1, 1, 1, 1))
point = virtualpy.end_model()

num_satelites = 2
positions = ((1,0,0), (0,0, 1))
velocities = ((0,0,math.sqrt(9.8)), (0,math.sqrt(9.8), 0))
sats = []
for i in range(num_satelites):
	sattent = virtualpy.create_modeled_entity(point, color_shader)
	sats.append(Satelite(sattent, virtualpy.Position(positions[i]), virtualpy.Position(velocities[i])))

#virtualpy_loader.debug("create textured entity")
texsq = virtualpy.create_textured_entity(texsqmod, texture_shader, groundtex)
#ceilsq = virtualpy.create_textured_entity(texsqmod, texture_shader, ceilingtex)

camera_location = [0, 0, 0]
theta = 0

virtualpy.set_camera_location(camera_location)


curr_time = time.perf_counter()

f = open(virtualpyloc+"\\resources\\space_data.txt")
lines = f.readlines()
lines2 = lines[1:len(lines):2]
#data = list(map(lambda x:list(map(float, x.strip().split(' ')[2:])), lines2))
data = list(map(lambda x:map(float, x.strip().split(' ')[2:8]), lines2))
mu = 398600
import pdb; pdb.set_trace()

def find_sat_pos_vel(inc,RAAN,e,omega,M,n):
	t = M/n
	a = math.pow(mu/math.pow(M/t,2),1/3)
	f = math.degrees(math.atan(math.tan(math.radians(M/2))*math.sqrt((1+e)/(1-e))))
	radius_norm = a*(1-math.pow(e,2))/(1+e*math.cos(math.radians(f)))
	velocity_norm = math.sqrt(mu*(2/radius_norm-1/a))
	h = math.sqrt(mu*a*(1-math.pow(e,2)))
	th = omega + f
	position = radius_norm*virtualpy.Position((\
		math.cos(math.radians(RAAN))*math.cos(math.radians(th))-math.cos(math.radians(inc))*math.sin(math.radians(RAAN))*math.sin(math.radians(th)),\
		math.sin(math.radians(RAAN))*math.cos(math.radians(th))+math.cos(math.radians(inc))*math.cos(math.radians(RAAN))*math.sin(math.radians(th)),\
		math.sin(math.radians(th))*math.sin(math.radians(inc))))
	velocity = -mu/h*virtualpy.Position((\
		math.cos(math.radians(RAAN))*(math.sin(math.radians(th))+e*math.sin(math.radians(omega)))+math.sin(math.radians(RAAN))*(math.cos(math.radians(th))+e*math.cos(math.radians(omega)))*math.cos(math.radians(inc)),\
		math.sin(math.radians(RAAN))*(math.sin(math.radians(th))+e*math.sin(math.radians(omega)))-math.cos(math.radians(RAAN))*(math.cos(math.radians(th))+e*math.cos(math.radians(omega)))*math.cos(math.radians(inc)),\
		-1*(math.cos(math.radians(th))+e*math.cos(math.radians(omega)))*math.sin(math.radians(inc))))
		
	return position, velocity
		
space_data = list(itertools.starmap(find_sat_pos_vel, data))
import pdb; pdb.set_trace()
		
while True:
	prev_time = curr_time
	curr_time = time.perf_counter()
	time_diff = curr_time - prev_time
	
	virtualpy_loader.debug("update")
	virtualpy.set_color(0, 0, 0)
	virtualpy.push_state()
	
	for sat in sats:
		sat.timestep(time_diff/15, virtualpy.Position((0,0,0)), 9.8)
	#print("Position:", satty.position)
	#print("Velocity:", satty.velocity)
	
	virtualpyloc = sys.argv[1]
	sys.path.append(virtualpyloc+'Debug')
	
	random_color = [0, 0, 0, 1]
	
	# get variables
	camera_location = camera_location
	
	
	#theta += 1*time_diff
	theta %= 2*3.14
	#random_color[0] = (random_color[0]+0.1*time_diff)%1
	
	(keys_at_state, keys_since_state) = virtualpy.get_keyboard_state()

	virtualpy_loader.debug("keys")
	if keys_since_state[ord('W')]:
		camera_location[2] -= 0.003
	if keys_since_state[ord('S')]:
		camera_location[2] += 0.003
	if keys_since_state[ord('A')]:
		camera_location[0] -= 0.003
	if keys_since_state[ord('D')]:
		camera_location[0] += 0.003
	if keys_since_state[ord('Q')]:
		camera_location[1] -= 0.003
	if keys_since_state[ord('E')]:
		camera_location[1] += 0.003
	
	#print(camera_location)
	virtualpy_loader.debug("update camera")
	virtualpy.set_camera_location(camera_location)
	
	
	#virtualpy.show_model(entity, position=(0, 0, -4), scale=(0.01, 0.01, 0.01), rotation=virtualpy.Quaternion((math.sin(theta/2), 0, 0, math.cos(theta/2))))
	for sat in sats:
		sat.draw()
	
	time.sleep(0.1)
	