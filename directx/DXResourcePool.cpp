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
	VertexType formatted_vertex_type;
	BeginNewModel(formatted_vertex_type);
}

void DXResourcePool::BeginNewModel(VertexType vertex_type) {
	
}

void DXResourcePool::AddModelVertex(void* new_vertex) {
	if (active_vertex_type == TEXTUREVERTEX_ID) {
		auto cast_model_generator = dynamic_cast<TypedModelGenerator<TEXTUREVERTEX>*>(active_model_generator);
		if (cast_model_generator != NULL) {
			cast_model_generator->AddVertex(new_vertex);
		} else {
			printf("Attempting to add the wrong type of vertex\n");
		}
	} else if (active_vertex_type == COLORVERTEX_ID) {
		auto cast_model_generator = dynamic_cast<TypedModelGenerator<COLORVERTEX>*>(active_model_generator);
		if (cast_model_generator != NULL) {
			cast_model_generator->AddVertex(new_vertex);
		}
		else {
			printf("Attempting to add the wrong type of vertex\n");
		}
	}
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

int DXResourcePool::CreateModeledEntity(int model_id) {
	int* shader_number = new int;
	std::vector<std::pair<ConstantBuffer*, int>> transformations;
	ConstantBuffer* model_transformation = new ConstantBuffer;

	*shader_number = 1;

	model_transformation->Initialize(device, device_context);
	XMStoreFloat4x4(&(model_transformation->GetBufferData().transformation), DirectX::XMMatrixTranslation(0, 0, 0));
	model_transformation->CreateBuffer();
	transformations.push_back(std::make_pair(model_transformation, 1));
	
	ModeledDrawHandler* draw_handler = new ModeledDrawHandler;
	draw_handler->Initialize(shader_number, transformations, models[model_id]);
	
	Entity* new_entity = new Entity;
	new_entity->Initialize((DrawHandler*)draw_handler);

	entities.push_back(new_entity);

	return entities.size() - 1;
}

int DXResourcePool::CreateTexturedEntity(int model_id, int texture_id) {
	int* shader_number = new int;
	std::vector<std::pair<ConstantBuffer*, int>> transformations;
	ConstantBuffer* model_transformation = new ConstantBuffer;

	*shader_number = 0;

	model_transformation->Initialize(device, device_context);
	XMStoreFloat4x4(&(model_transformation->GetBufferData().transformation), DirectX::XMMatrixTranslation(0, 0, 0));
	model_transformation->CreateBuffer();
	transformations.push_back(std::make_pair(model_transformation, 1));

	TexturedDrawHandler* draw_handler = new TexturedDrawHandler;
	draw_handler->Initialize(shader_number, transformations, models[model_id], textures[texture_id], 0, 0);

	Entity* new_entity = new Entity;
	new_entity->Initialize(draw_handler);

	entities.push_back(new_entity);

	return entities.size() - 1;
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
