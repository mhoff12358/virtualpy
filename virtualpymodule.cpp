#include "Python.h"

#include <thread>

#include "FrameState.h"

namespace virtualpy {
FrameStateBuffer state_buffer;

void NewThread() {
	printf("HELLO!\n");
}

PyObject* SpawnThread(PyObject* self, PyObject* args) {
	std::thread new_thread(NewThread);
	new_thread.detach();

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject* ex_foo(PyObject *self, PyObject *args)
{
	printf("Hello, world\n");
	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef virtualpy_methods[] = {
	{ "spawn_thread", SpawnThread, METH_VARARGS, "spawn_thread() doc string" },
	{ "foo", ex_foo, METH_VARARGS, "foo() doc string" },
	{ NULL, NULL }
};

struct PyModuleDef virtualpymodule = {
	PyModuleDef_HEAD_INIT,
	"virtualpy",
	"virtualpy module doc string",
	-1,
	virtualpy_methods,
	NULL,
	NULL,
	NULL,
	NULL
};
}

PyMODINIT_FUNC
PyInit_virtualpy(void)
{
	return PyModule_Create(&virtualpy::virtualpymodule);
}

