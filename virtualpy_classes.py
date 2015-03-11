
class RawType(object):
    def __init__(self, type_list):
        self.type_list = type_list

    def verify(self, vertex_element):
        return True

class VertexType(object):
    def __init__(self, *args):
        """
        Each element in args is a type definition for the vertex
        The type definition consists of 2 or 3 elements, the meta type
        (position, tex coord...), the raw type (3 32 bit floats RGB...), and
        optionally a name
        """
        self.type_def = args
        self.total_num_elements = type_list

    def getTypeDef(self):
        return self.type_def

    def createVertex(self, *args, **kwargs):
        

class Vertex(object):
    def __init__(self, vertex_type, data_list):
        self.vertex_type = vertex_type
        self.data_list = data_list
