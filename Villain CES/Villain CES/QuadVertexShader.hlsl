//////////////
// TYPEDEFS //
//////////////
struct TQuadVertexInputType
{
	float3 d3dPosition : POSITION0;
	float4 d3dColor : POSITION1;
};

struct TQuadGeoInputType
{
	float4 d3dPosition : SV_POSITION;
	float4 d3dColor : POSITION1;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
TQuadGeoInputType QuadVertexShader(TQuadVertexInputType tInput)
{
	TQuadGeoInputType tOutput;
	tOutput.d3dPosition = float4(tInput.d3dPosition.xyz, 1);
	tOutput.d3dColor = tInput.d3dColor;
	return tOutput;
}