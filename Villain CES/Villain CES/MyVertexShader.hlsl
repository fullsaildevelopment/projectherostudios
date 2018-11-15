/////////////
// CBUFFER //
/////////////
#pragma pack_matrix( row_major )

cbuffer MatrixBuffer : register(b0)
{
	matrix d3dWorldMatrix;
	matrix d3dViewMatrix;
	matrix d3dProjectionMatrix;

	float4 d3dColor;
};

//////////////
// TYPEDEFS //
//////////////
struct TVertexInputType
{
	float4 d3dPosition : POSITION0;
    float3 d3dNormal : NORM;
	float2 d3dUVs: TEXCOORD;
};

struct TPixelInputType
{
	float4 d3dPosition : SV_POSITION;
    float4 d3dWorldPos : POSITION0;
    float3 d3dNormal : NORM;
	float2 d3dUVs : TEXCOORD;
	float4 d3dColor : COLOR;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
TPixelInputType MyVertexShader(TVertexInputType tInput)
{
	TPixelInputType output;
    float3 testNormal = tInput.d3dNormal;

    //float3 Magnitude = length(tInput.d3dPosition);
    output.d3dNormal = tInput.d3dNormal;

	tInput.d3dPosition.w = 1;
	output.d3dPosition = mul(tInput.d3dPosition, d3dWorldMatrix);
    output.d3dWorldPos = output.d3dPosition;
    output.d3dNormal = mul(output.d3dNormal, (float3x3)d3dWorldMatrix);
    output.d3dNormal = normalize(output.d3dNormal);
	output.d3dPosition = mul(output.d3dPosition, d3dViewMatrix);
	output.d3dPosition = mul(output.d3dPosition, d3dProjectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.d3dUVs = tInput.d3dUVs;

	//output.d3dColor = d3dColor;
	
    if (d3dColor.x < 0)
    {
        output.d3dColor = float4(0, 0, 0, 0);
    }
    else
    {
        output.d3dColor = d3dColor;
    }

	return output;
}