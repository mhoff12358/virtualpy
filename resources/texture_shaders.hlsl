struct VOut
{
	float4 position : SV_POSITION;
	float2 tex_coords : TEXCOORD;
};

Texture2D model_skin : register(t0);
sampler skin_sampler : register(s0);

cbuffer shared_matrices : register(b0)
{
	matrix <float, 4, 4> view_projection;
};

cbuffer personal_matrices : register(b2)
{
	matrix <float, 4, 4> model;
};

VOut VShader(float4 position : POSITION, float2 tex_coord : TEXCOORD)
{
	VOut output;

	output.position = mul(model, position);
	output.position = mul(view_projection, output.position);
	output.tex_coords = tex_coord;

	return output;
}


float4 PShader(float4 position : SV_POSITION, float2 tex_coords : TEXCOORD) : SV_TARGET
{
	return model_skin.Sample(skin_sampler, tex_coords);
	//return float4(tex_coords.x, tex_coords.y, 0.0f, 1.0f);
}
