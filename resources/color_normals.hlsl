struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
	float4 normal : NORMAL;
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

VOut VShader(float4 position : POSITION, float4 normal : NORMAL, float4 color : COLOR)
{
    VOut output;

	output.position = mul(model, position);
	output.position = mul(view_projection, output.position);
	output.normal = normal;
	output.color = color;

    return output;
}


float4 PShader(float4 position : SV_POSITION, float4 normal : NORMAL, float4 color : COLOR) : SV_TARGET
{
    return normal;
}
