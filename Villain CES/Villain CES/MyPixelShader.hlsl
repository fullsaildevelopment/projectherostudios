/////////////
// GLOBALS //
/////////////
Texture2D g_d3dDiffuseTexture;
Texture2D g_d3dSpecularTexture;
Texture2D g_d3dEmissiveTexture;
SamplerState g_SampleType;
/////////////
// CBUFFER //
/////////////
cbuffer BlinnPhongBuffer
{
	float3 d3dLightPosition;
	float3 d3dCameraPosition;
	float fShininess;
};

//////////////
// TYPEDEFS //
//////////////
struct TPixelInputType
{
	float4 d3dPosition : SV_POSITION;
	float3 d3dNormal : NORMAL;
	float2 d3dTexture : TEXCOORD;
	float4 d3dWorldPosition : POSITION1;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 MyPixelShader(TPixelInputType tInput) : SV_TARGET
{
	if ((d3dCameraPosition[0] || d3dCameraPosition[1] || d3dCameraPosition[2]))
	{
		// Phong relfection is ambient + light-diffuse + spec highlights.
		// I = Ia*ka*Oda + fatt*Ip[kd*Od(N.L) + ks(R.V)^n]
		//
		float4 d3dDiffuseColor = g_d3dDiffuseTexture.Sample(g_SampleType, (float2) tInput.d3dTexture);
		float4 d3dSpecularColor = g_d3dSpecularTexture.Sample(g_SampleType, (float2) tInput.d3dTexture);
		float4 d3dEmissiveColor = g_d3dEmissiveTexture.Sample(g_SampleType, (float2) tInput.d3dTexture);
		float fXTwoMinusXOne = (tInput.d3dWorldPosition[0] - d3dLightPosition[0]);
		float fYTwoMinusYOne = (tInput.d3dWorldPosition[1] - d3dLightPosition[1]);
		float fZTwoMinusZOne = (tInput.d3dWorldPosition[2] - d3dLightPosition[2]);
		fXTwoMinusXOne *= fXTwoMinusXOne;
		fYTwoMinusYOne *= fYTwoMinusYOne;
		fZTwoMinusZOne *= fZTwoMinusZOne;
		float fDistance = sqrt(fXTwoMinusXOne + fYTwoMinusYOne + fZTwoMinusZOne);
		float fLightDistanceSquared = pow(fDistance, 2);

		float3 d3dAmbientLightColor;
		d3dAmbientLightColor.x = 0.0f;
		d3dAmbientLightColor.y = 0.0f;
		d3dAmbientLightColor.z = 0.0f;

		float3 d3dLightDiffuseColor;
		d3dLightDiffuseColor.x = 1.0f;
		d3dLightDiffuseColor.y = 1.0f;
		d3dLightDiffuseColor.z = 1.0f;

		float3 d3dLightSpecularColor;
		d3dLightSpecularColor.x = 1.0f;
		d3dLightSpecularColor.y = 1.0f;
		d3dLightSpecularColor.z = 1.0f;

		/*
		Specular Component
		VIEWDIR = NORMALIZE( CAMd3dWorldPosition – SURFACEPOS )
		HALFVECTOR = NORMALIZE(( -d3dLightDirection ) + VIEWDIR )
		INTENSITY = MAX( CLAMP( DOT( NORMAL, HALFVECTOR ))^fShininess , 0 )
		REFLECTEDLIGHT = LIGHTCOLOR * (materialSpecularColor * specularMapValueColor) * INTENSITY
		*/

		// Get light direction for this fragment
		float3 d3dLightDirection = normalize(tInput.d3dWorldPosition - d3dLightPosition); // per pixel diffuse lighting

																	 // Note: Non-uniform scaling not supported
		float fDiffuseLighting = saturate(dot(tInput.d3dNormal, -d3dLightDirection));

		// Introduce fall-off of light intensity
		fDiffuseLighting *= (fLightDistanceSquared / dot(d3dLightPosition - tInput.d3dWorldPosition, d3dLightPosition - tInput.d3dWorldPosition));

		// Using Blinn half angle modification for performance over correctness
		float3 d3dBlinnHalfAngle = normalize(normalize(d3dCameraPosition - tInput.d3dWorldPosition) - d3dLightDirection);
		float fSpecularLighting = pow(saturate(dot(d3dBlinnHalfAngle, tInput.d3dNormal)), fShininess);
		//float4 texel = tex2D(texsampler, tInput.d3dTextureCoords);

		//a - float4 
		//d - lambertian intensity (0-1 value) * d3dLightDiffuseColor * materialDiffuseColor * g_d3dDiffuseTextureColor
		//s - specular intensity(vector to light and surface normal) * d3dLightSpecularColor * materialSpecularColor * g_d3dSpecularTextureColor
		//e - materialEmmissiveColor * g_d3dEmissiveTextureColor
		//finalColor = a + d + s + e;
		//
		return float4(saturate(
			d3dAmbientLightColor.xyz +
			(d3dDiffuseColor.xyz * d3dLightDiffuseColor * fDiffuseLighting * 2.6) + // Use light diffuse vector as intensity multiplier
			(d3dSpecularColor.xyz * d3dLightSpecularColor * fSpecularLighting * 1.5) + // Use light specular vector as intensity multiplier
			d3dEmissiveColor
		), 1);
	}
	else
	{
		float4 d3dColor;
		d3dColor.x = tInput.d3dTexture[0];
		d3dColor.y = tInput.d3dTexture[1];
		d3dColor.z = 0;
		d3dColor.w = 1;
		return d3dColor;
	}
	
}