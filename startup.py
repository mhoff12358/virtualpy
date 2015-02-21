import virtualpy
import time

virtualpy.set_color(1, .5, 0)
virtualpy.push_state()
virtualpy.set_resources_location('C:\\Users\\mfhoffm2\\Source\\Repos\\virtualpy\\resources\\')
virtualpy.spawn_thread('directx_oculus')

virtualpy.begin_model()

virtualpy.color_vertex(-1, -1, 1, 1, 0, 0, 1)
virtualpy.color_vertex( 1, -1, 1, 1, 0, 0, 1)
virtualpy.color_vertex(-1,  1, 1, 1, 0, 0, 1)
virtualpy.color_vertex( 1,  1, 1, 1, 0, 0, 1)

redsq = virtualpy.end_model()

virtualpy.begin_model()

virtualpy.color_vertex(-1, -1, 1, 1, 1, 1, 1)
virtualpy.color_vertex( 1, -1, 1, 1, 1, 1, 1)
virtualpy.color_vertex(-1,  1, 1, 1, 1, 1, 1)
virtualpy.color_vertex( 1,  1, 1, 1, 1, 1, 1)

whitesq = virtualpy.end_model()

i = 0

while True:
	virtualpy.show_model(whitesq, position=(-2, 0, -5), scale=(1, 3, 1), rotation=(1, 0, 0, 0))
	virtualpy.show_model(redsq, (i*5, 0, -5), (1, 1, 1), (1, 0, 0, 0))
	virtualpy.push_state()
	i = 1-i
	time.sleep(5)
