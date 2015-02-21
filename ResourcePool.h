#pragma once

#include <string>

class ResourcePool
{
public:
	ResourcePool();
	~ResourcePool();

	void Initialize();

	virtual void BeginNewModel(int vertex_size) = 0;
	virtual void AddModelVertex(void* new_vertex) = 0;
	virtual int FinishModel() = 0;

	virtual int LoadTexture(std::string file_name) = 0;

	virtual int GetNumberOfEntities() = 0;

private:

};

