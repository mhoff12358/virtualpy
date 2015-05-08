# loads data from obj file
import virtualpy
import time

from itertools import *

# constants
COLOR_VERTEX = "color_vertex"
TEXTURE_VERTEX = "texture_vertex"

def load_model(fname, vertex_type, shader, color = (1, 1, 1, 1), texture = None):
    # loads vertices, returns list of entities
    # color is white by default
    vertices = []
    texture_vertices = []
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
                assert len(v) == 3, str(line)
                
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
                face = line[2:].split(" ")
                
                # format for each vertex is v/vt/vn
                # format is v//vn if only vertices and vertex normals
                if any(["//" in f for f in face]):
                    face = [i.split("//") for i in face]
                else:
                    face = [i.split("/") for i in face]
                
                # convert from string to int
                face = [list(map(int, i)) for i in face]
                
                # add new face
                #for i in face:
                for fs in [(face[0], face[i], face[i + 1]) if j else
                           (face[i + 1], face[i], face[0])
                           for i in range(1, len(face) - 1)
                           for j in range(0, 2)]:
                    for v in fs:
                        vi = v[0]
                        vt = v[1]
                        vn = v[-1]
                        
                        # positive or negative offset
                        vi = vi - 1 if vi > 0 else len(vertices) + i
                        v_coords = vertices[vi]
                        
                        #virtualpy.add_vertex(location = v_coords, color = color)
                        #virtualpy.add_vertex_batch((v_coords, color))
                        if vertex_type is COLOR_VERTEX:
                            model_vertices.append((v_coords, color))
                        elif vertex_type is TEXTURE_VERTEX:
                            vt = vt - 1 if vt > 0 else len(texture_vertices) + id
                            tex_coords = texture_vertices[vt]
                            
                            model_vertices.append((v_coords, tex_coords))
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
                
            # texture vertex
            elif line.startswith("vt"):
                # line contains 2 coordinate values, separated by space
                vt = line.strip("vt").strip().split(" ")
                vt = vt[0:2]
                
                # convert from string to float
                vt = tuple([float(x) for x in vt])
                
                # add new vertex
                texture_vertices.append(vt)
                    
            # unknown statement
            else:
                continue
    debug("parsing: " + str(time.time() - t))
    
    t = time.time()
    if vertex_type is COLOR_VERTEX:
        color_vertex_type = virtualpy.VertexType(("location", "POSITION", 3), ("color", "COLOR", 4))
        virtualpy.begin_model(color_vertex_type, virtualpy.PrimitiveType.triangle_list)
        virtualpy.add_vertex_batch(*model_vertices)
        model = virtualpy.end_model()
        entity = virtualpy.create_modeled_entity(model, shader)
    elif vertex_type is TEXTURE_VERTEX:
        assert(texture is not None)
        texture_vertex_type = virtualpy.VertexType(("location", "POSITION", 3), ("texture", "TEXCOORD", 2))
        virtualpy.begin_model(texture_vertex_type, virtualpy.PrimitiveType.triangle_list)
        virtualpy.add_vertex_batch(*model_vertices)
        model = virtualpy.end_model()
        entity = virtualpy.create_textured_entity(model, shader, texture)
        
    debug("adding: " + str(time.time() - t))
    return entity

def debug(msg):
    with open("out.txt", "a", encoding = "utf-8") as f:
        f.write(msg)
        f.write("\n")
