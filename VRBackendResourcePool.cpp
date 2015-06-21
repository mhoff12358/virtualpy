#include "VRBackendResourcePool.h"

namespace virtualpy {

	VRBackendResourcePool::VRBackendResourcePool()
	{
	}


	VRBackendResourcePool::~VRBackendResourcePool()
	{
	}

	void VRBackendResourcePool::Initialize(::ResourcePool* vr_backend_resource_pool, EntityHandler* vr_backend_entity_handler) {
		ResourcePool::Initialize();
		backend_resource_pool = vr_backend_resource_pool;
		backend_entity_handler = vr_backend_entity_handler;
	}

	void VRBackendResourcePool::BeginNewModel(PyObject* vertex_type) {
		BeginNewModel(PyVertexTypeToVertexType(vertex_type), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void VRBackendResourcePool::BeginNewModel(PyObject* vertex_type, PyObject* primitive_type) {
		BeginNewModel(PyVertexTypeToVertexType(vertex_type), PyPrimitiveTypeToD3DPrimitiveTopology(primitive_type));
	}

	void VRBackendResourcePool::BeginNewModel(VertexType vertex_type, D3D_PRIMITIVE_TOPOLOGY primitive_type) {
		temp_model_vertices.clear();
		temp_model_vertex_type = vertex_type;
		temp_model_topology = primitive_type;
	}

	void VRBackendResourcePool::AddModelVertex(PyObject* new_vertex) {
		AddModelVertex(PyVertexToVertex(new_vertex));
	}

	void VRBackendResourcePool::AddModelVertexBatch(PyObject* new_vertexes) {
		PyObject* fast_seq = PySequence_Fast(new_vertexes, "expected a sequence");
		int num_vertices = PySequence_Size(fast_seq);
		temp_model_vertices.reserve(temp_model_vertices.size() + num_vertices);
		for (int i = 0; i < num_vertices; i++) {
			AddModelVertex(PyVertexToVertex(PySequence_Fast_GET_ITEM(fast_seq, i)));
		}
		Py_DECREF(fast_seq);
	}

	void VRBackendResourcePool::AddModelVertex(Vertex new_vertex) {
		temp_model_vertices.push_back(new_vertex);
	}

	int VRBackendResourcePool::FinishModel() {
		model_store.push_back(backend_resource_pool->LoadModel("", temp_model_vertices, temp_model_topology));
		return model_store.size() - 1;
	}

	int VRBackendResourcePool::LoadModelFromFile(std::string file_name) {
		model_store.push_back(backend_resource_pool->LoadModel(file_name));
		return model_store.size() - 1;
	}

	int VRBackendResourcePool::LoadPixelShader(std::string file_name) {
		pixel_shader_store.push_back(backend_resource_pool->LoadPixelShader(file_name));
		return pixel_shader_store.size() - 1;
	}

	int VRBackendResourcePool::LoadVertexShader(std::string file_name, PyObject* vertex_type) {
		vertex_shader_store.push_back(backend_resource_pool->LoadVertexShader(file_name, PyVertexTypeToVertexType(vertex_type)));
		return vertex_shader_store.size() - 1;
	}

	int VRBackendResourcePool::CreateObjectSettings() {
		ConstantBufferTyped<TransformationMatrixAndInvTransData>* new_object_settings = new ConstantBufferTyped<TransformationMatrixAndInvTransData>(CB_PS_VERTEX_SHADER);
		new_object_settings->CreateBuffer(backend_resource_pool->device_interface);
		object_setting_store.push_back(new_object_settings);
		return object_setting_store.size() - 1;
	}

	void VRBackendResourcePool::CreateRenderBundle(int render_bundle_id, int num_constant_buffers, std::vector<char> pipeline_stages) {
		assert(false);
	}

	int VRBackendResourcePool::CreateModeledEntity(int model_id, int vertex_shader_id, int pixel_shader_id, int object_setting_id) {
		return backend_entity_handler->AddEntity(Entity(
			ES_NORMAL,
			pixel_shader_store[pixel_shader_id],
			vertex_shader_store[vertex_shader_id],
			ShaderSettings(NULL),
			model_store[model_id],
			object_setting_store[object_setting_id]));
	}

	VertexType VRBackendResourcePool::PyVertexTypeToVertexType(PyObject* vertex_type) {
		std::vector<D3D11_INPUT_ELEMENT_DESC> vertex_element_descriptors;

		PyObject* type_list = PyObject_GetAttrString(vertex_type, "type_def");
		Py_ssize_t number_of_elements = PySequence_Length(type_list);
		unsigned long existing_size = 0;
		for (Py_ssize_t i = 0; i < number_of_elements; i++) {
			PyObject* element_pair = PyList_GetItem(type_list, i);
			PyObject* meta_type = PyTuple_GetItem(element_pair, 0);
			PyObject* raw_type = PyTuple_GetItem(element_pair, 1);

			char* semantic = PyMetaTypeToSemantic(meta_type);
			DXGI_FORMAT dxgi_format = PyRawTypeToDXGIFormat(raw_type);
			D3D11_INPUT_ELEMENT_DESC new_element_desc = {
				semantic,
				0,
				dxgi_format,
				0,
				existing_size,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			};
			vertex_element_descriptors.push_back(new_element_desc);

			existing_size += PyLong_AsUnsignedLong(PyObject_GetAttrString(raw_type, "num_floats"))*sizeof(float);
		}

		return VertexType(vertex_element_descriptors);
	}


	Vertex VRBackendResourcePool::PyVertexToVertex(PyObject* vertex) {
		VertexType vertex_type = PyVertexTypeToVertexType(PyObject_GetAttrString(vertex, "vertex_type"));
		PyObject* data_list = PyObject_GetAttrString(vertex, "data_list");
		std::vector<float> data_vec;

		Py_ssize_t number_data_elements = PyList_Size(data_list);
		for (Py_ssize_t i = 0; i < number_data_elements; i++) {
			PyObject* data_block_tuple = PyList_GetItem(data_list, i);
			Py_ssize_t size_data_block = PyTuple_Size(data_block_tuple);
			for (Py_ssize_t j = 0; j < size_data_block; j++) {
				data_vec.push_back(PyFloat_AsDouble(PyTuple_GetItem(data_block_tuple, j)));
			}
		}

		return Vertex(vertex_type, data_vec);
	}

	DXGI_FORMAT VRBackendResourcePool::PyRawTypeToDXGIFormat(PyObject* raw_type) {
		long number_of_floats = PyLong_AsLong(PyObject_GetAttrString(raw_type, "num_floats"));
		if (number_of_floats == 1) {
			return DXGI_FORMAT_R32_FLOAT;
		}
		else if (number_of_floats == 2) {
			return DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (number_of_floats == 3) {
			return DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (number_of_floats == 4) {
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		OutputFormatted("Error parsing raw type with unknown DXGIFormat");

		return DXGI_FORMAT_UNKNOWN;
	}

	char* VRBackendResourcePool::PyMetaTypeToSemantic(PyObject* meta_type) {
		PyObject* semantic_string = PyObject_GetAttrString(meta_type, "type_name");
		char* retval = PyUnicode_AsUTF8(semantic_string);
		return retval;
	}

	D3D_PRIMITIVE_TOPOLOGY VRBackendResourcePool::PyPrimitiveTypeToD3DPrimitiveTopology(PyObject* primitive_type) {
		PyObject* py_enum_value = PyObject_GetAttrString(primitive_type, "value");
		return (D3D_PRIMITIVE_TOPOLOGY)PyLong_AsUnsignedLong(py_enum_value);
	}


} // virtualpy