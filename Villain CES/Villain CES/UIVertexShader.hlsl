/////////////
// CBUFFER //
/////////////
#pragma pack_matrix( row_major )

cbuffer MatrixBuffer : register(b0)
{
	//matrix d3dWorldMatrix;
	//matrix d3dViewMatrix;
	//matrix d3dProjectionMatrix;

	float start;
	float end;
	float ratio;
	float padding;
};

//////////////
// TYPEDEFS //
//////////////
struct TUIVertexInputType
{
	float3 d3dPosition : POSITION0;
	float2 d3dTexture : TEXCOORD;
};

struct TUIPixelInputType
{
	float4 d3dPosition : SV_POSITION;
	float2 d3dTexture : TEXCOORD;
	float4 d3dColor : POSITION1;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////

TUIPixelInputType UIVertexShader( TUIVertexInputType tInput )
{
	TUIPixelInputType tOutput;

	tOutput.d3dPosition = float4(tInput.d3dPosition.xyz, 1);

	tOutput.d3dTexture = tInput.d3dTexture;

	tOutput.d3dColor = float4(0, 0, 0, 1);

	if (start == -1 || end == -1 || ratio == -1)
	{
		//tOutput.d3dPosition = mul(tOutput.d3dPosition, d3dWorldMatrix);
		//tOutput.d3dPosition = mul(tOutput.d3dPosition, d3dViewMatrix);
		//tOutput.d3dPosition = mul(tOutput.d3dPosition, d3dProjectionMatrix);
	}
	else
	{
		float barPosition = ((end - start) * ratio) + start;

		if (barPosition < tOutput.d3dPosition.x)
			tOutput.d3dPosition.x = barPosition;
		
		if (tOutput.d3dPosition.x < start)
			tOutput.d3dPosition.x = start;
	}

	
	return tOutput;
}