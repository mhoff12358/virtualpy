#pragma once

#include <string>

class ResourcePool
{
public:
	ResourcePool();
	~ResourcePool();

	void Initialize();

	virtual void BeginNewModel(unsigned int vertex_type) = 0;
	virtual void AddModelVertex(void* new_vertex) = 0;
	virtual int FinishModel() = 0;

	virtual int LoadTexture(std::string file_name) = 0;

	virtual int CreateModeledEntity(int model_id) = 0;
	virtual int CreateTexturedEntity(int model_id, int texture_id) = 0;

	virtual int GetNumberOfEntities() = 0;

private:

};

