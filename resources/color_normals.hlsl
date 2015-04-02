struct VOut
{
	float4 position : SV_POSITION;
	float4 normal : TEXCOORD;
    float4 color : COLOR;
};

cbuffer shared_matrices : register(b0)
{
	matrix <float, 4, 4> view_projection;
};

cbuffer shared_matrices : register(b1)
{
	matrix <float, 4, 4> view_projection_inv_trans;
};

cbuffer personal_matrices : register(b2)
{
	matrix <float, 4, 4> model;
};

cbuffer personal_matrices_inv_trans : register(b3)
{
	matrix <float, 4, 4> model_inv_trans;
};

VOut VShader(float3 position : POSITION, float3 normal : NORMAL, float4 color : COLOR)
{
    VOut output;

	output.position = mul(model, float4(position.xyz, 1.0));
	output.position = mul(view_projection, output.position);
	output.normal = mul(model_inv_trans, float4(normal, 0.0));
	output.color = color;

    return output;
}


float4 PShader(float4 position : SV_POSITION, float4 normal : TEXCOORD, float4 color : COLOR) : SV_TARGET
{
	return float4(normal.xyz, 0.0);
}
