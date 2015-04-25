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
	QueryPerformanceCounter(&b);
	PyObject* vertex = PyObject_Call(vertex_creation_meth, vertex_args, vertex_kwargs);
	QueryPerformanceCounter(&c);
	AddModelVertex(vertex);
	QueryPerformanceCounter(&d);
	printf("%u\t%u\t%u\n", b.QuadPart - a.QuadPart, c.QuadPart - b.QuadPart, d.QuadPart-c.QuadPart);
}