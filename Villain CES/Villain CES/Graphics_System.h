#pragma once
#include "Entity.h"
#include "Graphics_Component.h"
#include"Gateware Redistribution R5B/Interface/G_System/GInput.h"
#include"Gateware Redistribution R5B/Interface/G_System/GKeyDefines.h"
#include "MyVertexShader.csh"
#include "MyPixelShader.csh"
#include "PrimalVertexShader.csh"
#include "PrimalPixelShader.csh"
#include <vector>
using namespace GW;
using namespace SYSTEM;
class CGraphicsSystem
{
public:
	// global declarations
	IDXGISwapChain * m_pd3dSwapchain;             // the pointer to the swap chain interface
	ID3D11Device *m_pd3dDevice;                     // the pointer to our Direct3D device interface
	ID3D11DeviceContext *m_pd3dDeviceContext;           // the pointer to our Direct3D device context
	D3D11_VIEWPORT m_d3dViewport;
	ID3D11RenderTargetView *m_pd3dRenderTargetView;
	ID3D11DepthStencilState* m_pd3dDepthStencilState;
	ID3D11DepthStencilView * m_pd3dDepthStencilView;
	ID3D11Texture2D* m_pd3dDepthStencil;
	std::vector<TPrimalVert> m_ctVectorPosition;

	struct TMyMatrixBufferType
	{
		XMMATRIX m_d3dWorldMatrix;
		XMMATRIX m_d3dViewMatrix;
		XMMATRIX m_d3dProjectionMatrix;
		XMFLOAT4X4 m_ad3dJointMatrices[33];
	};


	struct TPrimalMatrixBufferType
	{
		XMMATRIX m_d3dWorldMatrix;
		XMMATRIX m_d3dViewMatrix;
		XMMATRIX m_d3dProjectionMatrix;
	};

	struct TBlinnPhongType
	{
		XMFLOAT3 m_d3dLightPosition;
		XMFLOAT3 m_d3dCameraPosition;
		float m_fShininess;
		int m_nPadding;
	};
	struct TCamera {
		TPrimalMatrixBufferType t_cameraMatrix;
	};


										   // function prototypes
	CGraphicsSystem();
	~CGraphicsSystem();
	void InitD3D(HWND cTheWindow);     // sets up and initializes Direct3D
	void UpdateD3D();

	void CleanD3D(TWorld * ptWorld);         // closes Direct3D and releases memory
	//First Frame
	void CreateBuffers(TWorld * ptWorld);
	//First Frame
	void CreateShaders(ID3D11Device* pd3dDevice);
	//Every Frame
	void InitMyShaderData(ID3D11DeviceContext * pd3dDeviceContext, XMMATRIX d3dWorldMatrix, XMMATRIX d3dViewMatrix, XMMATRIX d3dProjectionMatrix, int nMask, XMFLOAT3 d3dLightPosition, XMFLOAT3 d3dCameraPosition, XMFLOAT4X4 *pd3dJointsForVS);
	void InitPrimalShaderData(ID3D11DeviceContext * pd3dDeviceContext, XMMATRIX d3dWorldMatrix, XMMATRIX d3dViewMatrix, XMMATRIX d3dProjectionMatrix, TDebugMesh tDebugMesh);

	//Every Frame
	void ExecutePipeline(ID3D11DeviceContext *pd3dDeviceContext, int m_nIndexCount, int nGraphicsMask, int nShaderID);
	void UpdateBuffer(TWorld * ptWorld, std::vector<TSimpleMesh> vtVertexVector, int nEntity, int nMask);
	XMMATRIX SetDefaultViewMatrix();
	XMMATRIX SetDefaultPerspective();
	XMMATRIX SetDefaultWorldPosition();

	/*
	* DebugCamera(): This fuction checks what keys are being pressed and modifing a camera's trasforms according to key response.
	*  
	* Ins:                  
	*                    TMatrixBufferType tWVP
	*					HWND cTheWindow
	* Outs:
	*
	* Returns:              XMMARIX
	*
	* Mod. Date:              06/29/2018
	* Mod. Initials:          ZFB
	*/

	XMMATRIX DebugCamera(XMMATRIX tWVP);
	
	GReturn InitlizeGInput(HWND cTheWindow);
private:
	int InputCheck(int GInput);
private:
	ID3D11VertexShader * m_pd3dPrimalVertexShader;
	ID3D11PixelShader	*m_pd3dPrimalPixelShader;
	ID3D11InputLayout	*m_pd3dPrimalInputLayout;
	ID3D11Buffer		*m_pd3dPrimalMatrixBuffer;

	ID3D11VertexShader	*m_pd3dMyVertexShader;
	ID3D11PixelShader	*m_pd3dMyPixelShader;
	ID3D11InputLayout	*m_pd3dMyInputLayout;
	ID3D11Buffer		*m_pd3dMyMatrixBuffer;

	ID3D11Buffer		*m_pd3dBlinnPhongBuffer;
	float					m_ncameraXPosition;
	float					m_ncameraYPosition;
	float					m_ncameraZPosition;
	GInput*				m_pcMyInput;
	

};

