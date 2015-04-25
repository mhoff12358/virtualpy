#include "ResourcePool.h"


ResourcePool::ResourcePool()
{
}


ResourcePool::~ResourcePool()
{
}

void ResourcePool::Initialize() {

}

void ResourcePool::AddModelVertexFromArgs(PyObject* vertex_args, PyObject* vertex_kwargs) {
	LARGE_INTEGER a, b, c, d;
	QueryPerformanceCounter(&a);
	PyObject* vertex_creation_meth = PyObject_GetAttrString(current_model_vertex_type, "createVertex");
	AddModelVertex(PyObject_Call(vertex_creation_meth, vertex_args, vertex_kwargs));
}

void ResourcePool::AddModelVertexFromArgsFast(PyObject* vertex_args) {
	PyObject* vertex_creation_meth = PyObject_GetAttrString(current_model_vertex_type, "createVertexFast");
	AddModelVertex(PyObject_Call(vertex_creation_meth, vertex_args, NULL));
}
