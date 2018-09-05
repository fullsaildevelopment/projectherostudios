TextureCube gCubeMap;

SamplerState samTriLinearSam;
//{
//	Filter = MIN_MAG_MIP_LINEAR;
//	AddressU = Wrap;
//	AddressV = Wrap;
//};

struct TPixelInputType
{
	float4 PosH : SV_POSITION;
	float3 PosL : TEXCOORD0;
};

float4 SkyboxPixelShader(TPixelInputType pin) : SV_TARGET
{
	return gCubeMap.Sample(samTriLinearSam, pin.PosL);
}