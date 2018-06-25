#pragma once
#include "Entity.h"
#include "Graphics_Component.h"
#include "MyVertexShader.csh"
#include "MyPixelShader.csh"
#include <vector>
class Graphics_System
{
public:
	// global declarations
	IDXGISwapChain * swapchain;             // the pointer to the swap chain interface
	ID3D11Device *dev;                     // the pointer to our Direct3D device interface
	ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
	D3D11_VIEWPORT viewport;
	ID3D11RenderTargetView *rtv;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView * depthStencilView;
	ID3D11Texture2D* pDepthStencil;
	ID3D11RasterizerState* pRSWireFrame;
	ID3D11RasterizerState* pRSNotWireFrame;
	std::vector<primalVert> vecPos;

	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMFLOAT4X4 joints[33];
	};

	struct BlinnPhongType
	{
		XMFLOAT3 LightPosition;
		XMFLOAT3 CameraPos;
		float Shininess;
		int padding;
	};
										   // function prototypes
	Graphics_System();
	~Graphics_System();
	void InitD3D(HWND hWnd);     // sets up and initializes Direct3D
	void UpdateD3D();

	void CleanD3D(World * wurld);         // closes Direct3D and releases memory
	//First Frame
	void CreateBuffers(World * wurld);
	//First Frame
	void CreateShaders(ID3D11Device* device);
	//Every Frame
	void InitShaderData(ID3D11DeviceContext * deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, bool collide, int mask, XMFLOAT3 lightPos, XMFLOAT3 camPos, XMFLOAT4X4 *x);
	//Every Frame
	void ExecutePipeline(World * wurld, ID3D11DeviceContext *deviceContext, int indexCount, int entity);
	void UpdateBuffer(World * wurld, std::vector<simple_mesh> vertVector, int entity, int mask);


private:
	//D3D11_SAMPLER_DESC	 m_samplerDesc;
	//ID3D11SamplerState	*m_samplerState;
	ID3D11VertexShader	*m_vertexShader;
	ID3D11PixelShader	*m_pixelShader;
	ID3D11InputLayout	*m_layout;
	ID3D11Buffer		*m_matrixBuffer;
	ID3D11Buffer		*m_blinnPhong;

};

