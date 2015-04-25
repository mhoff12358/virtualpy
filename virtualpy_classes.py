import math, functools, itertools

from enum import Enum

class RawType(object):
    def __init__(self, num_floats):
        if not num_floats in [1, 2, 3, 4]:
            raise ValueError("Creating RawType with an incorrect number of 32 bit floats")
        self.num_floats = num_floats

    def validateData(self, test_data):
        if len(test_data) != self.num_floats:
            raise ValueError("Attempting to store the wrong amount of data in a RawType")

class MetaType(object):
    valid_meta_type_names = [\
        "POSITION", "TEXCOORD", "COLOR", "NORMAL"\
        ]

    def __init__(self, meta_type_name):
        if not isinstance(meta_type_name, str):
            raise TypeError("Creating MetaType with non-string name")
        if not meta_type_name in MetaType.valid_meta_type_names:
            raise ValueError("Creating MetaType with invalid semantic name")
        self.type_name = meta_type_name


class VertexType(object):
    def __init__(self, *args):
        """
        Each element in args is a type definition for the vertex
        The type definition consists of 2 or 3 elements, the meta type
        (position, tex coord...), the raw type (3 32 bit floats RGB...), and
        optionally a name
        """
        self.type_def = []
        self.name_reverse_lookup = {}
        for i in range(len(args)):
            if len(args[i]) == 2:
                (meta, raw) = args[i]
            elif len(args[i]) == 3:
                (name, meta, raw) = args[i]
                if not isinstance(name, str):
                    raise TypeError("Vertex type initialization tuple had a non-string name")
                self.name_reverse_lookup[len(self.type_def)] = name
            else:
                raise ValueError("Invalid number of elements in a vertex type object's initialization tuple")
            if not isinstance(meta, MetaType):
                meta = MetaType(meta)
            if not isinstance(raw, RawType):
                raw = RawType(raw)
            self.type_def.append((meta, raw))


    def getTypeDef(self):
        return self.type_def

    def createVertex(self, *args, **kwargs):
        # Merges the args and kwargs into one data list based on the existing
        # name_reverse_lookup
        if len(kwargs)+len(args) != len(self.type_def):
            raise ValueError("Wrong number of elements passed into a createVertex, expected " + str(len(self.type_def)) + " and got " + str(len(kwargs)+len(args)))
        data_list = []
        for i in range(len(args)):
            self.type_def[i][1].validateData(args[i])
            data_list.append(args[i])
        for i in range(len(data_list), len(self.type_def)):
            # This will fail if the kwargs are the wrong values to fill out the list
            self.type_def[i][1].validateData(kwargs[self.name_reverse_lookup[i]])
            data_list.append(kwargs[self.name_reverse_lookup[i]])
        return Vertex(self, data_list)

    def createVertexFast(self, *args):
        return Vertex(self, list(args))

    def createVertexBatch(self, *args):
        return tuple(itertools.starmap(self.createVertex, args))

class PrimitiveType(Enum):
	point_list = 1
	line_list = 2
	line_strip = 3
	triangle_list = 4
	triangle_strip = 5

class Vertex(object):
    def __init__(self, vertex_type, data_list):
        self.vertex_type = vertex_type
        self.data_list = data_list

class Quaternion(object):
	def __init__(self, vector):
		#self.vector should be in the form (x, y, z, w) aka (i, j, k, real)
		if len(vector) != 4:
			raise ValueError("Wrong size of the vector being passed into Quaternion constructor")
		self.vector = tuple(vector)
		
	def x(self):
		return self.vector[0]

	def y(self):
		return self.vector[1]

	def z(self):
		return self.vector[2]

	def w(self):
		return self.vector[3]

	def __mul__(self, other):
		if isinstance(other, Quaternion):
			return Quaternion((
				self.w()*other.x()+self.x()*other.w()+self.y()*other.z()-self.z()*other.y(),
				self.w()*other.y()-self.x()*other.z()+self.y()*other.w()+self.z()*other.x(),
				self.w()*other.z()+self.x()*other.y()-self.y()*other.x()+self.z()*other.w(),
				self.w()*other.w()-self.x()*other.x()-self.y()*other.y()-self.z()*other.z()))
		else:
			return Quaternion(tuple(other*val for val in self.vector))

	def __add__(self, other):
		return Quaternion(tuple(self.vector[i]+other.vector[i] for i in range(4)))

	def __rmul__(self, other):
		return Quaternion(tuple(other*val for val in self.vector))
