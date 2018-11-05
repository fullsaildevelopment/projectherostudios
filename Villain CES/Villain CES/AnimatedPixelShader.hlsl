/////////////
// GLOBALS //
/////////////
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1

#define MAX_LIGHTS 3
Texture2D g_d3dDiffuseTexture;
SamplerState g_SampleType;

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
	float4 d3dPosition : SV_POSITION;
    float4 worldPos : POSITION;
    float3 normal : NORM;
	float2 d3dTexture : TEXCOORD;
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
float4 AnimatedPixelShader(PixelInputType tInput) : SV_TARGET
{
    float4 totalEmissive = 0;
    float4 totalAmbient = 0;
    float4 totalDiffuse = 0;
    float4 totalSpecular = 0;


	float4 d3dDiffuseColor = g_d3dDiffuseTexture.Sample(g_SampleType, (float2) tInput.d3dTexture);

    for (int lightIndex = 0; lightIndex < MAX_LIGHTS; lightIndex++)
    {
        if(worldLights[lightIndex].enabled)
        {
            if(worldLights[lightIndex].lightType == DIRECTIONAL_LIGHT)
            {
                float3 currentLightDir = normalize(worldLights[lightIndex].lightDirection.xyz);
                float lightRatio = -saturate(dot(currentLightDir, tInput.normal));
                lightRatio = saturate(lightRatio + ambience);
                totalDiffuse += lightRatio * worldLights[lightIndex].lightColor;

            }
            else if(worldLights[lightIndex].lightType == POINT_LIGHT)
            {
                float3 pointLight = normalize(worldLights[lightIndex].lightPosition - tInput.worldPos).xyz;
                float lightRatio = saturate(dot(pointLight, tInput.normal));
                float attentuation = 1.0f - saturate(length(worldLights[lightIndex].lightPosition.xyz - tInput.worldPos.xyz) / 7.5f);
                //This will make the attentuation fall off faster
                //attentuation *= attentuation;
                lightRatio = saturate(lightRatio * attentuation);

                totalDiffuse += lightRatio * worldLights[lightIndex].lightColor;

            }

        }

    }

    totalDiffuse = mProperties.diffuse * totalDiffuse;
    totalAmbient = ambience;

    d3dDiffuseColor = (totalDiffuse + totalAmbient) * d3dDiffuseColor;
    d3dDiffuseColor = saturate(d3dDiffuseColor);

        return d3dDiffuseColor;
}