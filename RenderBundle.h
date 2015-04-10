#ifndef __RENDER_BUNDLE_H_INCLUDED__
#define __RENDER_BUNDLE_H_INCLUDED__

#include <vector>

#include "ConstantBuffer.h"

class RenderBundle {
public:


private:
	std::vector<ConstantBuffer*> buffers;
};

#endif