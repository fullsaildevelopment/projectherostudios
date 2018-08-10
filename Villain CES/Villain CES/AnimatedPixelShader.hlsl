/////////////
// GLOBALS //
/////////////
Texture2D diffuseTexture;
SamplerState SampleType;

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
	float4 Position : SV_POSITION;
	float2 tex : TEXCOORD;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 MyPixelShader(PixelInputType input) : SV_TARGET
{
	float4 d3dDiffuseColor = diffuseTexture.Sample(g_SampleType, (float2) tInput.d3dTexture);

	float3 d3dAmbientLightColor;
	d3dAmbientLightColor.x = 0.0f;
	d3dAmbientLightColor.y = 0.0f;
	d3dAmbientLightColor.z = 0.0f;

	return float4(saturate(
		d3dAmbientLightColor.xyz +
		d3dDiffuseColor.xyz
	), 1);
}