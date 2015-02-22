import time
import sys
import math
sys.path.append('C:\\Users\\Matt\\Desktop\\cpython\\virtualpy_nt\\Debug')

import virtualpy


virtualpy.set_color(1, .5, 0)
virtualpy.push_state()
virtualpy.set_resources_location('C:\\Users\\Matt\\Desktop\\cpython\\virtualpy_nt\\resources\\')
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

while True:
	virtualpy.show_model(redsq, (i*5, 0, -4), (1, 1, 1), (1, 0, 0, 0))
	virtualpy.show_model(whitesq, position=(-1, 0, -8), scale=(1, 3, 1), rotation=(0, 0, 0, 1))
	virtualpy.show_model(texsq, (0, -1, 0), (10, 1, 10), (0, 0, 0, 1))
	virtualpy.show_model(ceilsq, (0, -.5, 0), (10, 1, 10), (0, 0, 0, 1))
	virtualpy.push_state()
	i = 1-i
	time.sleep(5)
