#include "EntityFactory.h"

void EntityFactory::Initialize(ID3D11Device* dev, ID3D11DeviceContext* dev_con) {
	device = dev;
	device_context = dev_con;
}

EntityFactory& EntityFactory::GetInstance() {
	static EntityFactory instance;
	return instance;
}

Entity* EntityFactory::LoadEntityFromFile(std::string file_name) {
	Entity* new_entity = GetNewUnitializedEntity();

	/*LuaModelLoader& model_loader = LuaModelLoader::GetInstance();
	Model* model = new Model;
	*model = model_loader.LoadModelFromFile(file_name);
	int* shader_number = new int;
	*shader_number = 1;
	std::vector<std::pair<ConstantBuffer*, int>> constant_buffers;
	constant_buffers.push_back(std::make_pair(new ConstantBuffer, 1));
	constant_buffers.at(0).first->Initialize(device, device_context);
	XMStoreFloat4x4(&(constant_buffers.at(0).first->GetBufferData().transformation), DirectX::XMMatrixTranslation(0, 1, 0));
	constant_buffers.at(0).first->CreateBuffer();
	ModeledDrawHandler* draw_handler = new ModeledDrawHandler;
	draw_handler->Initialize(shader_number, constant_buffers, model);
	new_entity->Initialize(draw_handler);*/

	return new_entity;
}

Entity* EntityFactory::GetNewUnitializedEntity() {
	return new Entity;
}