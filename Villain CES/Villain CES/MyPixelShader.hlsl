/////////////
// GLOBALS //
/////////////
Texture2D g_d3dDiffuseTexture;
SamplerState g_SampleType;
//////////////
// TYPEDEFS //
//////////////
struct TPixelInputType
{
	float4 d3dPosition : SV_POSITION;
	float2 d3dTexture : TEXCOORD;

	float4 d3dColor : COLOR;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 MyPixelShader(TPixelInputType tInput) : SV_TARGET
{
	float4 d3dDiffuseColor = g_d3dDiffuseTexture.Sample(g_SampleType, (float2) tInput.d3dTexture);

	float3 d3dAmbientLightColor;
	d3dAmbientLightColor.x = 0.0f;
	d3dAmbientLightColor.y = 0.0f;
	d3dAmbientLightColor.z = 0.0f;

	if (tInput.d3dColor.x != 0)
	{
		d3dDiffuseColor = d3dDiffuseColor * tInput.d3dColor;
	}

	return float4(saturate(
		d3dAmbientLightColor.xyz +
		d3dDiffuseColor.xyz
	), 1);
}