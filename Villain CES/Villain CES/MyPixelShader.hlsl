/////////////
// GLOBALS //
/////////////
Texture2D diffuseTexture;
Texture2D specularTexture;
Texture2D emissiveTexture;
SamplerState SampleType;

cbuffer BlinnPhongBuffer
{
	float3 LightPosition;
	float3 CameraPos;
	float Shininess;
};

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 tex : TEXCOORD;
	float4 WorldPos : POSITION1;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 MyPixelShader(PixelInputType input) : SV_TARGET
{
	if ((CameraPos[0] || CameraPos[1] || CameraPos[2]))
	{
		// Phong relfection is ambient + light-diffuse + spec highlights.
		// I = Ia*ka*Oda + fatt*Ip[kd*Od(N.L) + ks(R.V)^n]
		//
		float4 diffuseColor = diffuseTexture.Sample(SampleType, (float2) input.tex);
		float4 specularColor = specularTexture.Sample(SampleType, (float2) input.tex);
		float4 emissiveColor = emissiveTexture.Sample(SampleType, (float2) input.tex);
		float xTwoMinusxOne = (input.WorldPos[0] - LightPosition[0]);
		float yTwoMinusyOne = (input.WorldPos[1] - LightPosition[1]);
		float zTwoMinuszOne = (input.WorldPos[2] - LightPosition[2]);
		xTwoMinusxOne *= xTwoMinusxOne;
		yTwoMinusyOne *= yTwoMinusyOne;
		zTwoMinuszOne *= zTwoMinuszOne;
		float dist = sqrt(xTwoMinusxOne + yTwoMinusyOne + zTwoMinuszOne);
		float LightDistanceSquared = pow(dist, 2);

		float3 AmbientLightColor;
		AmbientLightColor.x = 0.0f;
		AmbientLightColor.y = 0.0f;
		AmbientLightColor.z = 0.0f;

		float3 LightDiffuseColor;
		LightDiffuseColor.x = 1.0f;
		LightDiffuseColor.y = 1.0f;
		LightDiffuseColor.z = 1.0f;

		float3 LightSpecularColor;
		LightSpecularColor.x = 1.0f;
		LightSpecularColor.y = 1.0f;
		LightSpecularColor.z = 1.0f;

		/*
		Specular Component
		VIEWDIR = NORMALIZE( CAMWORLDPOS – SURFACEPOS )
		HALFVECTOR = NORMALIZE(( -LIGHTDIR ) + VIEWDIR )
		INTENSITY = MAX( CLAMP( DOT( NORMAL, HALFVECTOR ))^Shininess , 0 )
		REFLECTEDLIGHT = LIGHTCOLOR * (materialSpecularColor * specularMapValueColor) * INTENSITY
		*/

		// Get light direction for this fragment
		float3 lightDir = normalize(input.WorldPos - LightPosition); // per pixel diffuse lighting

																	 // Note: Non-uniform scaling not supported
		float diffuseLighting = saturate(dot(input.Normal, -lightDir));

		// Introduce fall-off of light intensity
		diffuseLighting *= (LightDistanceSquared / dot(LightPosition - input.WorldPos, LightPosition - input.WorldPos));

		// Using Blinn half angle modification for performance over correctness
		float3 h = normalize(normalize(CameraPos - input.WorldPos) - lightDir);
		float specularLighting = pow(saturate(dot(h, input.Normal)), Shininess);
		//float4 texel = tex2D(texsampler, input.TexCoords);

		//a - float4 
		//d - lambertian intensity (0-1 value) * lightDiffuseColor * materialDiffuseColor * diffuseTextureColor
		//s - specular intensity(vector to light and surface normal) * lightSpecularColor * materialSpecularColor * specularTextureColor
		//e - materialEmmissiveColor * emissiveTextureColor
		//finalColor = a + d + s + e;
		//
		return float4(saturate(
			AmbientLightColor.xyz +
			(diffuseColor.xyz * LightDiffuseColor * diffuseLighting * 2.6) + // Use light diffuse vector as intensity multiplier
			(specularColor.xyz * LightSpecularColor * specularLighting * 1.5) + // Use light specular vector as intensity multiplier
			emissiveColor
		), 1);
	}
	else
	{
		float4 color;
		color.x = input.tex[0];
		color.y = input.tex[1];
		color.z = 0;
		color.w = 1;
		return color;
	}
	
}