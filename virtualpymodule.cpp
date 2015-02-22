#include "Python.h"

#include <thread>
#include <mutex>
#include <condition_variable>

#include "FrameState.h"
#include "ResourcePool.h"
#include "WindowsMainLoop.h"
#include "directx/DirectxLoop.h"
#include "directx/DXResourcePool.h"
#include "directx/Vertices.h"

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
	unsigned int vertex_id;
	if (!PyArg_ParseTuple(args, "I", &vertex_id)) {
		Py_INCREF(Py_None);
		return Py_None;
	}
	resource_pool->BeginNewModel(VERTEX_SIZE_LOOKUP[vertex_id] * sizeof(float)); // Currently assume color vertices
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

PyObject* TextureVertex(PyObject* self, PyObject* args) {
	float vertex_data[5];
	if (!PyArg_ParseTuple(args, "fffff", vertex_data, vertex_data + 1, vertex_data + 2, vertex_data + 3, vertex_data + 4)) {
		return NULL;
	}
	resource_pool->AddModelVertex((void*)vertex_data);
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject* EndModel(PyObject* self, PyObject* args) {
	int entity_id = resource_pool->FinishModel();
	if (entity_id < 0) {
		Py_INCREF(Py_None);
		return Py_None;
	}
	return Py_BuildValue("i", entity_id);
}

PyObject* LoadTexture(PyObject* self, PyObject* args) {
	char* file_name_cstr;
	if (!PyArg_ParseTuple(args, "s", &file_name_cstr)) {
		Py_INCREF(Py_None);
		return Py_None;
	}
	std::string file_name(file_name_cstr);
	int tex_id = resource_pool->LoadTexture(resources_location + file_name);
	if (tex_id < 0) {
		Py_INCREF(Py_None);
		return Py_None;
	}
	return Py_BuildValue("i", tex_id);
}

PyObject* ShowModel(PyObject* self, PyObject* args, PyObject* kwargs) {
	int entity_id = -1;

	//static char *kwlist[] = { "entity_id", "x_pos", "y_pos", "z_pos", "x_scale", "y_scale", "z_scale", "a", "b", "c", "d", NULL };
	static char *kwlist[] = { "entity_id", "position", "scale", "rotation", NULL };
	float scale[3];
	PyObject* location_tuple;
	PyObject* scale_tuple;
	PyObject* orientation_tuple;

	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "i|OOO", kwlist,
		&entity_id, &location_tuple, &scale_tuple, &orientation_tuple)) {
		//new_entity_state.location.data(), new_entity_state.location.data() + 1, new_entity_state.location.data() + 2,
		//scale, scale+1, scale+2,
		//new_entity_state.orientation.data(), new_entity_state.orientation.data() + 1, new_entity_state.orientation.data() + 2, new_entity_state.orientation.data() + 3)) {
		return NULL;
	}
	
	// Ensures that there is an entity state for the current entity
	if (current_state.entities.size() <= entity_id) {
		current_state.entities.resize(entity_id + 1);
		current_state.entities[entity_id].entity_id = entity_id;
		current_state.entities[entity_id].location = { { 0.0f, 0.0f, 0.0f } };
		current_state.entities[entity_id].scale = { { 1.0f, 1.0f, 1.0f } };
		current_state.entities[entity_id].orientation = { { 0.0f, 0.0f, 0.0f, 1.0f } };
	}

	current_state.entities[entity_id].display_state = 1;

	if (location_tuple != Py_None) {
		if (!PyArg_ParseTuple(location_tuple, "fff",
			current_state.entities[entity_id].location.data(),
			current_state.entities[entity_id].location.data() + 1,
			current_state.entities[entity_id].location.data() + 2)) {
			return NULL;
		}
	}
	if (scale_tuple != Py_None) {
		if (!PyArg_ParseTuple(scale_tuple, "fff",
			current_state.entities[entity_id].scale.data(),
			current_state.entities[entity_id].scale.data() + 1,
			current_state.entities[entity_id].scale.data() + 2)) {
			return NULL;
		}
	}
	if (orientation_tuple != Py_None) {
		if (!PyArg_ParseTuple(orientation_tuple, "ffff",
			current_state.entities[entity_id].orientation.data(),
			current_state.entities[entity_id].orientation.data() + 1,
			current_state.entities[entity_id].orientation.data() + 2,
			current_state.entities[entity_id].orientation.data() + 3)) {
			return NULL;
		}
	}
	
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject* PushState(PyObject* self, PyObject* args) {
	state_buffer.PushState(current_state);
	FrameState new_state;
	new_state.color = current_state.color;
	current_state = new_state;
	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef virtualpy_methods[] = {
	{ "set_resources_location", SetResourcesLocation, METH_VARARGS, "set_resources_location() doc string" },
	{ "spawn_thread", SpawnThread, METH_VARARGS, "spawn_thread() doc string" },
	{ "set_color", SetColor, METH_VARARGS, "set_color() doc string" },
	{ "begin_model", BeginModel, METH_VARARGS, "begin_model() doc string" },
	{ "color_vertex", ColorVertex, METH_VARARGS, "color_vertex() doc string" },
	{ "texture_vertex", TextureVertex, METH_VARARGS, "texture_vertex() doc string" },
	{ "end_model", EndModel, METH_VARARGS, "end_model() doc string" },
	{ "load_texture", LoadTexture, METH_VARARGS, "load_texture() doc string" },
	{ "show_model", (PyCFunction)ShowModel, METH_VARARGS | METH_KEYWORDS, "show_model() doc string" },
	{ "push_state", PushState, METH_VARARGS, "push_state() doc string" },
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

