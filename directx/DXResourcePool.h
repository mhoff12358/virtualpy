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

	virtual void BeginNewModel(unsigned int vertex_type);
	virtual void AddModelVertex(void* new_vertex);
	virtual int FinishModel();

	virtual int LoadTexture(std::string file_name);

	virtual int CreateModeledEntity(int model_id);
	virtual int CreateTexturedEntity(int model_id, int texture_id);

	virtual int GetNumberOfEntities();

	Entity* GetEntity(int entity_index);
	Model* GetModel(int model_index);
	Texture* GetTexture(int texture_index);

private:
	ID3D11Device* device;
	ID3D11DeviceContext* device_context;

	ModelGenerator* active_model_generator;
	unsigned int active_vertex_type;

	std::vector<Entity*> entities;
	std::vector<Model*> models;
	std::vector<Texture*> textures;
};
