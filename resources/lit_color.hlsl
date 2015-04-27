struct VOut
{
    float4 position : SV_POSITION;
	float4 color : COLOR;
};

cbuffer shared_matrices : register(b0)
{
	matrix <float, 4, 4> view_projection;
};

cbuffer personal_matrices : register(b1)
{
	matrix <float, 4, 4> model;
	matrix <float, 4, 4> model_inv_trans;
};

cbuffer per_batch : register(b2)
{
	float4 new_color;
};
cbuffer per_batch2 : register(b3)
{
	float4 light_dir;
};

VOut VShader(float4 position : POSITION, float3 normal : TEXCOORD)
{
    VOut output;

	output.position = mul(model, position);
	output.position = mul(view_projection, output.position);
	float light_scale = 0.2 + max(dot(normal, -light_dir.xyz), 0.0);
	output.color = float4(light_scale*new_color.xyz, 1.0);

    return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
    return color;
}
