# loads data from obj file
import virtualpy
import time

def load_model(fname, vertex_type, shader, color = (1, 1, 1, 1)):
	# loads vertices, returns list of entities
	# color is white by default
	vertices = []
	model_vertices = []
	t = time.time()
	with open(fname, "r", encoding = "utf-8") as f:
		while True:
			line = f.readline()
			
			# stop when line is empty
			if line == "":
				break
			else:
				# remove trailing newline
				line = line.strip()
			
			# skip comments, blank lines
			if line.startswith("#") or len(line) == 0:
				continue
				
			# vertex
			elif line.startswith("v "):
				# line contains 3 coordinate values, separated by space
				v = line.strip("v").strip().split(" ")
				#assert(len(v) == 3)
				
				# convert from string to float
				v = tuple([float(x) for x in v])
				
				# add new vertex
				vertices.append(v)
				if time.time() - t > 1:
                                        print("v: " + str(len(vertices)))
                                        t = time.time()
				
			# face
			elif line.startswith("f "):
				# line contains multiple vertex id values
				ts = time.time()
				face = line[2:].split(" ")
				
				# remove everything after double slashes
				face = [i.partition("//")[0] for i in face]
				
				# convert from string to int
				face = [int(i) for i in face]
				
				# add new face
				ts = time.time()
				for i in face:
					# positive or negative offset
					i = i - 1 if i > 0 else len(vertices) + i
					loc = vertices[i]
					#virtualpy.add_vertex(location = loc, color = color)
					#virtualpy.add_vertex_batch((loc, color))
					model_vertices.append((loc, color))
				if time.time() - t > 1:
                                        print("f: " + str(len(model_vertices)))
                                        t = time.time()

			# group statement
			elif line.startswith("g"):
				continue
				
			# use material
			elif line.startswith("usemtl"):
				continue
				
			# vertex normals
			elif line.startswith("vn"):
				continue
					
			# unknown statement
			else:
				continue
	debug("parsing: " + str(time.time() - t))

	t = time.time()
	virtualpy.begin_model(vertex_type, virtualpy.PrimitiveType.triangle_list)
	virtualpy.add_vertex_batch(*model_vertices)
	model = virtualpy.end_model()
	entity = virtualpy.create_modeled_entity(model, shader)
	debug("adding: " + str(time.time() - t))
	return entity

def debug(msg):
	with open("out.txt", "a", encoding = "utf-8") as f:
		f.write(msg)
		f.write("\n")
