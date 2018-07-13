/////////////
// GLOBALS //
/////////////

/////////////
// CBUFFER //
/////////////

//////////////
// TYPEDEFS //
//////////////
struct TPrimalPixelInputType
{
	float4 d3dColor : SV_POSITION;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 PrimalPixelShader(TPrimalPixelInputType tInput) : SV_TARGET
{
	return tInput.d3dColor;
}