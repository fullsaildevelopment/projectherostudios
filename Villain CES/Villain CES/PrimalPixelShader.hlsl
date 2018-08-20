/////////////
// GLOBALS //
/////////////

/////////////
// CBUFFER //
/////////////
cbuffer PixelBuffer : register(b0)
{
	float4 d3dCollisionColor;
}
//////////////
// TYPEDEFS //
//////////////
struct TPrimalPixelInputType
{
	float4 d3dPos : SV_POSITION;
	float4 d3dColor : POSITION1;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 PrimalPixelShader(TPrimalPixelInputType tInput) : SV_TARGET
{
	if (d3dCollisionColor.w)
	{
		return d3dCollisionColor;
	}
	else
	{
		return tInput.d3dColor;
	}
}