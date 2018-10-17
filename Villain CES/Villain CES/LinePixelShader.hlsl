/////////////
// GLOBALS //
/////////////
Texture2D g_d3dDiffuseTexture;
SamplerState g_SampleType;

struct TLinePixelInputType
{
	float4 d3dPos : SV_POSITION;
	float4 d3dColor : POSITION;
};


float4 main(TLinePixelInputType pixelO) : SV_TARGET
{
	float4 d3dDiffuseColor = g_d3dDiffuseTexture.Sample(g_SampleType, (float2) pixelO.d3dColor);

	float3 d3dAmbientLightColor;
	d3dAmbientLightColor.x = 0.0f;
	d3dAmbientLightColor.y = 0.0f;
	d3dAmbientLightColor.z = 0.0f;

	return float4(saturate(
		d3dAmbientLightColor.xyz +
		d3dDiffuseColor.xyz
	), 1);
	//return pixelO.d3dColor;
}