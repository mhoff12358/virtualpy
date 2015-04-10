#include "RenderBundle.h"

RenderBundle::RenderBundle() {

}

const std::vector<ConstantBuffer*>& RenderBundle::GetBuffersForReference() {
	return buffers;
}

std::vector<ConstantBuffer*>& RenderBundle::GetBuffersForEdit() {
	return buffers;
}