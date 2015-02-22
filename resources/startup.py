import virtualpy
import time

virtualpy.set_color(1, .5, 0)
virtualpy.push_state()
virtualpy.set_resources_location('C:\\Users\\mfhoffm2\\Source\\Repos\\virtualpy\\resources\\')
virtualpy.spawn_thread('directx_oculus')

virtualpy.begin_model(1)

virtualpy.color_vertex( 1,  1, 0, 1, 0, 0, 1)
virtualpy.color_vertex(-1,  1, 0, 1, 0, 0, 1)
virtualpy.color_vertex( 1, -1, 0, 1, 0, 0, 1)
virtualpy.color_vertex(-1, -1, 0, 1, 0, 0, 1)

redsq = virtualpy.end_model()

virtualpy.begin_model(1)

virtualpy.color_vertex(-1, -1, 0, 1, 1, 1, 1)
virtualpy.color_vertex(-1,  1, 0, 1, 1, 1, 1)
virtualpy.color_vertex( 1, -1, 0, 1, 1, 1, 1)
virtualpy.color_vertex( 1,  1, 0, 1, 1, 1, 1)

whitesq = virtualpy.end_model()

groundtex = virtualpy.load_texture("ground.png")

virtualpy.begin_model(0)
virtualpy.texture_vertex(-1, 0, 1, 0, 0)
virtualpy.texture_vertex(-1, 0, -1, 0, 1)
virtualpy.texture_vertex( 1, 0, 1, 1, 0)
virtualpy.texture_vertex( 1, 0, -1, 1, 1)
texsq = virtualpy.end_model()

i = 0

while True:
	virtualpy.show_model(whitesq, position=(-1, 0, -8), scale=(1, 3, 1), rotation=(0, 0, 0, 1))
	virtualpy.show_model(redsq, (i*5, 0, -4), (1, 1, 1), (1, 0, 0, 0))
	virtualpy.show_model(texsq, (0, -1, 0), (10, 1, 10), (0, 0, 0, 1))
	virtualpy.push_state()
	i = 1-i
	time.sleep(5)
