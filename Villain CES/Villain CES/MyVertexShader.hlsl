/////////////
// CBUFFER //
/////////////
cbuffer MatrixBuffer : register(b0)
{
	matrix d3dWorldMatrix;
	matrix d3dViewMatrix;
	matrix d3dProjectionMatrix;
	float4x4 ad3dJoints[33];//large number
};

//////////////
// TYPEDEFS //
//////////////
struct TVertexInputType
{
	float4 d3dPosition : POSITION0;
	float3 d3dNormal : NORMAL;
	float2 d3dTexture : TEXCOORD;
	float4 d3dWeights : BLENDWEIGHT;
	uint4 d3dIndex : BLENDINDICES;
};

struct TPixelInputType
{
	float4 d3dPosition : SV_POSITION;
	float3 d3dNormal : NORMAL;
	float2 d3dTexture : TEXCOORD;
	float4 d3dWorldPosition : POSITION1;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
TPixelInputType MyVertexShader(TVertexInputType tInput)
{
	TPixelInputType output;
	float4 d3dSkinnedPosition = float4(0.0f,0.0f,0.0f,0.0f);
	//tInput.Position.w = 1;
	for (int nSkinnedPositionIndex = 0; nSkinnedPositionIndex < 4; ++nSkinnedPositionIndex)
	{
		d3dSkinnedPosition += mul(tInput.d3dPosition, ad3dJoints[tInput.d3dIndex[nSkinnedPositionIndex]]) * tInput.d3dWeights[nSkinnedPositionIndex];
	}
	d3dSkinnedPosition.w = 1;

	if (tInput.d3dWeights[0] > 0)
		output.d3dPosition = mul(d3dSkinnedPosition, d3dWorldMatrix);
	else
	{
		tInput.d3dPosition.w = 1;
		output.d3dPosition = mul(tInput.d3dPosition, d3dWorldMatrix);
	}

	output.d3dPosition = mul(output.d3dPosition, d3dViewMatrix);
	output.d3dPosition = mul(output.d3dPosition, d3dProjectionMatrix);

	output.d3dNormal = tInput.d3dNormal;

	// Store the texture coordinates for the pixel shader.
	output.d3dTexture = tInput.d3dTexture;

	// Store the World Position
	output.d3dWorldPosition = mul(d3dSkinnedPosition, d3dWorldMatrix);

	return output;
}
