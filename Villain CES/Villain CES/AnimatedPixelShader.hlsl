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
struct PixelInputType
{
	float4 d3dPosition : SV_POSITION;
    float4 worldPos : POSITION;
    float3 normal : NORM;
	float2 d3dTexture : TEXCOORD;
};

struct LIGHT
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

	
};

cbuffer Light_Constant_Buffer : register(b0)
{
	MATERIAL mProperties;
    LIGHT worldLights;
    float4 ambience;
    float4 lightEyePos;
};
////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 AnimatedPixelShader(PixelInputType tInput) : SV_TARGET
{
    float4 totalEmissive = 0;
    float4 totalAmbient = 0;
    float4 totalDiffuse = 0;
    float4 totalSpecular = 0;
	totalAmbient = ambience;

	float4 d3dDiffuseColor = g_d3dDiffuseTexture.Sample(g_SampleType, (float2) tInput.d3dTexture);

    for (int lightIndex = 0; lightIndex < MAX_LIGHTS; lightIndex++)
    {
        if(worldLights.enabled)
        {
            if(worldLights.lightType == DIRECTIONAL_LIGHT)
            {
                float3 currentLightDir = normalize(worldLights.lightDirection.xyz);
                float lightRatio = -saturate(dot(currentLightDir, tInput.normal));
                lightRatio = saturate(lightRatio + totalAmbient);
                totalDiffuse += lightRatio * worldLights.lightColor;

            }
            else if(worldLights.lightType == POINT_LIGHT)
            {
                float3 pointLight = normalize((worldLights.lightPosition - tInput.worldPos)).xyz;
                float lightRatio = saturate(dot(pointLight, tInput.normal));
                float attentuation = 1.0f - saturate(length(worldLights.lightPosition.xyz - tInput.worldPos.xyz) /5.0f);
                //This will make the attentuation fall off faster
                //attentuation *= attentuation;
                lightRatio = saturate((lightRatio * attentuation));

                totalDiffuse += lightRatio * worldLights.lightColor;
				totalDiffuse = float4(totalDiffuse.xyz + totalAmbient.xyz, 1);
            }

        }

    }

    totalDiffuse = mProperties.diffuse * totalDiffuse;
   

    d3dDiffuseColor = totalDiffuse * d3dDiffuseColor;
    d3dDiffuseColor = saturate(d3dDiffuseColor);

        return d3dDiffuseColor;
}