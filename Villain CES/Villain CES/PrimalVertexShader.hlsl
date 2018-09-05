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
struct TPrimalVertexInputType
{
	float3 d3dPosition : POSITION0;
	float4 d3dColor : POSITION1;
};

struct TPrimalPixelInputType
{
	float4 d3dPosition : SV_POSITION;
	float4 d3dColor : POSITION1;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
TPrimalPixelInputType PrimalVertexShader(TPrimalVertexInputType tInput)
{
	TPrimalPixelInputType tOutput;
	tOutput.d3dPosition = float4(tInput.d3dPosition.xyz, 1);
	tOutput.d3dPosition = mul(tOutput.d3dPosition, d3dWorldMatrix);
	tOutput.d3dPosition = mul(tOutput.d3dPosition, d3dViewMatrix);
	tOutput.d3dPosition = mul(tOutput.d3dPosition, d3dProjectionMatrix);
	tOutput.d3dColor = tInput.d3dColor;
	return tOutput;
}