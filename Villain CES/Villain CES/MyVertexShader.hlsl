/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	float4x4 joints[33];//large number
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
	float4 Position : POSITION0;
	float3 Normal : NORMAL;
	float2 tex : TEXCOORD;
	float4 weights : BLENDWEIGHT;
	uint4 index : BLENDINDICES;
};

struct PixelInputType
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 tex : TEXCOORD;
	float4 WorldPos : POSITION1;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType MyVertexShader(VertexInputType input)
{
	PixelInputType output;
	float4 skinned_pos = float4(0.0f,0.0f,0.0f,0.0f);
	//input.Position.w = 1;
	for (int i = 0; i < 4; ++i)
	{
		skinned_pos += mul(input.Position, joints[input.index[i]]) * input.weights[i];
	}
	//skinned_pos += mul(input.Position, joints[input.index[0]]);
	skinned_pos.w = 1;
	//	skinned_pos.z *= -1;
	// Calculate the position of the vertex against the world, view, and projection matrices.
	if (input.weights[0] > 0)
		output.Position = mul(skinned_pos, worldMatrix);
	else
	{
		input.Position.w = 1;
		output.Position = mul(input.Position, worldMatrix);
	}

	output.Position = mul(output.Position, viewMatrix);
	output.Position = mul(output.Position, projectionMatrix);

	output.Normal = input.Normal;

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Store the World Position
	output.WorldPos = mul(skinned_pos, worldMatrix);

	return output;
}
