/////////////
// GLOBALS //
/////////////
Texture2D g_d3dDiffuseTexture;
SamplerState g_SampleType;

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
	float4 d3dPosition : SV_POSITION;
	float2 d3dTexture : TEXCOORD;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 AnimatedPixelShader(PixelInputType tInput) : SV_TARGET
{
	float4 d3dDiffuseColor = g_d3dDiffuseTexture.Sample(g_SampleType, (float2) tInput.d3dTexture);

	return d3dDiffuseColor;
}