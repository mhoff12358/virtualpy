import math
import time
import sys
import satelite

import virtualpy
import virtualpy_loader

def update(local_vars):
	#pass
	prev_time = local_vars.curr_time
	local_vars.curr_time = time.perf_counter()
	time_diff = local_vars.curr_time - prev_time
	
	virtualpy_loader.debug("update")
	virtualpy.set_color(1.0, 0, 0)
	virtualpy.push_state()
	
	local_vars.satty.timestep(time_diff)
	print(local_vars.satty.position)
	
	virtualpyloc = sys.argv[1]
	sys.path.append(virtualpyloc+'Debug')
	
	random_color = [0, 0, 0, 1]
	
	# get variables
	camera_location = local_vars.camera_location
	
	
	#local_vars.theta += 1*time_diff
	local_vars.theta %= 2*3.14
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
	
	#virtualpy.show_model(local_vars.texsq, (0, -1, 0), (10, 1, 10), virtualpy.Quaternion((0, 0, 0, 1)))
	#virtualpy.show_model(local_vars.ceilsq, (0, -.5, 0), (10, 1, 10), virtualpy.Quaternion((0, 0, 0, 1)))
	
	virtualpy_loader.debug("show start")
	virtualpy.show_model(local_vars.entity, position=(0, 0, -4), scale=(0.01, 0.01, 0.01), rotation=virtualpy.Quaternion((math.sin(local_vars.theta/2), 0, 0, math.cos(local_vars.theta/2))))
	virtualpy_loader.debug("show end")
	
	time.sleep(0.5)
	

	
