import sys
sys.path[0] = 'C:\\\\Users\\\\mhoff_000\\\\Documents\\\\cpython\\\\virtualpy_nt\\\\Debug'
import virtualpy
virtualpy.set_resources_location("C:\\Users\\mhoff_000\\Documents\\cpython\\virtualpy_nt\\resources\\")
virtualpy.spawn_thread('directx')
virtualpy.set_color(1,1,1)
virtualpy.push_state()
virtualpy.begin_model()
virtualpy.color_vertex(-1.0, -1.0, -1.0, 0.6, 0.4, 0.5, 1.0)
virtualpy.color_vertex(1.0, -1.0, -1.0, 0.6, 0.4, 0.5, 1.0)
virtualpy.color_vertex(-1.0, -1.0, 1.0, 0.6, 0.4, 0.5, 1.0)
virtualpy.color_vertex(1.0, -1.0, 1.0, 0.6, 0.4, 0.5, 1.0)
first_model = virtualpy.end_model()


print(first_model)
virtualpy.show_model(first_model, (0, 0, 0), (1, 1, 1), (1, 0, 0, 0))
virtualpy.push_state()