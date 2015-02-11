#include "Python.h"

#include <thread>
#include <mutex>
#include <condition_variable>

#include "FrameState.h"
#include "ResourcePool.h"
#include "WindowsMainLoop.h"
#include "directx/DirectxLoop.h"
#include "directx/DXResourcePool.h"

namespace virtualpy {
FrameStateBuffer state_buffer;
FiveSecondNoPredictInterpolater state_interpolater(&state_buffer);
ResourcePool* resource_pool;
FrameState current_state;
std::string resources_location(".");

void NewThread() {
	//MainLoop* main_loop = new PrintColor(&state_interpolater);

	//main_loop->Begin();
}

PyObject* SetResourcesLocation(PyObject* self, PyObject* args) {
	char* res_loc_text;
	if (!PyArg_ParseTuple(args, "s", &res_loc_text)) {
		return NULL;
	}

	resources_location.clear();
	resources_location.insert(0, res_loc_text);

	Py_INCREF(Py_None);
	return Py_None;
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
		std::mutex preparation_mutex;
		std::condition_variable preparation_cv;
		std::unique_lock<std::mutex> preparation_lock(preparation_mutex);
		resource_pool = new DXResourcePool();
		DirectxLoop* main_loop = new DirectxLoop(false, resources_location, (DXResourcePool*)resource_pool, &state_interpolater);
		std::thread new_thread(&DirectxLoop::BeginWithPrep, main_loop, &preparation_mutex, &preparation_cv);
		new_thread.detach();
		preparation_cv.wait(preparation_lock);
	}
	else if (strcmp(version_string, "directx_oculus") == 0) {
		std::mutex preparation_mutex;
		std::condition_variable preparation_cv;
		std::unique_lock<std::mutex> preparation_lock(preparation_mutex);
		resource_pool = new DXResourcePool();
		DirectxLoop* main_loop = new DirectxLoop(true, resources_location, (DXResourcePool*)resource_pool, &state_interpolater);
		std::thread new_thread(&DirectxLoop::BeginWithPrep, main_loop, &preparation_mutex, &preparation_cv);
		new_thread.detach();
		preparation_cv.wait(preparation_lock);
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

PyObject* BeginModel(PyObject* self, PyObject* args) {
	resource_pool->BeginNewModel(7 * sizeof(float)); // Currently assume color vertices
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject* ColorVertex(PyObject* self, PyObject* args) {
	float vertex_data[7];
	if (!PyArg_ParseTuple(args, "fffffff", vertex_data, vertex_data + 1, vertex_data + 2, vertex_data + 3, vertex_data + 4, vertex_data + 5, vertex_data + 6)) {
		return NULL;
	}
	resource_pool->AddModelVertex((void*)vertex_data);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject* EndModel(PyObject* self, PyObject* args) {
	resource_pool->FinishModel();
	current_state.number_of_entities = resource_pool->GetNumberOfEntities();
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
	{ "set_resources_location", SetResourcesLocation, METH_VARARGS, "set_resources_location() doc string" },
	{ "spawn_thread", SpawnThread, METH_VARARGS, "spawn_thread() doc string" },
	{ "set_color", SetColor, METH_VARARGS, "set_color() doc string" },
	{ "begin_model", BeginModel, METH_VARARGS, "begin_model() doc string" },
	{ "color_vertex", ColorVertex, METH_VARARGS, "color_vertex() doc string" },
	{ "end_model", EndModel, METH_VARARGS, "end_model() doc string" },
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

