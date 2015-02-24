import time
import sys
import math
virtualpyloc = sys.argv[1]
sys.path.append(virtualpyloc+'Debug')

import virtualpy


virtualpy.set_color(1, .5, 0)
virtualpy.push_state()
virtualpy.set_resources_location(virtualpyloc+'resources\\')
virtualpy.spawn_thread('directx')

virtualpy.begin_model(1)

virtualpy.color_vertex( 1,  1, 0, 1, 0, 0, 1)
virtualpy.color_vertex(-1,  1, 0, 1, 0, 0, 1)
virtualpy.color_vertex( 1, -1, 0, 1, 0, 0, 1)
virtualpy.color_vertex(-1, -1, 0, 1, 0, 0, 1)

redsqmod = virtualpy.end_model()
redsq = virtualpy.create_modeled_entity(redsqmod)



virtualpy.begin_model(1)

virtualpy.color_vertex(-1, -1, 0, 1, 1, 1, 1)
virtualpy.color_vertex(-1,  1, 0, 1, 1, 1, 1)
virtualpy.color_vertex( 1, -1, 0, 1, 1, 1, 1)
virtualpy.color_vertex( 1,  1, 0, 1, 1, 1, 1)

whitesqmod = virtualpy.end_model()
whitesq = virtualpy.create_modeled_entity(whitesqmod)



groundtex = virtualpy.load_texture("ground.png")
ceilingtex = virtualpy.load_texture("ceiling.png")

virtualpy.begin_model(0)
virtualpy.texture_vertex(-1, 0, 1, 0, 0)
virtualpy.texture_vertex(-1, 0, -1, 0, 1)
virtualpy.texture_vertex( 1, 0, 1, 1, 0)
virtualpy.texture_vertex( 1, 0, -1, 1, 1)
texsqmod = virtualpy.end_model()

texsq = virtualpy.create_textured_entity(texsqmod, groundtex)
ceilsq = virtualpy.create_textured_entity(texsqmod, ceilingtex)

i = 0

camera_location = [0, 0, 0]

while True:
	key_state = virtualpy.get_keyboard_state()
	
	if key_state[ord('W')]:
		camera_location[2] -= 0.25
	if key_state[ord('S')]:
		camera_location[2] += 0.25
	if key_state[ord('A')]:
		camera_location[0] -= 0.25
	if key_state[ord('D')]:
		camera_location[0] += 0.25
	
	virtualpy.set_camera_location(camera_location)

	if key_state[ord('F')]:
		virtualpy.show_model(redsq, (i*5, 3, -4), (1, 1, 1), (1, 0, 0, 0))
	else:
		virtualpy.show_model(redsq, (i*5, 0, -4), (1, 1, 1), (1, 0, 0, 0))
	
	virtualpy.show_model(whitesq, position=(-1, 0, -8), scale=(1, 3, 1), rotation=(0, 0, 0, 1))
	virtualpy.show_model(texsq, (0, -1, 0), (10, 1, 10), (0, 0, 0, 1))
	virtualpy.show_model(ceilsq, (0, -.5, 0), (10, 1, 10), (0, 0, 0, 1))
	virtualpy.push_state()
	i = 1-i
	time.sleep(0.5)
