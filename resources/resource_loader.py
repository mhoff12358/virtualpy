import sys

import virtualpy
import virtualpy_loader

class resource_loader(object):
    resource_cache = {}

    def __init__(self):
        virtualpyloc = sys.argv[1]
        resourcesloc = virtualpyloc+'resources\\'
        virtualpy.set_resources_location(resourcesloc)
        pass

    def load_shader(self, shader_name, vertex_type):
        if shader_name in self.resource_cache:
            return self.resource_cache[shader_name]
        else:
            shader = virtualpy.load_shader(shader_name, vertex_type)
            self.resource_cache[shader_name] = shader
            return shader

    def load_texture(self, texture_name):
        if texture_name in self.resource_cache:
            return self.resource_cache[texture_name]
        else:
            texture = virtualpy.load_texture(texture_name)
            self.resource_cache[texture_name] = texture
            return texture

    def load_model(self, model_name, texture_type, shader, color = (1, 1, 1, 1), texture = None):
        if model_name in self.resource_cache:
            return self.resource_cache[model_name]
        else:
            entity = virtualpy_loader.load_model(model_name, texture_type, shader, color, texture)
            self.resource_cache[model_name] = entity
            return entity
