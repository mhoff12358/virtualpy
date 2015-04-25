#include "ResourcePool.h"


ResourcePool::ResourcePool()
{
}


ResourcePool::~ResourcePool()
{
}

void ResourcePool::Initialize() {

}

PyObject* ResourcePool::AddModelVertexFromArgs(PyObject* vertex_args, PyObject* vertex_kwargs) {
	LARGE_INTEGER a, b, c, d;
	QueryPerformanceCounter(&a);
	PyObject* vertex_creation_meth = PyObject_GetAttrString(current_model_vertex_type, "createVertex");
	AddModelVertex(PyObject_Call(vertex_creation_meth, vertex_args, vertex_kwargs));
	Py_RETURN_NONE;
}

PyObject* ResourcePool::AddModelVertexFromArgsFast(PyObject* vertex_args) {
	PyObject* vertex_creation_meth = PyObject_GetAttrString(current_model_vertex_type, "createVertexFast");
	PyObject* vertex = PyObject_Call(vertex_creation_meth, vertex_args, NULL);
	PY_ERR_CHK;
	AddModelVertex(vertex);
	Py_RETURN_NONE;
}

PyObject* ResourcePool::AddModelVertexFromArgsBatch(PyObject* vertex_args_kwargs_list) {
	PyObject* vertex_creation_meth = PyObject_GetAttrString(current_model_vertex_type, "createVertexBatch");
	PY_ERR_CHK;
	PyObject* vertices = PyObject_Call(vertex_creation_meth, vertex_args_kwargs_list, NULL);
	PY_ERR_CHK;
	AddModelVertexBatch(vertices);
	Py_RETURN_NONE;
}