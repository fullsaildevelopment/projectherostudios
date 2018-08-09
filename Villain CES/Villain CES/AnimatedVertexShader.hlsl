/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	float4x4 joints[28];//large number
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
	float4 Position : POSITION0;
	float2 tex : TEXCOORD;
	float4 weights : BLENDWEIGHT;
	uint4 index : BLENDINDICES;
};

struct PixelInputType
{
	float4 Position : SV_POSITION;
	float2 tex : TEXCOORD;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType AnimatedVertexShader(VertexInputType input)
{
	PixelInputType output;
	
	float4 skinned_pos = float4(0.0f, 0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 4; ++i)
	{
		skinned_pos += mul(input.Position, joints[input.index[i]]) * input.weights[i];
	}
	skinned_pos.w = 1;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	if (input.weights[0])
		output.Position = mul(skinned_pos, worldMatrix);
	else
		output.Position = mul(input.Position, worldMatrix);

	output.Position = mul(output.Position, viewMatrix);
	output.Position = mul(output.Position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;
	return output;
}
