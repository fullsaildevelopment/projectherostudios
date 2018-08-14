/////////////
// CBUFFER //
/////////////
#pragma pack_matrix( row_major )

cbuffer MatrixBuffer : register(b0)
{
	matrix d3dWorldMatrix;
	matrix d3dViewMatrix;
	matrix d3dProjectionMatrix;
};

//////////////
// TYPEDEFS //
//////////////
struct TVertexInputType
{
	float4 d3dPosition : POSITION0;
	float2 d3dUVs: TEXCOORD;
};

struct TPixelInputType
{
	float4 d3dPosition : SV_POSITION;
	float2 d3dUVs : TEXCOORD;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
TPixelInputType MyVertexShader(TVertexInputType tInput)
{
	TPixelInputType output;

	tInput.d3dPosition.w = 1;
	output.d3dPosition = mul(tInput.d3dPosition, d3dWorldMatrix);
	output.d3dPosition = mul(output.d3dPosition, d3dViewMatrix);
	output.d3dPosition = mul(output.d3dPosition, d3dProjectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.d3dUVs = tInput.d3dUVs;
	return output;
}