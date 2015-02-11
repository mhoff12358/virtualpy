#include "Python.h"

#include <thread>

#include "FrameState.h"
#include "WindowsMainLoop.h"
#include "directx/DirectxLoop.h"

namespace virtualpy {
FrameStateBuffer state_buffer;
FiveSecondNoPredictInterpolater state_interpolater(&state_buffer);
FrameState current_state;

void NewThread() {
	//MainLoop* main_loop = new PrintColor(&state_interpolater);

	//main_loop->Begin();
}

PyObject* SpawnThread(PyObject* self, PyObject* args) {
	char* version_string;
	if (!PyArg_ParseTuple(args, "s", &version_string)) {
		return NULL;
	}

	if (strcmp(version_string, "console") == 0) {
		PrintColor* main_loop = new PrintColor(&state_interpolater);
		std::thread new_thread(&PrintColor::Begin, main_loop);
		new_thread.detach();
	}
	else if (strcmp(version_string, "directx") == 0) {
		DirectxLoop* main_loop = new DirectxLoop(false, "C:\\Users\\mhoff_000\\Documents\\cpython\\virtualpy_nt\\resources\\", &state_interpolater);
		std::thread new_thread(&DirectxLoop::Begin, main_loop);
		new_thread.detach();
	}
	else if (strcmp(version_string, "directx_oculus") == 0) {
		DirectxLoop* main_loop = new DirectxLoop(true, "C:\\Users\\mhoff_000\\Documents\\cpython\\virtualpy_nt\\resources\\", &state_interpolater);
		std::thread new_thread(&DirectxLoop::Begin, main_loop);
		new_thread.detach();
	}
	
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

