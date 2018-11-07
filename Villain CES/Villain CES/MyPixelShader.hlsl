/////////////
// GLOBALS //
/////////////
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1


#define MAX_LIGHTS 1

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

struct LIGHTS
{
    float4 lightDirection;
    float4 lightPosition;
    float4 lightColor;
    int lightType;
    int enabled;

    float2 padding;
};


struct MATERIAL
{
    float4 emissive;
    float4 diffuse;
    float4 specular;
    // X element holds actual shininess to save on padding that why its a float4 when it would only need a float or double
    float4 shininess;

	//int specularPower;

	//float3 padding;
};

cbuffer Light_Constant_Buffer : register(b0)
{
    MATERIAL mProperties;
    LIGHTS worldLights[MAX_LIGHTS];
    float4 ambience;
    float4 lightEyePos;
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 MyPixelShader(TPixelInputType tInput) : SV_TARGET
{

    float4 totalEmissive = 0;
    float4 totalAmbient = 0;
    float4 totalDiffuse = 0;
    float4 totalSpecular = 0;

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