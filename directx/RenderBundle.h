#ifndef __RENDER_BUNDLE_H_INCLUDED__
#define __RENDER_BUNDLE_H_INCLUDED__

#include <vector>

#include "ConstantBuffer.h"

class RenderBundle {
public:
	RenderBundle();
	RenderBundle(ID3D11Device* dev, ID3D11DeviceContext* dev_con, int num_constant_buffers, std::vector<char> pipeline_stages);

	const std::vector<ConstantBuffer*>& GetBuffersForReference();
	std::vector<ConstantBuffer*>& GetBuffersForEdit();

private:
	std::vector<ConstantBuffer*> buffers;
};

#endif