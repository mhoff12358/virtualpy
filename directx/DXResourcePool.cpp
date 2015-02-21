#include "DXResourcePool.h"


DXResourcePool::DXResourcePool() : active_model_generator(0)
{
}


DXResourcePool::~DXResourcePool()
{
}


void DXResourcePool::Initialize(ID3D11Device* dev, ID3D11DeviceContext* dev_con) {
	device = dev;
	device_context = dev_con;
}

void DXResourcePool::BeginNewModel(int vertex_size) {
	ModelGenerator new_generator(vertex_size);
	active_model_generator = new_generator;
}

void DXResourcePool::AddModelVertex(void* new_vertex) {
	active_model_generator.AddVertex(new_vertex);
}

int DXResourcePool::FinishModel() {
	Model* model = new Model;
	*model = active_model_generator.DumpModel(device, device_context);
	int* shader_number = new int;
	std::vector<std::pair<ConstantBuffer*, int>> transformations;
	ConstantBuffer* model_transformation = new ConstantBuffer;
	ModeledDrawHandler* draw_handler = new ModeledDrawHandler;

	*shader_number = 1;

	model_transformation->Initialize(device, device_context);
	XMStoreFloat4x4(&(model_transformation->GetBufferData().transformation), DirectX::XMMatrixTranslation(0, 0, 0));
	model_transformation->CreateBuffer();
	transformations.push_back(std::make_pair(model_transformation, 1));

	draw_handler->Initialize(shader_number, transformations, model);
	Entity new_entity;
	new_entity.Initialize(draw_handler);

	entities.push_back(new_entity);

	ModelGenerator inactive_generator(0);
	active_model_generator = inactive_generator;
	return entities.size() - 1;
}

int DXResourcePool::LoadTexture(std::string file_name) {
	return 0;
}

int DXResourcePool::GetNumberOfEntities() {
	return entities.size();
}

Entity* DXResourcePool::GetEntity(int entity_index) {
	return &entities[entity_index];
}

Texture* DXResourcePool::GetTexture(int texture_index) {
	return &textures[texture_index];
}
