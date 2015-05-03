import sys
import virtualpy

def startup():
    virtualpyloc = sys.argv[1]
    
    virtualpy.set_color(0.392, 0.584, 0.929)
    virtualpy.push_state()
    virtualpy.set_resources_location(virtualpyloc+'resources\\')
    virtualpy.spawn_thread('directx')
    return locals().copy()