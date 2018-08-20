/////////////
// GLOBALS //
/////////////
Texture2D g_d3dDiffuseTexture;
SamplerState g_SampleType;

/////////////
// CBUFFER //
/////////////
cbuffer PixelBuffer : register(b0)
{
	float4 hoverColor;
}

//////////////
// TYPEDEFS //
//////////////
struct TUIPixelInputType
{
	float4 d3dPos : SV_POSITION;
	float2 d3dTexture : TEXCOORD;
	float4 d3dColor : POSITION1;
};

float4 UIPixelShader(TUIPixelInputType tInput) : SV_TARGET
{
	float4 temp = g_d3dDiffuseTexture.Sample(g_SampleType, tInput.d3dTexture);

	temp += hoverColor;

	return temp;
}