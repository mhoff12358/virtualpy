#pragma once
#include "Python.h"

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

#include <unordered_map>

#include "../ResourcePool.h"

#include "Model.h"
#include "Entity.h"
#include "Vertices.h"
#include "RenderBundle.h"

#define FIRST_PRIVATE_BUFFER 2

class DXResourcePool :
	public ResourcePool
{
public:
	DXResourcePool();
	~DXResourcePool();

	void Initialize(ID3D11Device* dev, ID3D11DeviceContext* dev_con);

	virtual void BeginNewModel(PyObject* vertex_type);
	virtual void BeginNewModel(VertexType vertex_type);
	virtual void AddModelVertex(PyObject* new_vertex);
	virtual void AddModelVertex(Vertex new_vertex);
	virtual int FinishModel();

	virtual int LoadTexture(std::string file_name);
	virtual int LoadShader(std::string file_name, PyObject* vertex_type);
	virtual int LoadShader(std::string file_name, VertexType vertex_type);

	virtual int CreateModeledEntity(int model_id, int shader_id);
	virtual int CreateTexturedEntity(int model_id, int shader_id, int texture_id);

	virtual void CreateRenderBundle(int render_bundle_id, int num_constant_buffers);

	virtual int GetNumberOfEntities();

	Entity* GetEntity(int entity_index);
	Model* GetModel(int model_index);
	Texture* GetTexture(int texture_index);

	RenderBundle* GetRenderBundle(int render_bundle_id);

private:
	ID3D11Device* device;
	ID3D11DeviceContext* device_context;

	ModelGenerator* active_model_generator;
	unsigned int active_vertex_type;

	std::vector<Entity*> entities;
	std::vector<Model*> models;
	std::vector<Texture*> textures;
	std::vector<ShaderPipeline*> shaders;

	std::unordered_map<int, RenderBundle*> render_bundles;

	DXGI_FORMAT PyRawTypeToDXGIFormat(PyObject* raw_type);
	char* PyMetaTypeToSemantic(PyObject* meta_type);
	VertexType PyVertexTypeToVertexType(PyObject* vertex_type);
	Vertex PyVertexToVertex(PyObject* vertex);

	void AddModelTransformations(std::vector<std::pair<ConstantBuffer*, int>>& transformations);
};
