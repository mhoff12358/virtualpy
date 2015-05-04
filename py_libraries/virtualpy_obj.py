import virtualpy
import virtualpy_classes

from math import *

class Model:
    def __init__(self, vertex_type, vertex_data = [], auto_finalize = True):
        self.vertex_type = vertex_type
        self.vertex_data = vertex_data

        if auto_finalize:
            self.end_model()

    def add_vertex(self, vertex):
        self.vertex_data.append(vertex)

    def end_model(self, prim_type = virtualpy.PrimitiveType.triangle_list):
        # use batch method to add all vertices at once
        virtualpy.begin_model(self.vertex_type, prim_type)
        virtualpy.add_vertex_batch(*self.vertex_data)
        self.model_id = virtualpy.end_model()

class Texture:
    def __init__(self, filename):
        self.filename = filename
        self.texture_id = virtualpy.load_texture(filename)

class Shader:
    def __init__(self, filename, vertex_type):
        self.shader_id = virtualpy.load_shader(filename, vertex_type)

class Entity:
    def __init__(self):
        self.entity_id = None
        pass

    def show(self, position, scale, rotation):
        if self.entity_id is not None:
            rotation = rotation.as_quaternion() if type(rotation) is Rotation \
                       else rotation
            virtualpy.show_model(self.entity_id, position, scale, rotation)

class ModeledEntity(Entity):
    def __init__(self, model, shader):
        super().__init__()
        self.entity_id = virtualpy.create_modeled_entity(model, shader)

class TexturedEntity(Entity):
    def __init__(self, model, shader, texture):
        super().__init__()
        self.entity_id = virtualpy.create_textured_entity(model, shader,
                                                          texture)

class Rotation:
    def __init__(self):
        # TODO: make static variable for identity quaternion?
        self.quaternion = virtualpy.Quaternion((0, 0, 0, 1))

    def yawpitchroll(self, yaw, pitch, roll):
        self.yaw(yaw)
        self.pitch(pitch)
        self.roll(roll)

    def yaw(self, yaw):
        # assumes yaw is oriented around y axis
        self.axis_angle((0, 1, 0), yaw)

    def pitch(self, pitch):
        # assumes pitch is oriented around x axis
        self.axis_angle((1, 0, 0), pitch)

    def roll(self, roll):
        # assumes roll is oriented around z axis
        self.axis_angle((0, 0, 1), roll)

    def axis_angle(self, axis, angle):
        # assumes angle is given in radians
        (vx, vy, vz) = axis
        self.apply_quaternion(cos(angle / 2), vx * sin(angle / 2),
                              vy * sin(angle / 2), vz * sin(angle / 2))

    def apply_quaternion(self, quaternion):
        self.quaternion *= quaternion

    def as_quaternion(self):
        return self.quaternion
