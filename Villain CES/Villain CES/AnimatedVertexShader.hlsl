/////////////
// GLOBALS //
/////////////
#pragma pack_matrix( row_major )

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	float4x4 joints[62];//large number
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
	float4 Position : POSITION0;
    float3 normal : NORM;
	float2 tex : TEXCOORD;
	float4 weights : BLENDWEIGHT;
	uint4 index : BLENDINDICES;
};

struct PixelInputType
{
	float4 Position : SV_POSITION;
    float4 worldPos : POSITION;
    float3 normal : NORM;
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
		skinned_pos += mul(float4(input.Position.xyz, 1), joints[input.index[i]]) * input.weights[i];
	}
	skinned_pos.w = 1;
    float3 Magnitude = length(skinned_pos);
    output.normal = normalize(skinned_pos.xyz / Magnitude);
	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.Position = mul(skinned_pos, worldMatrix);
    output.worldPos = output.Position;
    output.normal = mul(output.normal, (float3x3)worldMatrix);
   // output.normal = normalize(output.normal);
	output.Position = mul(output.Position, viewMatrix);
	output.Position = mul(output.Position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;
	return output;
}