#pragma once
#include "Entity.h"
#include "Graphics_Component.h"
#include "MyVertexShader.csh"
#include "MyPixelShader.csh"
#include "PrimalVertexShader.csh"
#include "PrimalPixelShader.csh"
#include "QuadGeometryShader.csh"
#include "QuadVertexShader.csh"
#include "QuadPixelShader.csh"
#include <vector>
class CGraphicsSystem
{
public:
	// global declarations
	IDXGISwapChain * m_pd3dSwapchain;             // the pointer to the swap chain interface
	ID3D11Device *m_pd3dDevice;                     // the pointer to our Direct3D device interface
	ID3D11DeviceContext *m_pd3dDeviceContext;           // the pointer to our Direct3D device context
	ID3D11RenderTargetView *m_pd3dRenderTargetView;
	ID3D11DepthStencilState* m_pd3dDepthStencilState;
	ID3D11DepthStencilView * m_pd3dDepthStencilView;
	ID3D11Texture2D* m_pd3dDepthStencil;
	D3D11_VIEWPORT m_d3dViewport;


	struct TQuadVertexBufferType
	{
		XMMATRIX m_d3dWorldMatrix;
		XMMATRIX m_d3dViewMatrix;
		XMMATRIX m_d3dProjectionMatrix;
	};

	struct TQuadPixelBufferType
	{
		/*Nothing*/
	};

	struct TMyVertexBufferType
	{
		XMMATRIX m_d3dWorldMatrix;
		XMMATRIX m_d3dViewMatrix;
		XMMATRIX m_d3dProjectionMatrix;
	};

	struct TMyPixelBufferType
	{
		/*Nothing*/
	};

	struct TPrimalVertexBufferType
	{
		XMMATRIX m_d3dWorldMatrix;
		XMMATRIX m_d3dViewMatrix;
		XMMATRIX m_d3dProjectionMatrix;
	};

	struct TPrimalPixelBufferType
	{
		XMFLOAT4 m_d3dCollisionColor;
	};

	struct TBlinnPhongType
	{
		XMFLOAT3 m_d3dLightPosition;
		XMFLOAT3 m_d3dCameraPosition;
		float m_fShininess;
		int m_nPadding;
	};

	//First Frame
	CGraphicsSystem();
	~CGraphicsSystem();
	void InitD3D(HWND cTheWindow);
	void CleanD3D(TWorld *ptPlanet);
	void CreateBuffers(TWorld * ptWorld);
	void CreateEntityBuffer(TWorld * ptWorld, int nEnityIndex);
	void CreateShaders(ID3D11Device* pd3dDevice);

	XMMATRIX SetDefaultViewMatrix();
	XMMATRIX SetDefaultWorldPosition();
	XMMATRIX SetDefaultPerspective();
	XMMATRIX SetDefaultOffset();
	//Every Frame
	void UpdateD3D();
	void InitPrimalShaderData(ID3D11DeviceContext * pd3dDeviceContext, XMMATRIX d3dWorldMatrix, XMMATRIX d3dViewMatrix, XMMATRIX d3dProjectionMatrix, TDebugMesh tDebugMesh, XMMATRIX CameraMatrix);
	void InitPrimalShaderData2(ID3D11DeviceContext * pd3dDeviceContext, TPrimalVertexBufferType d3dVertexBuffer, TPrimalPixelBufferType d3dPixelBuffer, TSimpleMesh tSimpleMesh, XMMATRIX CameraMatrix);
	void InitMyShaderData(ID3D11DeviceContext * pd3dDeviceContext, TMyVertexBufferType d3dVertexBuffer, TMesh tSimpleMesh, XMMATRIX CameraMatrix);
	void ExecutePipeline(ID3D11DeviceContext *pd3dDeviceContext, int m_nIndexCount, int nGraphicsMask, int nShaderID);
	void UpdateBuffer(TWorld * ptWorld, std::vector<TSimpleMesh> vtVertexVector, int nEntity, int nMask);

	ImporterData ReadMesh(const char * input_file_path);
	XMVECTOR GetCameraPos();
	XMMATRIX SetDefaultCameraMatrix();
	//XMMATRIX SetPlayerViewMatrix(XMMATRIX d3d_ViewM, XMMATRIX d3d_playerM);
	//Last Frame
	void CleanD3DLevel(TWorld * ptWorld);         //  releases memory
	void CleanD3DObject(TWorld *ptPlanet, int nEntityIndex);
private:
	ID3D11VertexShader  *m_pd3dPrimalVertexShader;
	ID3D11PixelShader	*m_pd3dPrimalPixelShader;
	ID3D11InputLayout	*m_pd3dPrimalInputLayout;
	ID3D11Buffer		*m_pd3dPrimalVertexBuffer;
	ID3D11Buffer		*m_pd3dPrimalPixelBuffer;

	ID3D11VertexShader  *m_pd3dQuadVertexShader;
	ID3D11PixelShader	*m_pd3dQuadPixelShader;
	ID3D11GeometryShader*m_pd3dQuadGeometryShader;
	ID3D11InputLayout	*m_pd3dQuadInputLayout;
	ID3D11Buffer		*m_pd3dQuadVertexBuffer;
	ID3D11Buffer		*m_pd3dQuadGeometryBuffer;
	ID3D11Buffer		*m_pd3dQuadPixelBuffer;

	ID3D11VertexShader	*m_pd3dMyVertexShader;
	ID3D11PixelShader	*m_pd3dMyPixelShader;
	ID3D11InputLayout	*m_pd3dMyInputLayout;
	ID3D11Buffer		*m_pd3dMyVertexBuffer;

	ID3D11Buffer		*m_pd3dBlinnPhongBuffer;
	ID3D11Debug			*debug;
	float				m_fCameraXPosition;
	float				m_fCameraYPosition;
	float				m_fCameraZPosition;
	float				m_fAspectRatio;
	float				m_fFOV;
	float				m_fNewX;
	float				m_fNewY;
	float				m_fMouseRotationSpeed;
	float				m_fMouseMovementSpeed;
};