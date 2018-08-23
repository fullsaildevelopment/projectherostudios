#pragma pack_matrix( row_major )

cbuffer MatrixBuffer : register(b0)
{
	matrix d3dWorldMatrix;
	matrix d3dViewMatrix;
	matrix d3dProjectionMatrix;
};
struct TVertexInputType
{
	float3 PosL : POSITION0;
};

struct TPixelInputType
{
	float4 PosH : SV_POSITION;
	float3 PosL : TEXCOORD0;
};


TPixelInputType SkyboxVertexShader(TVertexInputType vin)
{
	TPixelInputType vout;
	float4x4 gWorldViewProj = mul(d3dWorldMatrix, d3dViewMatrix);
	gWorldViewProj = mul(gWorldViewProj, d3dProjectionMatrix);
	// Set z = w so that z/w = 1 (i.e., skydome always on far plane).
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj).xyzw;

	// Use local vertex position as cubemap lookup vector.
	vout.PosL = vin.PosL;

	return vout;
}