#pragma once

#include "Python.h"
#include "PyHelper.h"
#include <string>

class ResourcePool
{
public:
	ResourcePool();
	~ResourcePool();

	void Initialize();

	virtual void BeginNewModel(PyObject* vertex_type) = 0;
	virtual void AddModelVertex(PyObject* new_vertex) = 0;
	virtual void AddModelVertexFromArgs(PyObject* vertex_args, PyObject* vertex_kwargs);
	virtual int FinishModel() = 0;

	virtual int LoadTexture(std::string file_name) = 0;

	virtual int CreateModeledEntity(int model_id) = 0;
	virtual int CreateTexturedEntity(int model_id, int texture_id) = 0;

	virtual int GetNumberOfEntities() = 0;

protected:
	PyObject* current_model_vertex_type;
};

