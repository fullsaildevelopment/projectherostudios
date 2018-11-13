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
    float4 d3dWorldPos : POSITION0;
    float3 d3dNormal : NORM;
    float2 d3dUVs : TEXCOORD;
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
    LIGHTS worldLights;
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
    float4 pointLightR, directionLightR;
    float4 totalSpecular = 0;
    float specularRatio = 0;
    totalAmbient = ambience;
	float4 d3dDiffuseColor = g_d3dDiffuseTexture.Sample(g_SampleType, (float2)tInput.d3dUVs);
      // totalEmissive = g_d3dEmissiveTexture.Sample(g_SampleType, (float2) tInput.d3dTexture);
    //totalSpecular = g_d3dSpecularTexture.Sample(g_SampleType, (float2) tInput.d3dTexture);

 
    //for (int lightIndex = 0; lightIndex < MAX_LIGHTS; lightIndex++)
    //{
        specularRatio = 0;
        directionLightR = pointLightR = 0;
    directionLightR = 0;
        if (worldLights.enabled)
        {
            if (worldLights.lightType == DIRECTIONAL_LIGHT)
            {
                float3 currentLightDir = normalize(-worldLights.lightDirection.xyz);
                float lightRatio = -saturate(dot(currentLightDir, tInput.d3dNormal));
               // lightRatio = saturate(lightRatio + totalAmbient.y); // add ambience
            directionLightR = lerp(0, worldLights.lightColor, lightRatio);
            totalDiffuse += directionLightR;
              //float3 viewDirection = -normalize(lightEyePos.xyz);
              //float3 reflect = -currentLightDir - 2 * tInput.d3dNormal * dot(-currentLightDir, tInput.d3dNormal);
              // specularRatio = saturate(dot(reflect, viewDirection));
              //   totalSpecular += worldLights.lightColor * pow(specularRatio, mProperties.shininess.x);

            }
            else if (worldLights.lightType == POINT_LIGHT)
            {
                float3 pointLight = normalize((worldLights.lightPosition - tInput.d3dWorldPos)).xyz;
                float lightRatio = saturate(dot(pointLight, tInput.d3dNormal));
               // lightRatio = saturate(lightRatio + totalAmbient.y); // add ambience commented out so you can add at the end before return 
            float lightRadius = 5.0f;
                float attentuation = 1.0f - saturate(length(worldLights.lightPosition - tInput.d3dWorldPos) / lightRadius);
                //This will make the attentuation fall off faster
                attentuation *= attentuation;
                lightRatio = saturate((lightRatio * attentuation));
                pointLightR = lightRatio * worldLights.lightColor;
           // pointLightR = lerp(0, worldLights.lightColor, lightRatio);
            //pointLightR = saturate((pointLightR * attentuation));
             totalDiffuse += pointLightR;
                   //float3 viewDirection = -normalize(lightEyePos.xyz);
                   //float3 currentLightDir = normalize(worldLights.lightDirection.xyz);
                   //float3 reflect = currentLightDir - 2 * tInput.d3dNormal * dot(currentLightDir, tInput.d3dNormal);

                 //specularRatio = saturate(dot(reflect, -viewDirection));
                 // float4 currentColor = worldLights.lightColor;
                 //totalSpecular += lightRatio * currentColor * pow(specularRatio, mProperties.shininess.x);
            }
        }
    //}

    totalDiffuse = mProperties.diffuse * totalDiffuse;
   

    d3dDiffuseColor = (float4(totalAmbient.xyz + totalDiffuse.xyz, 1)) * d3dDiffuseColor;
    d3dDiffuseColor = saturate(d3dDiffuseColor);
     //   totalEmissive = totalEmissive * mProperties.emissive;
     //   totalSpecular = totalSpecular * mProperties.specular;
    d3dDiffuseColor.w = 1;
    return d3dDiffuseColor;

}
	
    //PUT UNDER D3D DIFFUSE COLOR VAR SIMPLE TEXTURE WITH NO LIGHTING
   //   float3 d3dAmbientLightColor;
 //   d3dAmbientLightColor.x = 0.0f;
 //   d3dAmbientLightColor.y = 0.0f;
 //   d3dAmbientLightColor.z = 0.0f;
 //   if (tInput.d3dColor.x != 0)
 //   {
 //       d3dDiffuseColor = d3dDiffuseColor * tInput.d3dColor;
 //   }
 //   return float4(saturate(
	//	d3dAmbientLightColor.xyz +
	//	d3dDiffuseColor.xyz
	//), 1);