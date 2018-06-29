#pragma once
#include "Entity.h"
#include "Graphics_Component.h"
#include"Gateware Redistribution R5B/Interface/G_System/GInput.h"
#include"Gateware Redistribution R5B/Interface/G_System/GKeyDefines.h"
#include "MyVertexShader.csh"
#include "MyPixelShader.csh"
#include <vector>
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

	struct TMatrixBufferType
	{
		XMMATRIX m_d3dWorldMatrix;
		XMMATRIX m_d3dViewMatrix;
		XMMATRIX m_d3dProjectionMatrix;
		XMFLOAT4X4 m_ad3dJointMatrices[33];
	};

	struct TBlinnPhongType
	{
		XMFLOAT3 m_d3dLightPosition;
		XMFLOAT3 m_d3dCameraPosition;
		float m_fShininess;
		int m_nPadding;
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
	void InitShaderData(ID3D11DeviceContext * pd3dDeviceContext, XMMATRIX d3dWorldMatrix, XMMATRIX d3dViewMatrix, XMMATRIX d3dProjectionMatrix, bool bCollide, int nMask, XMFLOAT3 d3dLightPosition, XMFLOAT3 d3dCameraPosition, XMFLOAT4X4 *pd3dJointsForVS);
	//Every Frame
	void ExecutePipeline(TWorld * ptWorld, ID3D11DeviceContext *pd3dDeviceContext, int m_nIndexCount, int nEntity);
	void UpdateBuffer(TWorld * ptWorld, std::vector<TSimpleMesh> vtVertexVector, int nEntity, int nMask);


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
	XMMATRIX DebugCamera(TMatrixBufferType tWVP, HWND cTheWindow);
	

private:
	ID3D11VertexShader	*m_pd3dVertexShader;
	ID3D11PixelShader	*m_pd3dPixelShader;
	ID3D11InputLayout	*m_pd3dInputLayout;
	ID3D11Buffer		*m_pd3dMatrixBuffer;
	ID3D11Buffer		*m_pd3dBlinnPhongBuffer;

};

