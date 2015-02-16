#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

#include "../ResourcePool.h"

#include "Model.h"
#include "Entity.h"

class DXResourcePool :
	public ResourcePool
{
public:
	DXResourcePool();
	~DXResourcePool();

	void Initialize(ID3D11Device* dev, ID3D11DeviceContext* dev_con);

	virtual void BeginNewModel(int vertex_size);
	virtual void AddModelVertex(void* new_vertex);
	virtual int FinishModel();

	virtual int GetNumberOfEntities();

	Entity* GetEntity(int entity_index);

private:
	ID3D11Device* device;
	ID3D11DeviceContext* device_context;

	ModelGenerator active_model_generator;

	std::vector<Entity> entities;
};
