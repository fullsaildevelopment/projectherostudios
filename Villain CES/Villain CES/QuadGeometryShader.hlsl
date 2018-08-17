/////////////
// CBUFFER //
/////////////
#pragma pack_matrix( row_major )

cbuffer MatrixBuffer : register(b0)
{
	matrix d3dWorldMatrix;
	matrix d3dViewMatrix;
	matrix d3dProjectionMatrix;
};

struct GSOutput
{
	float4 pos : SV_POSITION;
};

[maxvertexcount(4)]
void QuadGeometryShader(
	point float4 input[1] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{	
	//
	//Compute 4 triangle strip vertices (quad) in local space.
	//The quad faces down the +z axis in local space.
	//
	float halfWidth = .5f ;//* input[0].sizeW.x;
	float halfHeight = .5f ;//* gIn[0].sizeW.y;

	float4 v[4];
	v[0] = float4(-halfWidth, -halfHeight, 0.0f, 1.0f);
	v[1] = float4(+halfWidth, -halfHeight, 0.0f, 1.0f);
	v[2] = float4(-halfWidth, +halfHeight, 0.0f, 1.0f);
	v[3] = float4(+halfWidth, +halfHeight, 0.0f, 1.0f);
	

	for (uint i = 0; i < 4; i++)
	{
		GSOutput element;
		element.pos = v[i];
		output.Append(element);
	}


}