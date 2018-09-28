#pragma pack_matrix( row_major )

cbuffer MatrixBuffer : register(b0)
{
	matrix d3dWorldMatrix;
	matrix d3dViewMatrix;
	matrix d3dProjectionMatrix;
	float4 endPoint;
};

struct TLineGeoInputType
{
	float4 pos : SV_POSITION;
	float4 color : POSITION1;
};

struct TQuadPixelInputType
{
	float4 pos : SV_POSITION;
	float4 color : POSITION1;
	uint PrimID : SV_PrimitiveID;
};

[maxvertexcount(2)]
void main(point TLineGeoInputType input[1], uint primID : SV_PrimitiveID, inout LineStream<TQuadPixelInputType> output)
{

	for (uint i = 0; i < 2; i++)
	{
		TQuadPixelInputType element;
		element.pos = input[0].pos;
		element.color = input[0].color
		element.PrimID = primID;
		output.Append(element);
	}
}