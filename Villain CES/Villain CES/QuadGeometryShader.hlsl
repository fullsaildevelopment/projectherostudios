/////////////
// CBUFFER //
/////////////
#pragma pack_matrix( row_major )

cbuffer MatrixBuffer : register(b0)
{
	matrix d3dWorldMatrix;
	matrix d3dViewMatrix;
	matrix d3dProjectionMatrix;
	float fHealth;
};

struct TQuadGeoInputType
{
	float4 d3dPosition : SV_POSITION;
	float4 d3dColor : POSITION1;
};

struct TQuadPixelInputType
{
	float4 pos    : SV_POSITION;
	float4 d3dColor: POSITION1;
	uint   PrimID  : SV_PrimitiveID;
};

[maxvertexcount(4)]
void QuadGeometryShader(point TQuadGeoInputType gIn[1],
	uint primID : SV_PrimitiveID, 
	inout TriangleStream<TQuadPixelInputType> tristream)
{	
	//
	//Compute 4 triangle strip vertices (quad) in local space.
	//The quad faces down the +z axis in local space.
	//
	float halfWidth = 1.0f ;//* input[0].sizeW.x;
	float halfHeight = 0.3f ;//* gIn[0].sizeW.y;

	float4 v[4];
	v[0] = float4(-halfWidth * fHealth, -halfHeight, 0.0f, 1.0f);
	v[1] = float4(-halfWidth * fHealth, +halfHeight, 0.0f, 1.0f);
	v[2] = float4(+halfWidth * fHealth, -halfHeight, 0.0f, 1.0f);
	v[3] = float4(+halfWidth * fHealth, +halfHeight, 0.0f, 1.0f);
	
	float4 center = gIn[0].d3dPosition;
	center = mul(center, d3dWorldMatrix);
	center = mul(center, d3dViewMatrix);
	for (uint i = 0; i < 4; i++)
	{
		TQuadPixelInputType element;
		float4 p;
		p = mul(center + v[i], d3dProjectionMatrix);
		element.pos = p;
		element.d3dColor = gIn[0].d3dColor;
		element.PrimID = primID;
		tristream.Append(element);
	}
}