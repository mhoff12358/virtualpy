#pragma once

#include <Windows.h>

#include "Python.h"
#include "PyHelper.h"
#include <string>
#include <vector>

namespace virtualpy {

	class ResourcePool
	{
	public:
		ResourcePool();
		~ResourcePool();

		virtual void Initialize();

		virtual void BeginNewModel(PyObject* vertex_type) = 0;
		virtual void BeginNewModel(PyObject* vertex_type, PyObject* primitive_type) = 0;
		virtual void AddModelVertex(PyObject* new_vertex) = 0;
		virtual void AddModelVertexBatch(PyObject* new_vertexes) = 0;
		virtual PyObject* AddModelVertexFromArgs(PyObject* vertex_args, PyObject* vertex_kwargs);
		virtual PyObject* AddModelVertexFromArgsFast(PyObject* vertex_args);
		virtual PyObject* AddModelVertexFromArgsBatch(PyObject* vertex_args_kwargs_list);
		virtual int FinishModel() = 0;

		virtual int LoadModelFromFile(std::string file_name) = 0;

		virtual int LoadVertexShader(std::string file_name, PyObject* vertex_type) = 0;
		virtual int LoadPixelShader(std::string file_name) = 0;

		virtual int CreateObjectSettings() = 0;

		virtual int CreateModeledEntity(int model_id, int vertex_shader_id, int pixel_shader_id, int object_setting_id) = 0;
		//virtual int CreateTexturedEntity(int model_id, int shader_id, int texture_id) = 0;

		//virtual void CreateRenderBundle(int render_bundle_id, int num_constant_buffers, std::vector<char> pipeline_stages) = 0;

	protected:
		PyObject* current_model_vertex_type;
	};

} // virtualpy