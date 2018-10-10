
#pragma pack_matrix( row_major )

cbuffer MatrixBuffer : register(b0)
{
	matrix d3dWorldMatrix;
	matrix d3dViewMatrix;
	matrix d3dProjectionMatrix;
};

struct TVertexInputType
{
	float3 d3dPosition : POSITION;
	float4 d3dColor: COLOR;
};

struct TPixelInputType
{
	float4 d3dPosition : SV_POSITION;
	float4 d3dColor: COLOR;
};

TPixelInputType LineVertexShader(TVertexInputType in_vertex)
{
	TPixelInputType line_Output = (TPixelInputType)0;
    line_Output.d3dPosition = float4(in_vertex.d3dPosition.xyz, 1);
	line_Output.d3dPosition = mul(line_Output.d3dPosition, d3dWorldMatrix);
	line_Output.d3dPosition = mul(line_Output.d3dPosition, d3dViewMatrix);
	line_Output.d3dPosition = mul(line_Output.d3dPosition, d3dProjectionMatrix);

	line_Output.d3dColor = in_vertex.d3dColor;

	return line_Output;



}