#include "DXResourcePool.h"


DXResourcePool::DXResourcePool() : active_model_generator(NULL)
{
}


DXResourcePool::~DXResourcePool()
{
}


void DXResourcePool::Initialize(ID3D11Device* dev, ID3D11DeviceContext* dev_con) {
	device = dev;
	device_context = dev_con;
}

void DXResourcePool::BeginNewModel(PyObject* vertex_type) {
	current_model_vertex_type = vertex_type;
	BeginNewModel(PyVertexTypeToVertexType(vertex_type), D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void DXResourcePool::BeginNewModel(PyObject* vertex_type, PyObject* primitive_type) {
	current_model_vertex_type = vertex_type;
	BeginNewModel(PyVertexTypeToVertexType(vertex_type), PyPrimitiveTypeToD3DPrimitiveTopology(primitive_type));
}

void DXResourcePool::BeginNewModel(VertexType vertex_type, D3D_PRIMITIVE_TOPOLOGY primitive_type) {
	active_model_generator = new ModelGenerator(vertex_type, primitive_type);
}

void DXResourcePool::AddModelVertex(PyObject* new_vertex) {
	AddModelVertex(PyVertexToVertex(new_vertex));
}

void DXResourcePool::AddModelVertex(Vertex new_vertex) {
	if (active_model_generator == NULL) {
		OutputFormatted("Error attempting to add a vertex to a non-existant model generator");
		return;
	}
	active_model_generator->AddVertex(new_vertex);
}

int DXResourcePool::FinishModel() {
	if (active_model_generator != NULL) {
		Model* new_model = new Model;
		*new_model = active_model_generator->DumpModel(device, device_context);
		models.push_back(new_model);

		delete active_model_generator;
		active_model_generator = NULL;
		return models.size() - 1;
	} else {
		printf("Attempting to finish a model that doesn't exist\n");
		return -1;
	}
}

int DXResourcePool::LoadTexture(std::string file_name) {
	ID3D11Texture2D* loaded_texture;
	HRESULT load_result;
	D3DX11CreateTextureFromFile(
		device,
		file_name.c_str(),
		NULL,
		NULL,
		(ID3D11Resource**)&loaded_texture,
		&load_result);
	if (load_result != S_OK) {
		return -1;
	}
	Texture* new_tex = new Texture(true, false);
	D3D11_TEXTURE2D_DESC tex_desc;
	loaded_texture->GetDesc(&tex_desc);
	new_tex->Initialize(device, device_context, loaded_texture, { { tex_desc.Width, tex_desc.Height } });
	textures.push_back(new_tex);
	return textures.size() - 1;
}

int DXResourcePool::LoadShader(std::string file_name, PyObject* vertex_type) {
	return LoadShader(file_name, PyVertexTypeToVertexType(vertex_type));
}

int DXResourcePool::LoadShader(std::string file_name, VertexType vertex_type) {
	ShaderPipeline* new_shader = new ShaderPipeline;
	new_shader->Initialize(device, device_context, file_name, vertex_type.GetVertexType(), vertex_type.GetSizeVertexType());
	shaders.push_back(new_shader);
	return shaders.size() - 1;
}

void DXResourcePool::AddModelTransformations(std::vector<std::pair<ConstantBuffer*, int>>& transformations) {
	ConstantBufferTyped<TransformationMatrixAndInvTransData>* model_transformation = new ConstantBufferTyped<TransformationMatrixAndInvTransData>;
	
	model_transformation->Initialize(device, device_context);
	model_transformation->SetBothTransformations(DirectX::XMMatrixTranslation(0, 0, 0));
	model_transformation->CreateBuffer();
	transformations.push_back(std::make_pair(dynamic_cast<ConstantBuffer*>(model_transformation), PER_MODEL_CONSTANT_BUFFER_REGISTER));
}

int DXResourcePool::CreateModeledEntity(int model_id, int shader_id) {
	std::vector<std::pair<ConstantBuffer*, int>> transformations;
	AddModelTransformations(transformations);
	
	ModeledDrawHandler* draw_handler = new ModeledDrawHandler;
	draw_handler->Initialize(shaders[shader_id], transformations, models[model_id]);
	
	Entity* new_entity = new Entity;
	new_entity->Initialize((DrawHandler*)draw_handler);

	entities.push_back(new_entity);

	return entities.size() - 1;
}

int DXResourcePool::CreateTexturedEntity(int model_id, int shader_id, int texture_id) {
	std::vector<std::pair<ConstantBuffer*, int>> transformations;
	AddModelTransformations(transformations);

	TexturedDrawHandler* draw_handler = new TexturedDrawHandler;
	draw_handler->Initialize(shaders[shader_id], transformations, models[model_id], textures[texture_id], 0, 0);

	Entity* new_entity = new Entity;
	new_entity->Initialize(draw_handler);

	entities.push_back(new_entity);

	return entities.size() - 1;
}

void DXResourcePool::CreateRenderBundle(int render_bundle_id, int num_constant_buffers) {
	RenderBundle* new_render_bundle = new RenderBundle(device, device_context, num_constant_buffers);
	render_bundles.insert(std::make_pair(render_bundle_id, new_render_bundle));
}

int DXResourcePool::GetNumberOfEntities() {
	return entities.size();
}

Entity* DXResourcePool::GetEntity(int entity_index) {
	return entities[entity_index];
}

Model* DXResourcePool::GetModel(int model_index) {
	return models[model_index];
}

Texture* DXResourcePool::GetTexture(int texture_index) {
	return textures[texture_index];
}

RenderBundle* DXResourcePool::GetRenderBundle(int render_bundle_id) {
	return render_bundles.at(render_bundle_id);
}

VertexType DXResourcePool::PyVertexTypeToVertexType(PyObject* vertex_type) {
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


Vertex DXResourcePool::PyVertexToVertex(PyObject* vertex) {
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

DXGI_FORMAT DXResourcePool::PyRawTypeToDXGIFormat(PyObject* raw_type) {
	long number_of_floats = PyLong_AsLong(PyObject_GetAttrString(raw_type, "num_floats"));
	if (number_of_floats == 1) {
		return DXGI_FORMAT_R32_FLOAT;
	} else if (number_of_floats == 2) {
		return DXGI_FORMAT_R32G32_FLOAT;
	} else if (number_of_floats == 3) {
		return DXGI_FORMAT_R32G32B32_FLOAT;
	} else if (number_of_floats == 4) {
		return DXGI_FORMAT_R32G32B32A32_FLOAT;
	}
	OutputFormatted("Error parsing raw type with unknown DXGIFormat");

	return DXGI_FORMAT_UNKNOWN;
}

char* DXResourcePool::PyMetaTypeToSemantic(PyObject* meta_type) {
	PyObject* semantic_string = PyObject_GetAttrString(meta_type, "type_name");
	char* retval = PyUnicode_AsUTF8(semantic_string);
	return retval;
}

D3D_PRIMITIVE_TOPOLOGY DXResourcePool::PyPrimitiveTypeToD3DPrimitiveTopology(PyObject* primitive_type) {
	PyObject* py_enum_value = PyObject_GetAttrString(primitive_type, "value");
	return (D3D_PRIMITIVE_TOPOLOGY)PyLong_AsUnsignedLong(py_enum_value);
}