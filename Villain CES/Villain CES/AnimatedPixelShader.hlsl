/////////////
// GLOBALS //
/////////////
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1

#define MAX_LIGHTS 11
Texture2D g_d3dDiffuseTexture : register(t0);
Texture2D g_d3dEmissiveTexture : register(t1);
Texture2D g_d3dSpecularTexture : register(t2);

SamplerState g_SampleType : register(s0);

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
    LIGHT worldLights[MAX_LIGHTS];
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
    float specularRatio = 0;
	totalAmbient = ambience;

	float4 d3dDiffuseColor = g_d3dDiffuseTexture.Sample(g_SampleType, (float2) tInput.d3dTexture);
     totalEmissive = g_d3dEmissiveTexture.Sample(g_SampleType, (float2) tInput.d3dTexture);
    totalSpecular = g_d3dSpecularTexture.Sample(g_SampleType, (float2) tInput.d3dTexture);
    for (int lightIndex = 0; lightIndex < MAX_LIGHTS; lightIndex++)
    {
        if(worldLights[lightIndex].enabled)
        {
            if(worldLights[lightIndex].lightType == DIRECTIONAL_LIGHT)
            {
                float3 currentLightDir = normalize(worldLights[lightIndex].lightDirection.xyz);
                float lightRatio = saturate(dot(-currentLightDir, tInput.normal));
               // lightRatio = saturate(lightRatio);
                totalDiffuse += lightRatio * worldLights[lightIndex].lightColor;
              float3 viewDirection = -normalize(lightEyePos.xyz);
                float3 reflect = currentLightDir - 2 * tInput.normal * dot(currentLightDir, tInput.normal);
                specularRatio = saturate(dot(reflect, viewDirection));

                if (mProperties.shininess.x != 0)
                {
                    totalSpecular += lightRatio * worldLights[lightIndex].lightColor * pow(specularRatio, mProperties.shininess.x);
                }
                else
                {
                    totalSpecular += lightRatio * worldLights[lightIndex].lightColor * pow(specularRatio, 1);
                }

            }
            else if (worldLights[lightIndex].lightType == POINT_LIGHT)
            {
                float3 pointLight = normalize((worldLights[lightIndex].lightPosition - tInput.worldPos)).xyz;
                float lightRatio = saturate(dot(pointLight, tInput.normal));
                float lightRadius = 15.0f;
                float attentuation = 1.0f - saturate(length(worldLights[lightIndex].lightPosition.xyz - tInput.worldPos.xyz) / lightRadius);
                //This will make the attentuation fall off faster
                attentuation *= attentuation;
                lightRatio = saturate((lightRatio * attentuation));

                totalDiffuse += lightRatio * worldLights[lightIndex].lightColor;

                             float3 viewDirection = -normalize(lightEyePos.xyz);
                float3 currentLightDir = normalize(worldLights[lightIndex].lightDirection.xyz);
                float3 reflect = currentLightDir - 2 * tInput.normal * dot(currentLightDir, tInput.normal);

                specularRatio = saturate(dot(reflect, -viewDirection));
                float4 currentColor = worldLights[lightIndex].lightColor;
                if (mProperties.shininess.x != 0)
                {
                    totalSpecular += lightRatio * currentColor * pow(specularRatio, mProperties.shininess.x);
                }
                else
                {
                    totalSpecular += lightRatio * currentColor * pow(specularRatio, 1);

                }
            }

        }

    }
    totalDiffuse = float4(totalDiffuse.xyz + totalAmbient.xyz, 1);

    if (mProperties.diffuse.x != 0 || mProperties.diffuse.y != 0 || mProperties.diffuse.z != 0)
    {
        totalDiffuse = mProperties.diffuse * totalDiffuse;

    }
    if (mProperties.emissive.x != 0 || mProperties.emissive.y != 0 || mProperties.emissive.z != 0)
    {
        totalEmissive = totalEmissive * mProperties.emissive;
    }
    if (mProperties.specular.x != 0 || mProperties.specular.y != 0 || mProperties.specular.z != 0)
    {
        totalSpecular = totalSpecular * mProperties.specular;
    }

    d3dDiffuseColor = (totalDiffuse + totalEmissive + totalSpecular) * d3dDiffuseColor;
    d3dDiffuseColor = saturate(d3dDiffuseColor);
    d3dDiffuseColor.w = 1;
        return d3dDiffuseColor;
}