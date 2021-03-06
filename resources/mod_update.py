import math
import time
import sys

import virtualpy
import virtualpy_loader

def update(local_vars):
    #pass
    virtualpy_loader.debug("update")
    virtualpy.set_color(1.0, 0, 0)
    virtualpy.push_state()
    
    virtualpyloc = sys.argv[1]
    sys.path.append(virtualpyloc+'Debug')
    
    random_color = [0, 0, 0, 1]
    
    # get variables
    theta = local_vars.theta
    camera_location = local_vars.camera_location
    
    #theta += 0.75*time_diff
    theta %= 2*3.14
    #random_color[0] = (random_color[0]+0.1*time_diff)%1
    
    (keys_at_state, keys_since_state) = virtualpy.get_keyboard_state()

    movescale = 0.003
    virtualpy_loader.debug("keys")
    if keys_since_state[ord('W')]:
        camera_location[2] -= movescale
    if keys_since_state[ord('S')]:
        camera_location[2] += movescale
    if keys_since_state[ord('A')]:
        camera_location[0] -= movescale
    if keys_since_state[ord('D')]:
        camera_location[0] += movescale
    if keys_since_state[ord('Q')]:
        camera_location[1] -= movescale
    if keys_since_state[ord('E')]:
        camera_location[1] += movescale
    
    #print(camera_location)
    virtualpy_loader.debug("update camera")
    virtualpy.set_camera_location(camera_location)
    
    
    virtualpy_loader.debug("show start")
    #virtualpy.show_model(local_vars.entity, position=(0, 0, -3), scale=(0.01, 0.01, 0.01), rotation=virtualpy.Quaternion((math.sin(theta/2), 0, 0, math.cos(theta/2))))
    
    virtualpy.update_render_bundle(local_vars.override_color_bundle, ((1,1,1,1),))
    virtualpy.show_model_with_render_bundle(local_vars.override_color_bundle, local_vars.teapot, position=(0, 0, -3), scale=(0.01, 0.01, 0.01), rotation=virtualpy.Quaternion((math.sin(theta/2), 0, 0, math.cos(theta/2))))
    
    virtualpy_loader.debug("show end")
