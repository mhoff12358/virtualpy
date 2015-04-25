#include "Python.h"

#include <thread>
#include <mutex>
#include <condition_variable>

#include "PyHelper.h"

#include "FrameState.h"
#include "IOState.h"
#include "ResourcePool.h"
#include "WindowsMainLoop.h"
#include "directx/DirectxLoop.h"
#include "directx/DXResourcePool.h"
#include "directx/Vertices.h"

namespace vpdb {
	PyObject* pydebug_fn;
}

namespace virtualpy {
FrameStateBuffer state_buffer;
ConstantDelayNoPreemeptingExtrapolateInterpolater state_interpolater(&state_buffer, 0.5);
ResourcePool* resource_pool;
FrameState current_state;
int current_render_bundle = 0;
int new_render_bundle_id = 1;
std::string resources_location(".");

IOStateBuffer io_state_buffer;


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

	PY_ERR_CHK;
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject* SpawnThread(PyObject* self, PyObject* args) {
	char* version_string;
	if (!PyArg_ParseTuple(args, "s", &version_string)) {
		return NULL;
	}

	if (strcmp(version_string, "console") == 0) {
		PrintColor* main_loop = new PrintColor(&state_interpolater, &io_state_buffer);
		std::thread new_thread(&PrintColor::Begin, main_loop);
		new_thread.detach();
	}
	else if (strcmp(version_string, "directx") == 0) {
		std::mutex preparation_mutex;
		std::condition_variable preparation_cv;
		std::unique_lock<std::mutex> preparation_lock(preparation_mutex);
		resource_pool = new DXResourcePool();
		DirectxLoop* main_loop = new DirectxLoop(false, resources_location, (DXResourcePool*)resource_pool, &state_interpolater, &io_state_buffer);
		std::thread new_thread(&DirectxLoop::BeginWithPrep, main_loop, &preparation_mutex, &preparation_cv);
		new_thread.detach();
		preparation_cv.wait(preparation_lock);
	}
	else if (strcmp(version_string, "directx_oculus") == 0) {
		std::mutex preparation_mutex;
		std::condition_variable preparation_cv;
		std::unique_lock<std::mutex> preparation_lock(preparation_mutex);
		resource_pool = new DXResourcePool();
		DirectxLoop* main_loop = new DirectxLoop(true, resources_location, (DXResourcePool*)resource_pool, &state_interpolater, &io_state_buffer);
		std::thread new_thread(&DirectxLoop::BeginWithPrep, main_loop, &preparation_mutex, &preparation_cv);
		new_thread.detach();
		preparation_cv.wait(preparation_lock);
	}
	
	PY_ERR_CHK;
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject* SetColor(PyObject* self, PyObject* args) {
	if (!PyArg_ParseTuple(args, "fff", current_state.color.data(), current_state.color.data() + 1, current_state.color.data() + 2)) {
		return NULL;
	}
	PY_ERR_CHK;
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject* BeginModel(PyObject* self, PyObject* args) {
	assert(!PyErr_Occurred());
	PyObject* vertex_type;
	PyObject* primitive_type = Py_None;
	if (!PyArg_ParseTuple(args, "O|O", &vertex_type, &primitive_type)) {
		return NULL;
	}
	
	if (PyErr_Occurred()) return NULL;
	if (primitive_type == Py_None) {
		resource_pool->BeginNewModel(vertex_type);
	} else {
		resource_pool->BeginNewModel(vertex_type, primitive_type);
	}
	PY_ERR_CHK;
	Py_INCREF(Py_None);
	return Py_None;
}

/*PyObject* ColorVertex(PyObject* self, PyObject* args) {
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
}*/

PyObject* AddVertex(PyObject* self, PyObject* args, PyObject* kwargs) {
	return resource_pool->AddModelVertexFromArgs(args, kwargs);
}

PyObject* AddVertexFast(PyObject* self, PyObject* args) {
	return resource_pool->AddModelVertexFromArgsFast(args);
}

PyObject* AddVertexBatch(PyObject* self, PyObject* args) {
	return resource_pool->AddModelVertexFromArgsBatch(args);
}

PyObject* EndModel(PyObject* self, PyObject* args) {
	int entity_id = resource_pool->FinishModel();
	if (entity_id < 0) {
		Py_INCREF(Py_None);
		return Py_None;
	}
	PY_ERR_CHK;
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
	PY_ERR_CHK;
	return Py_BuildValue("i", tex_id);
}

PyObject* LoadShader(PyObject* self, PyObject* args) {
	char* file_name_cstr;
	PyObject* vertex_type;
	if (!PyArg_ParseTuple(args, "sO", &file_name_cstr, &vertex_type)) {
		Py_INCREF(Py_None);
		return Py_None;
	}
	std::string file_name(file_name_cstr);
	int shad_id = resource_pool->LoadShader(resources_location + file_name, vertex_type);
	if (shad_id < 0) {
		Py_INCREF(Py_None);
		return Py_None;
	}
	PY_ERR_CHK;
	return Py_BuildValue("i", shad_id);
}

PyObject* CreateModeledEntity(PyObject* self, PyObject* args) {
	unsigned int model_id;
	unsigned int shader_id;
	if (!PyArg_ParseTuple(args, "II", &model_id, &shader_id)) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	int entity_id = resource_pool->CreateModeledEntity(model_id, shader_id);
	PY_ERR_CHK;
	return Py_BuildValue("i", entity_id);
}

PyObject* CreateTexturedEntity(PyObject* self, PyObject* args) {
	unsigned int model_id;
	unsigned int shader_id;
	unsigned int texture_id;
	if (!PyArg_ParseTuple(args, "III", &model_id, &shader_id, &texture_id)) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	int entity_id = resource_pool->CreateTexturedEntity(model_id, shader_id, texture_id);
	PY_ERR_CHK;
	return Py_BuildValue("i", entity_id);
}

PyObject* AddModelToFrameState(int render_bundle_id, int entity_id, PyObject* location_tuple, PyObject* scale_tuple, PyObject* orientation_quaternion) {
	// Ensures that there is an entity state for the current entity
	if (current_state.entities.size() <= entity_id) {
		current_state.entities.resize(entity_id + 1);
	}
		current_state.entities[entity_id].entity_id = entity_id;
		current_state.entities[entity_id].position.location = { { 0.0f, 0.0f, 0.0f } };
		current_state.entities[entity_id].position.scale = { { 1.0f, 1.0f, 1.0f } };
		current_state.entities[entity_id].position.orientation = { { 0.0f, 0.0f, 0.0f, 1.0f } };

	current_state.entities[entity_id].render_bundle_id = render_bundle_id;

	if (location_tuple != Py_None) {
		if (!PyArg_ParseTuple(location_tuple, "fff",
			current_state.entities[entity_id].position.location.data(),
			current_state.entities[entity_id].position.location.data() + 1,
			current_state.entities[entity_id].position.location.data() + 2)) {
			return NULL;
		}
	}
	if (scale_tuple != Py_None) {
		if (!PyArg_ParseTuple(scale_tuple, "fff",
			current_state.entities[entity_id].position.scale.data(),
			current_state.entities[entity_id].position.scale.data() + 1,
			current_state.entities[entity_id].position.scale.data() + 2)) {
			return NULL;
		}
	}
	if (orientation_quaternion != Py_None) {
		PyObject* orientation_tuple = PyObject_GetAttrString(orientation_quaternion, "vector");
		if (orientation_tuple == NULL) {
			return NULL;
		}
		if (!PyArg_ParseTuple(orientation_tuple, "ffff",
			current_state.entities[entity_id].position.orientation.data(),
			current_state.entities[entity_id].position.orientation.data() + 1,
			current_state.entities[entity_id].position.orientation.data() + 2,
			current_state.entities[entity_id].position.orientation.data() + 3)) {
			return NULL;
		}
	}

	PY_ERR_CHK;
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject* ShowModelWithRenderBundle(PyObject* self, PyObject* args, PyObject* kwargs) {
	int entity_id = -1;
	int render_bundle_id = -2;

	static char *kwlist[] = { "render_bundle_id", "entity_id", "position", "scale", "rotation", NULL };
	PyObject* location_tuple;
	PyObject* scale_tuple;
	PyObject* orientation_quaternion;

	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ii|OOO", kwlist,
		&render_bundle_id, &entity_id, &location_tuple, &scale_tuple, &orientation_quaternion)) {
		//new_entity_state.location.data(), new_entity_state.location.data() + 1, new_entity_state.location.data() + 2,
		//scale, scale+1, scale+2,
		//new_entity_state.orientation.data(), new_entity_state.orientation.data() + 1, new_entity_state.orientation.data() + 2, new_entity_state.orientation.data() + 3)) {
		return NULL;
	}

	return AddModelToFrameState(render_bundle_id, entity_id, location_tuple, scale_tuple, orientation_quaternion);
}

PyObject* ShowModel(PyObject* self, PyObject* args, PyObject* kwargs) {
	int entity_id = -1;

	//static char *kwlist[] = { "entity_id", "x_pos", "y_pos", "z_pos", "x_scale", "y_scale", "z_scale", "a", "b", "c", "d", NULL };
	static char *kwlist[] = { "entity_id", "position", "scale", "rotation", NULL };
	PyObject* location_tuple;
	PyObject* scale_tuple;
	PyObject* orientation_quaternion;

	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "i|OOO", kwlist,
		&entity_id, &location_tuple, &scale_tuple, &orientation_quaternion)) {
		//new_entity_state.location.data(), new_entity_state.location.data() + 1, new_entity_state.location.data() + 2,
		//scale, scale+1, scale+2,
		//new_entity_state.orientation.data(), new_entity_state.orientation.data() + 1, new_entity_state.orientation.data() + 2, new_entity_state.orientation.data() + 3)) {
		return NULL;
	}

	return AddModelToFrameState(virtualpy::current_render_bundle, entity_id, location_tuple, scale_tuple, orientation_quaternion);
}

PyObject* CreateRenderBundle(PyObject* self, PyObject* args) {
	// Parse the args as a number of constant buffers
	int num_constant_buffers = 1;
	PyObject* pipeline_stage_bindings = Py_None;
	if (!PyArg_ParseTuple(args, "|iO", &num_constant_buffers, &pipeline_stage_bindings)) {
		return NULL;
	}

	RenderBundleState new_render_bundle(num_constant_buffers);
	std::vector<char> pipeline_stages;
	// If the pipeline_stage_bindings are specified, apply them. Else, bind them all to vertex buffers
	if (pipeline_stage_bindings != Py_None) {
		for (int i = 0; i < num_constant_buffers; i++) {
			PyObject* binding_type = PySequence_GetItem(pipeline_stage_bindings, i);
			pipeline_stages.push_back((char)PyLong_AsLong(binding_type));
		}
	} else {
		pipeline_stages.resize(num_constant_buffers, 1);
		//for (int i = 0; i < num_constant_buffers; i++) {
		//	new_render_bundle.constant_buffers[i].pipeline_stage = 1;
		//}
	}
	current_state.render_bundles.insert(std::make_pair(new_render_bundle_id, new_render_bundle));
	resource_pool->CreateRenderBundle(new_render_bundle_id, 1, pipeline_stages);
	PyObject* returned_id = Py_BuildValue("i", new_render_bundle_id);
	new_render_bundle_id++;

	PY_ERR_CHK;
	return returned_id;
}

PyObject* UpdateRenderBundle(PyObject* self, PyObject* args) {
	// Parse the args as a series of float 4-tuples
	int render_bundle_id;
	PyObject* values;
	if (!PyArg_ParseTuple(args, "iO", &render_bundle_id, &values)) {
		return NULL;
	}
	PY_ERR_CHK;
	RenderBundleState& rbs = current_state.render_bundles[render_bundle_id];
	
	rbs.constant_buffers.resize(PySequence_Size(values));

	PyObject* iter = PyObject_GetIter(values);
	PyObject* item;
	int i = 0;
	while (item = PyIter_Next(iter)) {
		float* constant_buffer_data_ptr = rbs.constant_buffers[i].data.data();
		for (int j = 0; j < 4; j++) {
			PyObject* float_val = PySequence_GetItem(item, j);
			constant_buffer_data_ptr[j] = PyFloat_AsDouble(float_val);
			Py_DECREF(float_val);
		}
		// Always release the reference to my item
		Py_DECREF(item);
		i++;
	}
	PY_ERR_CHK;
	
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject* SetCameraLocation(PyObject* self, PyObject* args) {
	std::array<float, 3> location;
	if (!PyArg_ParseTuple(args, "(fff)", location.data(), location.data() + 1, location.data() + 2)) {
		return NULL;
	}
	current_state.camera_position.location = location;

	PY_ERR_CHK;
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject* PushState(PyObject* self, PyObject* args) {
	state_buffer.PushState(current_state);
	FrameState new_state;
	new_state.color = current_state.color;
	current_state = new_state;
	current_render_bundle = 0;

	PY_ERR_CHK;
	Py_INCREF(Py_None);
	return Py_None;
}

PyObject* GetKeysAtState(const IOState& io_state) {
	PyObject* keyboard_list = PyList_New(256);
	for (int i = 0; i < 256; ++i) {
		// Set the value to 0 or 1 based on the highest order bit of the BYTE
		PyList_SetItem(keyboard_list, i, Py_BuildValue("B", io_state.keyboard_at_request[i] / 128));
	}
	return keyboard_list;
}

PyObject* GetKeysSinceState(const IOState& io_state) {
	PyObject* keyboard_list = PyList_New(256);
	for (int i = 0; i < 256; ++i) {
		PyList_SetItem(keyboard_list, i, Py_BuildValue("B", io_state.keyboard_since_request[i] / 128));
	}
	return keyboard_list;
}

PyObject* GetKeyboardState(PyObject* self, PyObject* args) {
	IOState latest_state = io_state_buffer.ReadState();
	PyObject* keys_at_state = GetKeysAtState(latest_state);
	PyObject* keys_since_state = GetKeysSinceState(latest_state);
	PY_ERR_CHK;
	return PyTuple_Pack(2, keys_at_state, keys_since_state);
}

PyObject* SetActiveRenderBundle(PyObject* self, PyObject* args) {
	int new_render_bundle = 0;
	if (PySequence_Length(args) != 0) {
		if (!PyArg_ParseTuple(args, "i", &new_render_bundle)) {
			return NULL;
		}	
	}
	current_render_bundle = new_render_bundle;
	PY_ERR_CHK;
	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef virtualpy_methods[] = {
	{ "set_resources_location", SetResourcesLocation, METH_VARARGS, "set_resources_location() doc string" },
	{ "spawn_thread", SpawnThread, METH_VARARGS, "spawn_thread() doc string" },
	{ "set_color", SetColor, METH_VARARGS, "set_color() doc string" },
	{ "begin_model", BeginModel, METH_VARARGS, "begin_model() doc string" },
	//{ "color_vertex", ColorVertex, METH_VARARGS, "color_vertex() doc string" },
	//{ "texture_vertex", TextureVertex, METH_VARARGS, "texture_vertex() doc string" },
	{ "add_vertex", (PyCFunction)AddVertex, METH_VARARGS | METH_KEYWORDS, "add_vertex() doc string" },
	{ "add_vertex_fast", AddVertexFast, METH_VARARGS, "add_vertex_fast() doc string" },
	{ "add_vertex_batch", AddVertexBatch, METH_VARARGS, "add_vertex_fast() doc string" },
	{ "end_model", EndModel, METH_VARARGS, "end_model() doc string" },
	{ "load_texture", LoadTexture, METH_VARARGS, "load_texture() doc string" },
	{ "load_shader", LoadShader, METH_VARARGS, "load_shader() doc string" },
	{ "create_modeled_entity", CreateModeledEntity, METH_VARARGS, "create_modeled_entity() doc string" },
	{ "create_textured_entity", CreateTexturedEntity, METH_VARARGS, "create_textured_entity() doc string" },
	{ "create_render_bundle", CreateRenderBundle, METH_VARARGS, "create_render_bundle() doc string" },
	{ "update_render_bundle", UpdateRenderBundle, METH_VARARGS, "update_render_bundle() doc string" },
	{ "set_active_render_bundle", SetActiveRenderBundle, METH_VARARGS, "set_active_render_bundle() doc string" },
	{ "show_model", (PyCFunction)ShowModel, METH_VARARGS | METH_KEYWORDS, "show_model() doc string" },
	{ "show_model_with_render_bundle", (PyCFunction)ShowModelWithRenderBundle, METH_VARARGS | METH_KEYWORDS, "show_model_with_render_bundle() doc string" },
	{ "set_camera_location", SetCameraLocation, METH_VARARGS, "set_camera_location() doc string" },
	{ "push_state", PushState, METH_VARARGS, "push_state() doc string" },
	{ "get_keyboard_state", GetKeyboardState, METH_VARARGS, "get_keyboard_state() doc string" },
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

void MergeModuleDictionaryElement(PyObject* dest_mod, PyObject* src_mod, const char* elem_name) {
	PyDict_SetItemString(dest_mod, elem_name, PyDict_GetItemString(src_mod, elem_name));
}

PyMODINIT_FUNC
PyInit_virtualpy(void)
{
	PyObject* unfinished_module = PyModule_Create(&virtualpy::virtualpymodule);
	PyObject* unfinished_module_dict = PyModule_GetDict(unfinished_module);

	PyObject* helper_module = PyImport_ImportModule("virtualpy_helpers");
	PyObject* helper_module_dict = PyModule_GetDict(helper_module);

	PyObject* classes_module = PyImport_ImportModule("virtualpy_classes");
	PY_ERR_CHK;
	PyObject* classes_module_dict = PyModule_GetDict(classes_module);

	PyObject* classes_module_items = PyDict_Items(classes_module_dict);
	Py_ssize_t num_module_items = PyList_Size(classes_module_items);


	vpdb::pydebug_fn = PyDict_GetItemString(helper_module_dict, "debugObj");

	MergeModuleDictionaryElement(unfinished_module_dict, classes_module_dict, "RawType");
	MergeModuleDictionaryElement(unfinished_module_dict, classes_module_dict, "MetaType");
	MergeModuleDictionaryElement(unfinished_module_dict, classes_module_dict, "VertexType");
	MergeModuleDictionaryElement(unfinished_module_dict, classes_module_dict, "Vertex");
	MergeModuleDictionaryElement(unfinished_module_dict, classes_module_dict, "Quaternion");
	MergeModuleDictionaryElement(unfinished_module_dict, classes_module_dict, "PrimitiveType");
	

	return unfinished_module;
}

