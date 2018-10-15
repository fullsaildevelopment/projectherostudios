
struct TLinePixelInputType
{
	float4 d3dPos : SV_POSITION;
	float4 d3dColor : POSITION;
};


float4 main(TLinePixelInputType pixelO) : SV_TARGET
{
	return pixelO.d3dColor;
}