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
};

VOut VShader(float4 position : POSITION, float4 color : COLOR)
{
    VOut output;

	output.position = mul(model, position);
	output.position = mul(view_projection, output.position);
	output.color = color;
	output.color.r = output.position.z/output.position.w;
	output.color.g = 0;
	output.color.b = 0;

    return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
    return color;
}
