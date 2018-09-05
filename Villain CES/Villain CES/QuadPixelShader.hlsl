cbuffer TQuadPixelBufferType : register(b0)
{
	float4 d3dBackgroundColor;
};

float4 QuadPixelShader() : SV_TARGET
{
	return d3dBackgroundColor;
}