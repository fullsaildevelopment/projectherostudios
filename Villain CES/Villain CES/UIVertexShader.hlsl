/////////////
// CBUFFER //
/////////////
#pragma pack_matrix( row_major )

cbuffer MatrixBuffer : register(b0)
{
	matrix d3dWorldMatrix;
	matrix d3dViewMatrix;
	matrix d3dProjectionMatrix;

	//float2 rcpDim;
	//float2 rcpDim2;
};

//////////////
// TYPEDEFS //
//////////////
struct TUIVertexInputType
{
	float3 d3dPosition : POSITION0;
	float2 d3dTexture : TEXCOORD;
};

struct TUIPixelInputType
{
	float4 d3dPosition : SV_POSITION;
	float2 d3dTexture : TEXCOORD;
	float4 d3dColor : POSITION1;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////

TUIPixelInputType UIVertexShader( TUIVertexInputType tInput )
{
	TUIPixelInputType tOutput;
	//tOutput.d3dPosition.xy = tInput.d3dPosition.xy * rcpDim;
	//tOutput.d3dPosition.zw = float2(0, 1);

	tOutput.d3dPosition = float4(tInput.d3dPosition.xyz, 1);
	tOutput.d3dTexture = tInput.d3dTexture;
	//tOutput.d3dPosition = mul(tOutput.d3dPosition, d3dWorldMatrix);
	//tOutput.d3dPosition = mul(tOutput.d3dPosition, d3dViewMatrix);
	//tOutput.d3dPosition = mul(tOutput.d3dPosition, d3dProjectionMatrix);
	tOutput.d3dColor = float4(1, 1, 1, 1);
	return tOutput;
}