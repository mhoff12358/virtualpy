#pragma once

#include "ResourcePool.h"
#include "VRBackend/EntityHandler.h"
#include "VRBackend/ResourcePool.h"

namespace virtualpy {

	class VRBackendResourcePool : public ResourcePool
	{
	public:
		VRBackendResourcePool();
		~VRBackendResourcePool();

		virtual void Initialize(::ResourcePool* vr_backend_resource_pool, EntityHandler* vr_backend_entity_handler);

		virtual void BeginNewModel(PyObject* vertex_type);
		virtual void BeginNewModel(PyObject* vertex_type, PyObject* primitive_type);
		void BeginNewModel(VertexType vertex_type, D3D_PRIMITIVE_TOPOLOGY primitive_type);
		virtual void AddModelVertex(PyObject* new_vertex);
		virtual void AddModelVertexBatch(PyObject* new_vertexes);
		virtual void AddModelVertex(Vertex new_vertex);
		virtual int FinishModel();
		virtual int LoadModelFromFile(std::string file_name);

		virtual int LoadVertexShader(std::string file_name, PyObject* vertex_type);
		virtual int LoadPixelShader(std::string file_name);

		virtual int CreateObjectSettings();

		virtual int CreateModeledEntity(int model_id, int vertex_shader_id, int pixel_shader_id, int object_setting_id);
//		virtual int CreateTexturedEntity(int model_id, int shader_id, int texture_id);

		virtual void CreateRenderBundle(int render_bundle_id, int num_constant_buffers, std::vector<char> pipeline_stages);

		DXGI_FORMAT PyRawTypeToDXGIFormat(PyObject* raw_type);
		char* PyMetaTypeToSemantic(PyObject* meta_type);
		VertexType PyVertexTypeToVertexType(PyObject* vertex_type);
		Vertex PyVertexToVertex(PyObject* vertex);
		D3D_PRIMITIVE_TOPOLOGY PyPrimitiveTypeToD3DPrimitiveTopology(PyObject* primitive_type);

	private:
		// The underlying resource pool for the actualy graphics library
		::ResourcePool* backend_resource_pool;
		EntityHandler* backend_entity_handler;

		// Helper attributes for this object's higher level functionality
		std::vector<Vertex> temp_model_vertices;
		VertexType temp_model_vertex_type;
		D3D_PRIMITIVE_TOPOLOGY temp_model_topology;

		std::vector<Model> model_store;
		std::vector<PixelShader> pixel_shader_store;
		std::vector<VertexShader> vertex_shader_store;
		std::vector<ConstantBuffer*> object_setting_store;
	};

} // virtualpy