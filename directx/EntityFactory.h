#pragma once

#include <string>

#include "Entity.h"
class EntityFactory {
public:
	static EntityFactory& GetInstance();
	void Initialize(ID3D11Device* dev, ID3D11DeviceContext* dev_con);

	Entity* LoadEntityFromFile(std::string filename);

private:
	ID3D11Device* device;
	ID3D11DeviceContext* device_context;

	Entity* GetNewUnitializedEntity();

	EntityFactory() {}
	EntityFactory(EntityFactory const&);
	void operator=(EntityFactory const&);
};

