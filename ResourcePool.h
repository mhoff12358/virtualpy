#pragma once

#include "Python.h"
#include "PyHelper.h"
#include <string>
#include <vector>

class ResourcePool
{
public:
	ResourcePool();
	~ResourcePool();

	void Initialize();

	virtual void BeginNewModel(PyObject* vertex_type) = 0;
	virtual void BeginNewModel(PyObject* vertex_type, PyObject* primitive_type) = 0;
	virtual void AddModelVertex(PyObject* new_vertex) = 0;
	virtual void AddModelVertexFromArgs(PyObject* vertex_args, PyObject* vertex_kwargs);
	virtual int FinishModel() = 0;

	virtual int LoadTexture(std::string file_name) = 0;

	virtual int LoadShader(std::string file_name, PyObject* vertex_type) = 0;

	virtual int CreateModeledEntity(int model_id, int shader_id) = 0;
	virtual int CreateTexturedEntity(int model_id, int shader_id, int texture_id) = 0;

	virtual void CreateRenderBundle(int render_bundle_id, int num_constant_buffers, std::vector<char> pipeline_stages) = 0;

	virtual int GetNumberOfEntities() = 0;

protected:
	PyObject* current_model_vertex_type;
};

