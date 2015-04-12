#include "RenderMode.h"

void RenderMode::Initialize(ID3D11Device* dev, ID3D11DeviceContext* dev_con, ID3D11RenderTargetView* rend_tar_view, ID3D11DepthStencilView* dep_sten_view) {
	device = dev;
	device_context = dev_con;
	render_target_view = rend_tar_view;
	depth_stencil_view = dep_sten_view;
}

void RenderMode::Initialize(ID3D11Device* dev, ID3D11DeviceContext* dev_con, ID3D11RenderTargetView* rend_tar_view, ID3D11DepthStencilView* dep_sten_view, D3D11_VIEWPORT view) {
	Initialize(dev, dev_con, rend_tar_view, dep_sten_view);
	SetViewport(view);
}

void RenderMode::SetViewport(D3D11_VIEWPORT view) {
	viewport = view;
}

void RenderMode::SetViewport(std::array<int, 2> size, std::array<float, 2> depth_range) {
	SetViewport(size[0], size[1], depth_range[0], depth_range[1]);
}

void RenderMode::SetViewport(std::array<int, 2> offset, std::array<int, 2> size, std::array<float, 2> depth_range) {
	SetViewport(offset[0], offset[1], size[0], size[1], depth_range[0], depth_range[1]);
}

void RenderMode::SetViewport(int width, int height, float min_depth, float max_depth) {
	SetViewport(0, 0, width, height, min_depth, max_depth);
}

void RenderMode::SetViewport(int xoff, int yoff, int width, int height, float min_depth, float max_depth) {
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = xoff; viewport.TopLeftY = yoff;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = min_depth;
	viewport.MaxDepth = max_depth;
}

void RenderMode::Cleanup() {
}

void RenderMode::Prepare() {
	device_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);
	//device_context->OMSetRenderTargets(1, &render_target_view, NULL);
	device_context->RSSetViewports(1, &viewport);
}

void RenderMode::PrepareShader(ShaderPipeline* shader) {
	device_context->VSSetShader(shader->vertex_shader, 0, 0);
	device_context->PSSetShader(shader->pixel_shader, 0, 0);

	device_context->IASetInputLayout(shader->input_layout);
}

void RenderMode::PrepareConstantBuffer(ConstantBuffer* constant_buffer, int buffer_slot) {
	constant_buffer->ActivateBuffer(buffer_slot);
}

void RenderMode::Clear(D3DXCOLOR color) {
	device_context->ClearRenderTargetView(render_target_view, color);
}

void RenderMode::RenderModel(const Model& model) const {
	ID3D11Buffer* vertex_buffer = model.GetVertexBuffer();
	device_context->IASetVertexBuffers(0, 1, &vertex_buffer, &model.stride, &model.offset);

	device_context->IASetPrimitiveTopology(model.primitive_type);

	device_context->Draw(model.num_vertices, 0);
}