# loads data from obj file
import virtualpy

def load_model(fname, vertex_type, shader, color = (1, 1, 1, 1)):
	# loads vertices, returns list of entities
	# color is white by default
	vertices = []
	virtualpy.begin_model(vertex_type, virtualpy.PrimitiveType.triangle_list)
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
				
			# group statement
			elif line.startswith("g"):
				continue
				
			# use material
			elif line.startswith("usemtl"):
				continue
				
			# vertex normals
			elif line.startswith("vn"):
				continue
				
			# vertex
			elif line.startswith("v"):
				# line contains 3 coordinate values, separated by space
				v = line.strip("v").strip().split(" ")
				assert(len(v) == 3)
				
				# convert from string to float
				v = tuple([float(x) for x in v])
				
				# add new vertex
				vertices.append(v)
				
			# face
			elif line.startswith("f"):
				# line contains multiple vertex id values
				face = line.strip("f").strip().split(" ")
				
				# remove everything after double slashes
				face = [i.partition("//")[0] for i in face]
				
				# convert from string to int
				face = [int(i) for i in face]
				
				# add new face
				for i in face:
					loc = vertices[i - 1]
					virtualpy.add_vertex(location = loc, color = color)

			# unknown statement
			else:
				continue
	
	model = virtualpy.end_model()
	entity = virtualpy.create_modeled_entity(model, shader)
	return entity
