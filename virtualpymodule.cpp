#include "Python.h"

#include <thread>

#include "FrameState.h"
#include "MainLoop.h"

namespace virtualpy {
FrameStateBuffer state_buffer;
FrameState current_state;

void NewThread() {
	MainLoop* main_loop = new PrintColor(&state_buffer);

	main_loop->Begin();
}

PyObject* SpawnThread(PyObject* self, PyObject* args) {
	std::thread new_thread(NewThread);
	new_thread.detach();

	Py_INCREF(Py_None);
	return Py_None;
}

PyObject* SetColor(PyObject* self, PyObject* args) {
	if (!PyArg_ParseTuple(args, "fff", current_state.color.data(), current_state.color.data() + 1, current_state.color.data() + 2)) {
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject* PushState(PyObject* self, PyObject* args) {
	state_buffer.PushState(current_state);
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
	{ "set_color", SetColor, METH_VARARGS, "set_color() doc string" },
	{ "push_state", PushState, METH_VARARGS, "push_state() doc string" },
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

