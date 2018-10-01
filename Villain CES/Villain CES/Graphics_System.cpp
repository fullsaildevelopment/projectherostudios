#include "stdafx.h"
#include "Graphics_System.h"
#include <map>
#define FARVIEW 1000
CGraphicsSystem::CGraphicsSystem()
{
	m_fCameraXPosition = 0;
	m_fCameraYPosition = 0.5;
	m_fCameraZPosition = -10;
}

CGraphicsSystem::~CGraphicsSystem()
{
	
}

void CGraphicsSystem::InitD3D(HWND cTheWindow)
{
	#pragma region Main Window Stuff

	#pragma region Swapchain
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC d3dSwapchainDescription;

	// clear out the struct for use
	ZeroMemory(&d3dSwapchainDescription, sizeof(DXGI_SWAP_CHAIN_DESC));

	RECT cRectangle;
	GetClientRect(cTheWindow, &cRectangle);

	// fill the swap chain description struct
	d3dSwapchainDescription.BufferCount = 1;                                    // one back buffer
	d3dSwapchainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	d3dSwapchainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	d3dSwapchainDescription.OutputWindow = cTheWindow;                          // the window to be used
	d3dSwapchainDescription.SampleDesc.Count = 1;                               // how many multisamples
	d3dSwapchainDescription.Windowed = TRUE;                                    // windowed/full-screen mode
	d3dSwapchainDescription.BufferDesc.Width = cRectangle.right - cRectangle.left;
	d3dSwapchainDescription.BufferDesc.Height = cRectangle.bottom - cRectangle.top;
	d3dSwapchainDescription.BufferDesc.RefreshRate.Numerator = 60;
	d3dSwapchainDescription.BufferDesc.RefreshRate.Denominator = 1;

	#ifdef _DEBUG
	unsigned int nDeviceAndSwapchainFlag = D3D11_CREATE_DEVICE_DEBUG;
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		nDeviceAndSwapchainFlag,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&d3dSwapchainDescription,
		&m_pd3dSwapchain,
		&m_pd3dDevice,
		NULL,
		&m_pd3dDeviceContext);
	#endif // DEBUG

	#ifndef _DEBUG
	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&d3dSwapchainDescription,
		&m_pd3dSwapchain,
		&m_pd3dDevice,
		NULL,
		&m_pd3dDeviceContext);

	#endif // !_DEBUG

#pragma endregion

	#pragma region RenderTargetView And Viewport
	m_fAspectRatio = (float)(cRectangle.bottom - cRectangle.top / cRectangle.right - cRectangle.left);

	D3D11_TEXTURE2D_DESC d3dTextureDescription;
	ZeroMemory(&d3dTextureDescription, sizeof(d3dTextureDescription));


	m_pd3dSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_pd3dRenderTargetTexture);
	m_pd3dDevice->CreateRenderTargetView(m_pd3dRenderTargetTexture, NULL, &m_pd3dRenderTargetView);
	float fViewportWidth = 0;
	float fViewportHeight = 0;
	
	m_pd3dRenderTargetTexture->GetDesc(&d3dTextureDescription);
	//pd3dRenderTargetTexture->Release();
	fViewportWidth = (float)d3dTextureDescription.Width;
	fViewportHeight = (float)d3dTextureDescription.Height;

	m_d3dViewport = {
		0,
		0,
		fViewportWidth,
		fViewportHeight,
		0,
		1
	};
#pragma endregion

	#pragma region DepthStencilView And DepthStencilState
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = (UINT)fViewportWidth;
	descDepth.Height = (UINT)fViewportHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	m_pd3dDevice->CreateTexture2D(&descDepth, NULL, &m_pd3dDepthStencil);

	D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDescription;
	d3dDepthStencilViewDescription.Format = DXGI_FORMAT_D32_FLOAT;
	d3dDepthStencilViewDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	d3dDepthStencilViewDescription.Texture2D.MipSlice = 0;
	d3dDepthStencilViewDescription.Flags = 0;
	m_pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencil, NULL, &m_pd3dDepthStencilView);

	D3D11_DEPTH_STENCIL_DESC d3dDepthStencilDescription;

	// Depth test parameters
	d3dDepthStencilDescription.DepthEnable = true;
	d3dDepthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	d3dDepthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	// Stencil test parameters
	d3dDepthStencilDescription.StencilEnable = true;
	d3dDepthStencilDescription.StencilReadMask = 0xFF;
	d3dDepthStencilDescription.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	d3dDepthStencilDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	d3dDepthStencilDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	d3dDepthStencilDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	d3dDepthStencilDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	m_pd3dDevice->CreateDepthStencilState(&d3dDepthStencilDescription, &m_pd3dDepthStencilState);

#pragma endregion

#pragma endregion

	#pragma region RasterStates
	/*
	D3D11_FILL_MODE FillMode;
	D3D11_CULL_MODE CullMode;
	BOOL FrontCounterClockwise;
	INT DepthBias;
	FLOAT DepthBiasClamp;
	FLOAT SlopeScaledDepthBias;
	BOOL DepthClipEnable;
	BOOL ScissorEnable;
	BOOL MultisampleEnable;
	BOOL AntialiasedLineEnable;
	*/
	D3D11_RASTERIZER_DESC NoCull
	{
		D3D11_FILL_SOLID,
		D3D11_CULL_NONE,
		true,
		1,
		1.0f,
		0.0f,
		true,
		false,
		true,
		false
	};

	D3D11_RASTERIZER_DESC Cull
	{
		D3D11_FILL_SOLID,
		D3D11_CULL_FRONT,
		true,
		1,
		1.0f,
		0.0f,
		true,
		false,
		true,
		false
	};

	m_pd3dDevice->CreateRasterizerState(&NoCull, &m_pd3dNoCullRasterizerState);
	m_pd3dDevice->CreateRasterizerState(&Cull, &m_pd3dCullRasterizerState);
#pragma endregion
	
	#pragma region SamplerStates
	D3D11_SAMPLER_DESC sampTmp
	{
		D3D11_FILTER_ANISOTROPIC,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MIRROR,
		1.0f,
		16,
		D3D11_COMPARISON_LESS_EQUAL,
		1,
		0,
		D3D11_FLOAT32_MAX
	};

	m_pd3dDevice->CreateSamplerState(&sampTmp, &m_pd3dSamplerState);
#pragma endregion

	#pragma region RTV
	DXGI_SAMPLE_DESC tmp;
	tmp.Count = 1;
	tmp.Quality = 0;
	D3D11_TEXTURE2D_DESC offScreenDesc
	{
		m_d3dViewport.Width,
		m_d3dViewport.Height,
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		tmp,
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
		0,//D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE,
		D3D11_RESOURCE_MISC_GENERATE_MIPS
	};
	m_pd3dSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_pd3dOutsideGlassRenderToTexture);

	m_pd3dDevice->CreateTexture2D(&offScreenDesc, 0, &m_pd3dOutsideGlassRenderToTexture);
	
	m_pd3dDevice->CreateRenderTargetView(m_pd3dOutsideGlassRenderToTexture, NULL, &m_pd3dOutsideRenderTargetView);
#pragma endregion

	#pragma region BlendState
	D3D11_BLEND_DESC d3dBlendDescription;
	d3dBlendDescription.AlphaToCoverageEnable = false;
	d3dBlendDescription.IndependentBlendEnable = false;
	d3dBlendDescription.RenderTarget[0].BlendEnable = true;
	d3dBlendDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	d3dBlendDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	d3dBlendDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	d3dBlendDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	d3dBlendDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	d3dBlendDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	d3dBlendDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	m_pd3dDevice->CreateBlendState(&d3dBlendDescription, &m_pd3dBlendState);

	m_pd3dDeviceContext->OMSetBlendState(m_pd3dBlendState, 0, 0xffffffff);
#pragma endregion
}

void CGraphicsSystem::UpdateD3D()
{
	const float afBackgroundColor[] = { .5f, .05f, .5f, 1 };
	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);
	m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, afBackgroundColor);
	m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH, 1, 0); // clear it to Z exponential Far.
	m_pd3dDeviceContext->RSSetViewports(1, &m_d3dViewport);
}

void CGraphicsSystem::CleanD3D(TWorld *ptPlanet)
{
	// close and release all existing COM objects
	for (int nEntityIndex = 0; nEntityIndex < ENTITYCOUNT; nEntityIndex++)
	{
		//Check planet's mask at [i] to see what needs to be released
		if (ptPlanet->atGraphicsMask[nEntityIndex].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
		{
			ptPlanet->atDebugMesh[nEntityIndex].m_pd3dVertexBuffer->Release();
		}

		if (ptPlanet->atGraphicsMask[nEntityIndex].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID))
		{
			ptPlanet->atSimpleMesh[nEntityIndex].m_pd3dVertexBuffer->Release();
			ptPlanet->atSimpleMesh[nEntityIndex].m_pd3dIndexBuffer->Release();
		}
		if (ptPlanet->atGraphicsMask[nEntityIndex].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID) || ptPlanet->atGraphicsMask[nEntityIndex].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_SKYBOX | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			ptPlanet->atMesh[nEntityIndex].m_pd3dVertexBuffer->Release();
			ptPlanet->atMesh[nEntityIndex].m_pd3dIndexBuffer->Release();
			//ptPlanet->atMesh[nEntityIndex].m_d3dSRVDiffuse->Release();
		}
		destroyEntity(ptPlanet, nEntityIndex);
#ifdef _DEBUG
		//HRESULT result = debug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL);

#endif // !_DEBUG
	}

	m_pd3dSwapchain->Release();
	m_pd3dDevice->Release();
	m_pd3dDeviceContext->Release();
	m_pd3dDepthStencil->Release();
	m_pd3dDepthStencilState->Release();
	m_pd3dDepthStencilView->Release();
	m_pd3dRenderTargetView->Release();

	m_pd3dPrimalVertexShader->Release();
	m_pd3dPrimalPixelShader->Release();
	m_pd3dPrimalInputLayout->Release();
	m_pd3dPrimalVertexBuffer->Release();
	m_pd3dPrimalPixelBuffer->Release();

	m_pd3dQuadVertexShader->Release();
	m_pd3dQuadPixelShader->Release();
	m_pd3dQuadGeometryShader->Release();
	m_pd3dQuadGeometryBuffer->Release();
	m_pd3dQuadPixelBuffer->Release();

	m_pd3dMyVertexShader->Release();
	m_pd3dMyPixelShader->Release();
	m_pd3dMyInputLayout->Release();
	m_pd3dMyVertexBuffer->Release();

	m_pd3dAnimatedVertexShader->Release();
	m_pd3dAnimatedPixelShader->Release();
	m_pd3dAnimatedInputLayout->Release();
	m_pd3dAnimatedVertexBuffer->Release();

	m_pd3dUIVertexShader->Release();
	m_pd3dUIPixelShader->Release();
	m_pd3dUIInputLayout->Release();
	m_pd3dUIVertexBuffer->Release();
	m_pd3dUIPixelBuffer->Release();

	m_pd3dSkyboxVertexShader->Release();
	m_pd3dSkyboxPixelShader->Release();
	m_pd3dSkyboxInputLayout->Release();
	m_pd3dSkyboxVertexBuffer->Release();
	if (debug != nullptr)
	{
		HRESULT result = debug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL);
	}
	//m_pcMyInput->DecrementCount();
}

void CGraphicsSystem::CleanD3DLevel(TWorld * ptPlanet)
{
	for (int nEntityIndex = 0; nEntityIndex < ENTITYCOUNT; nEntityIndex++)
	{
		/*if (ptPlanet->atUIMask[nEntityIndex].m_tnUIMask == (COMPONENT_LABEL | COMPONENT_TEXT | COMPONENT_UIMASK | COMPONENT_BUTTON))
			continue;*/
		//Check planet's mask at [i] to see what needs to be released
		if (ptPlanet->atGraphicsMask[nEntityIndex].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
		{
			ptPlanet->atDebugMesh[nEntityIndex].m_pd3dVertexBuffer->Release();
		}

		if (ptPlanet->atGraphicsMask[nEntityIndex].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID))
		{
			ptPlanet->atSimpleMesh[nEntityIndex].m_pd3dVertexBuffer->Release();
			ptPlanet->atSimpleMesh[nEntityIndex].m_pd3dIndexBuffer->Release();
		}

		if (ptPlanet->atGraphicsMask[nEntityIndex].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
		{
			ptPlanet->atMesh[nEntityIndex].m_pd3dVertexBuffer->Release();
			ptPlanet->atMesh[nEntityIndex].m_pd3dIndexBuffer->Release();
			//ptPlanet->atMesh[nEntityIndex].m_d3dSRVDiffuse->Release();
		}

		if (ptPlanet->atGraphicsMask[nEntityIndex].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_SHADERID))
		{
			ptPlanet->atMesh[nEntityIndex].m_pd3dVertexBuffer->Release();
			ptPlanet->atMesh[nEntityIndex].m_pd3dIndexBuffer->Release();
		}
		destroyEntity(ptPlanet, nEntityIndex);
	}
}

void CGraphicsSystem::CleanD3DObject(TWorld * ptPlanet, int nEntityIndex)
{
	if (ptPlanet->atGraphicsMask[nEntityIndex].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
	{
		ptPlanet->atDebugMesh[nEntityIndex].m_pd3dVertexBuffer->Release();
	}

	if (ptPlanet->atGraphicsMask[nEntityIndex].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID))
	{
		ptPlanet->atSimpleMesh[nEntityIndex].m_pd3dVertexBuffer->Release();
		ptPlanet->atSimpleMesh[nEntityIndex].m_pd3dIndexBuffer->Release();
	}
	destroyEntity(ptPlanet, nEntityIndex);
}

void CGraphicsSystem::CreateShaders(ID3D11Device * device)
{
	#pragma region MyShaders
	D3D11_BUFFER_DESC d3dMyVertexBufferDesc;
	device->CreateVertexShader(MyVertexShader, sizeof(MyVertexShader), NULL, &m_pd3dMyVertexShader);
	device->CreatePixelShader(MyPixelShader, sizeof(MyPixelShader), NULL, &m_pd3dMyPixelShader);
	//Input Layout Setup
	//Now setup the layout of the data that goes into the shader.
	D3D11_INPUT_ELEMENT_DESC m_d3dMyLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	//Get a count of the elements in the layout.
	int	nElements = sizeof(m_d3dMyLayoutDesc) / sizeof(m_d3dMyLayoutDesc[0]);

	//Create the input layout.
	device->CreateInputLayout(m_d3dMyLayoutDesc, nElements, MyVertexShader,
		sizeof(MyVertexShader), &m_pd3dMyInputLayout);

	//Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	d3dMyVertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dMyVertexBufferDesc.ByteWidth = sizeof(TMyVertexBufferType);
	d3dMyVertexBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dMyVertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dMyVertexBufferDesc.MiscFlags = 0;
	d3dMyVertexBufferDesc.StructureByteStride = 0;

	//Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	device->CreateBuffer(&d3dMyVertexBufferDesc, NULL, &m_pd3dMyVertexBuffer);
#pragma endregion

	#pragma region PrimalShaders
	D3D11_BUFFER_DESC d3dPrimalMatrixBufferDesc;
	D3D11_BUFFER_DESC d3dPrimalPixelBufferDesc;
	device->CreateVertexShader(PrimalVertexShader, sizeof(PrimalVertexShader), NULL, &m_pd3dPrimalVertexShader);
	device->CreatePixelShader(PrimalPixelShader, sizeof(PrimalPixelShader), NULL, &m_pd3dPrimalPixelShader);
	//Input Layout Setup
	//Now setup the layout of the data that goes into the shader.
	D3D11_INPUT_ELEMENT_DESC m_d3dPrimalLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "POSITION", 1, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	//Get a count of the elements in the layout.
	int nPrimalElements = sizeof(m_d3dPrimalLayoutDesc) / sizeof(m_d3dPrimalLayoutDesc[0]);

	//Create the vertex input layout.
	device->CreateInputLayout(m_d3dPrimalLayoutDesc, nPrimalElements, PrimalVertexShader,
		sizeof(PrimalVertexShader), &m_pd3dPrimalInputLayout);

	//Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	d3dPrimalMatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dPrimalMatrixBufferDesc.ByteWidth = sizeof(TPrimalVertexBufferType);
	d3dPrimalMatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dPrimalMatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dPrimalMatrixBufferDesc.MiscFlags = 0;
	d3dPrimalMatrixBufferDesc.StructureByteStride = 0;

	//Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	device->CreateBuffer(&d3dPrimalMatrixBufferDesc, NULL, &m_pd3dPrimalVertexBuffer);

	//Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	d3dPrimalPixelBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dPrimalPixelBufferDesc.ByteWidth = sizeof(TPrimalPixelBufferType);
	d3dPrimalPixelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dPrimalPixelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dPrimalPixelBufferDesc.MiscFlags = 0;
	d3dPrimalPixelBufferDesc.StructureByteStride = 0;

	//Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	device->CreateBuffer(&d3dPrimalPixelBufferDesc, NULL, &m_pd3dPrimalPixelBuffer);
#pragma endregion

	#pragma region UIShaders
	D3D11_BUFFER_DESC d3dUIMatrixBufferDesc;
	D3D11_BUFFER_DESC d3dUIPixelBufferDesc;
	device->CreateVertexShader(UIVertexShader, sizeof(UIVertexShader), NULL, &m_pd3dUIVertexShader);
	device->CreatePixelShader(UIPixelShader, sizeof(UIPixelShader), NULL, &m_pd3dUIPixelShader);

	//Input Layout Setup
	//Now setup the layout of the data that goes into the shader.
	D3D11_INPUT_ELEMENT_DESC m_d3dUILayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//Get a count of the elements in the layout.
	int nUIElements = sizeof(m_d3dUILayoutDesc) / sizeof(m_d3dUILayoutDesc[0]);

	//Create the vertex input layout.
	device->CreateInputLayout(m_d3dUILayoutDesc, nUIElements, UIVertexShader,
		sizeof(UIVertexShader), &m_pd3dUIInputLayout);

	//Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	d3dUIMatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dUIMatrixBufferDesc.ByteWidth = sizeof(TUIVertexBufferType);
	d3dUIMatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dUIMatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dUIMatrixBufferDesc.MiscFlags = 0;
	d3dUIMatrixBufferDesc.StructureByteStride = 0;

	//Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	device->CreateBuffer(&d3dUIMatrixBufferDesc, NULL, &m_pd3dUIVertexBuffer);

	//Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	d3dUIPixelBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dUIPixelBufferDesc.ByteWidth = sizeof(TUIPixelBufferType);
	d3dUIPixelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dUIPixelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dUIPixelBufferDesc.MiscFlags = 0;
	d3dUIPixelBufferDesc.StructureByteStride = 0;

	//Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	device->CreateBuffer(&d3dUIPixelBufferDesc, NULL, &m_pd3dUIPixelBuffer);
#pragma endregion

	#pragma region QuadGeometryShaders
	D3D11_BUFFER_DESC d3dQuadMatrixBufferDesc;
	D3D11_BUFFER_DESC d3dQuadPixelBufferDesc;
	device->CreateVertexShader(QuadVertexShader, sizeof(QuadVertexShader), NULL, &m_pd3dQuadVertexShader);
	device->CreatePixelShader(QuadPixelShader, sizeof(QuadPixelShader), NULL, &m_pd3dQuadPixelShader);
	device->CreateGeometryShader(QuadGeometryShader, sizeof(QuadGeometryShader), NULL, &m_pd3dQuadGeometryShader);
	//Input Layout Setup
	//Now setup the layout of the data that goes into the shader.
	D3D11_INPUT_ELEMENT_DESC m_d3dQuadLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "POSITION", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	//Get a count of the elements in the layout.
	int nQuadElements = sizeof(m_d3dQuadLayoutDesc) / sizeof(m_d3dQuadLayoutDesc[0]);

	//Create the vertex input layout.
	device->CreateInputLayout(m_d3dQuadLayoutDesc, nQuadElements, QuadVertexShader,sizeof(QuadVertexShader), &m_pd3dQuadInputLayout);

	//Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	d3dQuadMatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dQuadMatrixBufferDesc.ByteWidth = sizeof(TQuadGeometryBufferType);
	d3dQuadMatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dQuadMatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dQuadMatrixBufferDesc.MiscFlags = 0;
	d3dQuadMatrixBufferDesc.StructureByteStride = 0;

	//Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	device->CreateBuffer(&d3dQuadMatrixBufferDesc, NULL, &m_pd3dQuadGeometryBuffer);


	//Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	d3dQuadPixelBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dQuadPixelBufferDesc.ByteWidth = sizeof(TQuadPixelBufferType);
	d3dQuadPixelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dQuadPixelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dQuadPixelBufferDesc.MiscFlags = 0;
	d3dQuadPixelBufferDesc.StructureByteStride = 0;

	//Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	device->CreateBuffer(&d3dQuadPixelBufferDesc, NULL, &m_pd3dQuadPixelBuffer);
#pragma endregion
#if 0
#pragma region Line Geometry Shaders
	D3D11_BUFFER_DESC d3dLineMatrixBufferDesc;
	

	device->CreateGeometryShader(LineGeometryShader, sizeof(LineGeometryShader), NULL, &m_pd3dLineGeometryShader);
	// Use Quad Layout Desc, Quad vertex shader, and Quad Pixel Shader
	d3dLineMatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dLineMatrixBufferDesc.ByteWidth = sizeof(TLineGeometryBufferType);
	d3dLineMatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dLineMatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dLineMatrixBufferDesc.MiscFlags = 0;
	d3dLineMatrixBufferDesc.StructureByteStride = 0;
	// create buffer
	device->CreateBuffer(&d3dLineMatrixBufferDesc, NULL, &m_pd3dLineGeometryBuffer);


#pragma endregion 
#endif // 0

	

	#pragma region SkyboxShaders
	D3D11_BUFFER_DESC d3dSkyboxMatrixBufferDesc;
	D3D11_BUFFER_DESC d3dSkyboxPixelBufferDesc;
	device->CreateVertexShader(SkyboxVertexShader, sizeof(SkyboxVertexShader), NULL, &m_pd3dSkyboxVertexShader);
	device->CreatePixelShader(SkyboxPixelShader, sizeof(SkyboxPixelShader), NULL, &m_pd3dSkyboxPixelShader);

	//Input Layout Setup
	//Now setup the layout of the data that goes into the shader.
	D3D11_INPUT_ELEMENT_DESC m_d3dSkyboxLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//Get a count of the elements in the layout.
	int nSkyboxElements = sizeof(m_d3dSkyboxLayoutDesc) / sizeof(m_d3dSkyboxLayoutDesc[0]);

	//Create the vertex input layout.
	device->CreateInputLayout(m_d3dSkyboxLayoutDesc, nSkyboxElements, SkyboxVertexShader,
		sizeof(SkyboxVertexShader), &m_pd3dSkyboxInputLayout);

	//Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	d3dSkyboxMatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dSkyboxMatrixBufferDesc.ByteWidth = sizeof(TMyVertexBufferType);
	d3dSkyboxMatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dSkyboxMatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dSkyboxMatrixBufferDesc.MiscFlags = 0;
	d3dSkyboxMatrixBufferDesc.StructureByteStride = 0;

	//Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	device->CreateBuffer(&d3dSkyboxMatrixBufferDesc, NULL, &m_pd3dSkyboxVertexBuffer);
#pragma endregion

	#pragma region AnimatedShaders
	D3D11_BUFFER_DESC d3dAnimatedVertexBufferDesc;
	device->CreateVertexShader(AnimatedVertexShader, sizeof(AnimatedVertexShader), NULL, &m_pd3dAnimatedVertexShader);
	device->CreatePixelShader(AnimatedPixelShader, sizeof(AnimatedPixelShader), NULL, &m_pd3dAnimatedPixelShader);
	//Input Layout Setup
	//Now setup the layout of the data that goes into the shader.
	D3D11_INPUT_ELEMENT_DESC m_d3dAnimatedLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	//Get a count of the elements in the layout.
	nElements = sizeof(m_d3dAnimatedLayoutDesc) / sizeof(m_d3dAnimatedLayoutDesc[0]);

	//Create the input layout.
	device->CreateInputLayout(m_d3dAnimatedLayoutDesc, nElements, AnimatedVertexShader,
		sizeof(AnimatedVertexShader), &m_pd3dAnimatedInputLayout);

	//Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	d3dAnimatedVertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dAnimatedVertexBufferDesc.ByteWidth = sizeof(TAnimatedVertexBufferType);
	d3dAnimatedVertexBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dAnimatedVertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dAnimatedVertexBufferDesc.MiscFlags = 0;
	d3dAnimatedVertexBufferDesc.StructureByteStride = 0;

	//Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	device->CreateBuffer(&d3dAnimatedVertexBufferDesc, NULL, &m_pd3dAnimatedVertexBuffer);
#pragma endregion
}

TMaterialOptimized CGraphicsSystem::CreateTexturesFromFile(TMaterialImport * arrayOfMaterials, int numberOfEntities)
{
		//Maps texture filenames to entity indices
		map<int, string> Map_EntityIndex_FileName;
		string x;
		std::vector<int> materialIndex;
		ID3D11ShaderResourceView **SRVDefault = new ID3D11ShaderResourceView *[1];

		bool insertFlag = true;
		if (arrayOfMaterials[0].m_tPBRFileNames[0] != NULL)
		{
			x = arrayOfMaterials[0].m_tPBRFileNames[0];
			Map_EntityIndex_FileName.insert(pair<int, string>(0, x));
			//Vector of indices, one index for each entity that correspond to texture file names
			materialIndex.push_back(0);
			//loop number of entity times
			for (int i = 1; i < numberOfEntities; i++)
			{
				//reset flag to true that we should insert filename into map
				insertFlag = true;
				//store filename in char*
				char* x = arrayOfMaterials[i].m_tPBRFileNames[0];
				int lambert = arrayOfMaterials[i].lambert;
				//loop number of elements in map times
				string strEntity;
				string strMap;
				if (x != nullptr)
				{
					strEntity = x;
					for (int mapIndex = 0; mapIndex < Map_EntityIndex_FileName.size(); mapIndex++)
					{
						if (!Map_EntityIndex_FileName[mapIndex].empty())
						{
							strMap = Map_EntityIndex_FileName[mapIndex];
							//if this current filename in the map is equal to the one of this entity
							if (strMap == strEntity)
							{
								//set insert flag to false so we don't insert this texture to the map
								insertFlag = false;
								materialIndex.push_back(mapIndex);
								break;
							}
						}
					}
					if (insertFlag && *x != NULL)
					{
						Map_EntityIndex_FileName.insert(pair<int, string>(i, x));
						materialIndex.push_back(i);
					}
				}
				else if (lambert == 3)
				{
					materialIndex.push_back(-2);
				}
				else
				{
					materialIndex.push_back(-1);
				}
			}
		}
		else if(arrayOfMaterials[0].m_tFileNames[0] != NULL && arrayOfMaterials[0].m_tFileNames[0][1] != '.')
		{
			x = arrayOfMaterials[0].m_tFileNames[0];
			Map_EntityIndex_FileName.insert(pair<int, string>(0, x));
			//Vector of indices, one index for each entity that correspond to texture file names
			materialIndex.push_back(0);
			//loop number of entity times
			for (int i = 1; i < numberOfEntities; i++)
			{
				//reset flag to true that we should insert filename into map
				insertFlag = true;
				//store filename in char*
				char* x = arrayOfMaterials[i].m_tFileNames[0];
				//loop number of elements in map times
				string strEntity;
				string strMap;
				if (x != nullptr)
				{
					strEntity = x;
					for (int mapIndex = 0; mapIndex < Map_EntityIndex_FileName.size(); mapIndex++)
					{
						if (!Map_EntityIndex_FileName[mapIndex].empty())
						{
							strMap = Map_EntityIndex_FileName[mapIndex];
							//if this current filename in the map is equal to the one of this entity
							if (strMap == strEntity)
							{
								//set insert flag to false so we don't insert this texture to the map
								insertFlag = false;
								materialIndex.push_back(mapIndex);
								break;
							}
						}
					}
					if (insertFlag && *x != NULL)
					{
						Map_EntityIndex_FileName.insert(pair<int, string>(i, x));
						materialIndex.push_back(i);
					}
				}
				else
				{
					materialIndex.push_back(-1);
				}
			}
		}
		else//Apply a wood texture to the mesh or if it's a material gun projectile
		{
			int* Map_SRVIndex_EntityIndexArray = new int[1];
			Map_SRVIndex_EntityIndexArray[0] = -3;
			if (arrayOfMaterials[0].lambert == 4)
			{
				for (int i = 0; i < numberOfEntities; i++)
				{
					materialIndex.push_back(-3);
				}				
				CreateWICTextureFromFile(m_pd3dDevice, L"TestScene_V1.fbm\\PaintedMetal04_col.jpg", NULL, &SRVDefault[0], NULL);
				TMaterialOptimized answer;
				answer.Map_SRVIndex_EntityIndex = Map_SRVIndex_EntityIndexArray;
				answer.materialIndex = materialIndex;
				answer.SRVArrayOfMaterials = SRVDefault;
				answer.numberOfMaterials = 1;
				return answer;
			}
			else
			{
				int* Map_SRVIndex_EntityIndexArray = new int[1];
				Map_SRVIndex_EntityIndexArray[0] = 0;
				for (int i = 0; i < numberOfEntities; i++)
				{
					materialIndex.push_back(0);
				}
				CreateWICTextureFromFile(m_pd3dDevice, L"TestScene_V1.fbm\\Wood01_col.jpg", NULL, &SRVDefault[0], NULL);
				TMaterialOptimized answer;
				answer.Map_SRVIndex_EntityIndex = Map_SRVIndex_EntityIndexArray;
				answer.materialIndex = materialIndex;
				answer.SRVArrayOfMaterials = SRVDefault;
				answer.numberOfMaterials = 1;
				return answer;
			}
		}

		std::map<int, string>::iterator mapItr;
		wchar_t fnPBR[260];
		size_t result = 0;
		mbstate_t d;
		int count = 0;
		for (mapItr = Map_EntityIndex_FileName.begin(); mapItr != Map_EntityIndex_FileName.end(); mapItr++)
		{
			if (mapItr->second.size())
			{
				count++;
			}
		}

		ID3D11ShaderResourceView **SRVArrayOfMaterials = new ID3D11ShaderResourceView*[count];
		count = 0;
		//Key - Count
		//Value - Entity Index
		std::map<int, int> Map_SRVIndex_EntityIndex;
		for (mapItr = Map_EntityIndex_FileName.begin(); mapItr != Map_EntityIndex_FileName.end(); mapItr++)
		{
			if (mapItr->second.size())
			{
				const char * name = mapItr->second.c_str();
				int size = mapItr->second.size();
				mbsrtowcs_s(&result, fnPBR, 260, (const char **)(&name), size, &d);
				CreateWICTextureFromFile(m_pd3dDevice, &fnPBR[1], NULL, &SRVArrayOfMaterials[count], NULL);
				Map_SRVIndex_EntityIndex.insert(pair<int, int>(count, mapItr->first));
				count++;
				int test = 0;
			}
		}
		int* Map_SRVIndex_EntityIndexArray = new int[count];
		int intArrayIndex = 0;
		for (mapItr = Map_EntityIndex_FileName.begin(); mapItr != Map_EntityIndex_FileName.end(); mapItr++)
		{
			if (mapItr->second.size())
			{
				Map_SRVIndex_EntityIndexArray[intArrayIndex] = mapItr->first;
				intArrayIndex++;
				int test = 0;
			}
		}
		TMaterialOptimized answer;
		answer.Map_SRVIndex_EntityIndex = Map_SRVIndex_EntityIndexArray;
		answer.materialIndex = materialIndex;
		answer.SRVArrayOfMaterials = SRVArrayOfMaterials;
		answer.numberOfMaterials = Map_SRVIndex_EntityIndex.size();
		return answer;
}

ID3D11ShaderResourceView * CGraphicsSystem::TexturesToCubeMap(ID3D11DeviceContext * pd3dDeviceContext, ID3D11Resource * srcTex[6])
{
	// Each element in the texture array has the same format/dimensions.
	D3D11_TEXTURE2D_DESC texElementDesc;
	((ID3D11Texture2D*)srcTex[0])->GetDesc(&texElementDesc);

	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width = texElementDesc.Width;
	texArrayDesc.Height = texElementDesc.Height;
	texArrayDesc.MipLevels = texElementDesc.MipLevels;
	texArrayDesc.ArraySize = 6;
	texArrayDesc.Format = texElementDesc.Format;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	pd3dDeviceContext->GetDevice(&m_pd3dDevice);
	ID3D11Texture2D* texArray = 0;
	m_pd3dDevice->CreateTexture2D(&texArrayDesc, 0, &texArray);

	// Copy individual texture elements into texture array.
	D3D11_BOX sourceRegion;

	//Copy the mip-map levels of the textures
	for (UINT x = 0; x < 6; x++)
	{
		for (UINT mipLevel = 0; mipLevel < texArrayDesc.MipLevels; mipLevel++)
		{
			sourceRegion.left = 0;
			sourceRegion.right = (texArrayDesc.Width >> mipLevel);
			sourceRegion.top = 0;
			sourceRegion.bottom = (texArrayDesc.Height >> mipLevel);
			sourceRegion.front = 0;
			sourceRegion.back = 1;

			//test for overflow
			if (sourceRegion.bottom == 0 || sourceRegion.right == 0)
				break;

			pd3dDeviceContext->CopySubresourceRegion(texArray, D3D11CalcSubresource(mipLevel, x, texArrayDesc.MipLevels), 0, 0, 0, srcTex[x], mipLevel, &sourceRegion);
		}
	}

	// Create a resource view to the texture array.
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	viewDesc.TextureCube.MostDetailedMip = 0;
	viewDesc.TextureCube.MipLevels = texArrayDesc.MipLevels;
	ID3D11ShaderResourceView * srv;
	m_pd3dDevice->CreateShaderResourceView(texArray, &viewDesc, &srv);
	return srv;
}

void CGraphicsSystem::UpdateD3D_RenderToTexture()
{
	const float afBackgroundColor[] = { .5f, .05f, .5f, 1 };

	//m_pd3dDeviceContext->GenerateMips(
	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dOutsideRenderTargetView, m_pd3dDepthStencilView);
	m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dOutsideRenderTargetView, afBackgroundColor);
	m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH, 1, 0); // clear it to Z exponential Far.
	m_pd3dDeviceContext->RSSetViewports(1, &m_d3dViewport);
}

void CGraphicsSystem::CreateBuffers(TWorld *ptPlanet)//init first frame
{
	CreateShaders(m_pd3dDevice);
	for (int nCurrentEntity = 0; nCurrentEntity < ENTITYCOUNT; nCurrentEntity++)
	{
		//Check Mask to see what buffers need to get created
		if (ptPlanet->atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
		{
			if (ptPlanet->atDebugMesh[nCurrentEntity].m_nVertexCount)
				m_pd3dDevice->CreateBuffer(&ptPlanet->atDebugMesh[nCurrentEntity].m_d3dVertexBufferDesc, &ptPlanet->atDebugMesh[nCurrentEntity].m_d3dVertexData, &ptPlanet->atDebugMesh[nCurrentEntity].m_pd3dVertexBuffer);
		}
		if (ptPlanet->atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID))
		{
			if (ptPlanet->atSimpleMesh[nCurrentEntity].m_nIndexCount && ptPlanet->atSimpleMesh[nCurrentEntity].m_nVertexCount)
			{
				m_pd3dDevice->CreateBuffer(&ptPlanet->atSimpleMesh[nCurrentEntity].m_d3dVertexBufferDesc, &ptPlanet->atSimpleMesh[nCurrentEntity].m_d3dVertexData, &ptPlanet->atSimpleMesh[nCurrentEntity].m_pd3dVertexBuffer);
				m_pd3dDevice->CreateBuffer(&ptPlanet->atSimpleMesh[nCurrentEntity].m_d3dIndexBufferDesc, &ptPlanet->atSimpleMesh[nCurrentEntity].m_d3dIndexData, &ptPlanet->atSimpleMesh[nCurrentEntity].m_pd3dIndexBuffer);
			}
		}
		if ((ptPlanet->atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
			|| (ptPlanet->atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_SHADERID))
			|| (ptPlanet->atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_SKYBOX | COMPONENT_TEXTURE | COMPONENT_SHADERID)))
		{
			if (ptPlanet->atMesh[nCurrentEntity].m_nIndexCount && ptPlanet->atMesh[nCurrentEntity].m_nVertexCount)
			{
				m_pd3dDevice->CreateBuffer(&ptPlanet->atMesh[nCurrentEntity].m_d3dVertexBufferDesc, &ptPlanet->atMesh[nCurrentEntity].m_d3dVertexData, &ptPlanet->atMesh[nCurrentEntity].m_pd3dVertexBuffer);
				m_pd3dDevice->CreateBuffer(&ptPlanet->atMesh[nCurrentEntity].m_d3dIndexBufferDesc, &ptPlanet->atMesh[nCurrentEntity].m_d3dIndexData, &ptPlanet->atMesh[nCurrentEntity].m_pd3dIndexBuffer);
			}
		}
		if (ptPlanet->atGraphicsMask[nCurrentEntity].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_ANIMATION | COMPONENT_SHADERID))
		{
			if (ptPlanet->atMesh[nCurrentEntity].m_nIndexCount && ptPlanet->atMesh[nCurrentEntity].m_nVertexCount)
			{
				m_pd3dDevice->CreateBuffer(&ptPlanet->atMesh[nCurrentEntity].m_d3dVertexBufferDesc, &ptPlanet->atMesh[nCurrentEntity].m_d3dVertexData, &ptPlanet->atMesh[nCurrentEntity].m_pd3dVertexBuffer);
				m_pd3dDevice->CreateBuffer(&ptPlanet->atMesh[nCurrentEntity].m_d3dIndexBufferDesc, &ptPlanet->atMesh[nCurrentEntity].m_d3dIndexData, &ptPlanet->atMesh[nCurrentEntity].m_pd3dIndexBuffer);
			}
		}
	}
}

void CGraphicsSystem::CreateEntityBuffer(TWorld * ptWorld, int nEnityIndex)
{
	if (ptWorld->atGraphicsMask[nEnityIndex].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
	{
		if (ptWorld->atDebugMesh[nEnityIndex].m_nVertexCount)
			m_pd3dDevice->CreateBuffer(&ptWorld->atDebugMesh[nEnityIndex].m_d3dVertexBufferDesc, &ptWorld->atDebugMesh[nEnityIndex].m_d3dVertexData, &ptWorld->atDebugMesh[nEnityIndex].m_pd3dVertexBuffer);
	}
	if (ptWorld->atGraphicsMask[nEnityIndex].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID))
	{
		if (ptWorld->atSimpleMesh[nEnityIndex].m_nIndexCount && ptWorld->atSimpleMesh[nEnityIndex].m_nVertexCount)
		{
			m_pd3dDevice->CreateBuffer(&ptWorld->atSimpleMesh[nEnityIndex].m_d3dVertexBufferDesc, &ptWorld->atSimpleMesh[nEnityIndex].m_d3dVertexData, &ptWorld->atSimpleMesh[nEnityIndex].m_pd3dVertexBuffer);
			m_pd3dDevice->CreateBuffer(&ptWorld->atSimpleMesh[nEnityIndex].m_d3dIndexBufferDesc, &ptWorld->atSimpleMesh[nEnityIndex].m_d3dIndexData, &ptWorld->atSimpleMesh[nEnityIndex].m_pd3dIndexBuffer);
		}
	}
	if (ptWorld->atGraphicsMask[nEnityIndex].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
	{
		if (ptWorld->atMesh[nEnityIndex].m_nIndexCount && ptWorld->atMesh[nEnityIndex].m_nVertexCount)
		{
			m_pd3dDevice->CreateBuffer(&ptWorld->atMesh[nEnityIndex].m_d3dVertexBufferDesc, &ptWorld->atMesh[nEnityIndex].m_d3dVertexData, &ptWorld->atMesh[nEnityIndex].m_pd3dVertexBuffer);
			m_pd3dDevice->CreateBuffer(&ptWorld->atMesh[nEnityIndex].m_d3dIndexBufferDesc, &ptWorld->atMesh[nEnityIndex].m_d3dIndexData, &ptWorld->atMesh[nEnityIndex].m_pd3dIndexBuffer);
		}
	}

	if (ptWorld->atGraphicsMask[nEnityIndex].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_SHADERID))
	{
		if (ptWorld->atMesh[nEnityIndex].m_nIndexCount && ptWorld->atMesh[nEnityIndex].m_nVertexCount)
		{
			m_pd3dDevice->CreateBuffer(&ptWorld->atMesh[nEnityIndex].m_d3dVertexBufferDesc, &ptWorld->atMesh[nEnityIndex].m_d3dVertexData, &ptWorld->atMesh[nEnityIndex].m_pd3dVertexBuffer);
			m_pd3dDevice->CreateBuffer(&ptWorld->atMesh[nEnityIndex].m_d3dIndexBufferDesc, &ptWorld->atMesh[nEnityIndex].m_d3dIndexData, &ptWorld->atMesh[nEnityIndex].m_pd3dIndexBuffer);
		}
	}
}

void CGraphicsSystem::UpdateBuffer(TWorld * ptWorld, std::vector<TSimpleMesh> vtVertexVector, int nEntity, int nMask)
{
	bool bIsVertexBufferSet = false;
	if (bIsVertexBufferSet)
	{
		// This is where it copies the new vertices to the buffer.
		// but it's causing flickering in the entire screen...
		D3D11_MAPPED_SUBRESOURCE d3dMappedBufferResource;
		if (nMask == COMPONENT_DEBUGMESH)
		{
			m_pd3dDeviceContext->Map(ptWorld->atDebugMesh[nEntity].m_pd3dVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedBufferResource);
			memcpy(d3dMappedBufferResource.pData, &vtVertexVector[0], sizeof(vtVertexVector));
			m_pd3dDeviceContext->Unmap(ptWorld->atDebugMesh[nEntity].m_pd3dVertexBuffer, 0);
		}
	}
	else
	{
		UINT nVertexBufferStride = sizeof(TSimpleMesh);
		UINT nVertexBufferOffset = 0;

		D3D11_SUBRESOURCE_DATA d3dBufferResourceData;
		ZeroMemory(&d3dBufferResourceData, sizeof(d3dBufferResourceData));
		d3dBufferResourceData.pSysMem = &vtVertexVector[0];
		if (nMask == COMPONENT_DEBUGMESH)
		{
			// This is run in the first frame. But what if new vertices are added to the scene?
			ptWorld->atDebugMesh[nEntity].m_d3dVertexBufferDesc.ByteWidth = sizeof(TSimpleMesh) * ptWorld->atDebugMesh[nEntity].m_nVertexCount;

			if (ptWorld->atDebugMesh[nEntity].m_pd3dVertexBuffer)
				ptWorld->atDebugMesh[nEntity].m_pd3dVertexBuffer->Release();
			m_pd3dDevice->CreateBuffer(&ptWorld->atDebugMesh[nEntity].m_d3dVertexBufferDesc, &d3dBufferResourceData, &ptWorld->atDebugMesh[nEntity].m_pd3dVertexBuffer);
			m_pd3dDeviceContext->IASetVertexBuffers(0, 1, &ptWorld->atDebugMesh[nEntity].m_pd3dVertexBuffer, &nVertexBufferStride, &nVertexBufferOffset);
		}
		bIsVertexBufferSet = true;
	}
}

XMMATRIX CGraphicsSystem::SetDefaultCameraMatrix()
{
	XMMATRIX DefaultCameraMatrix;
	DefaultCameraMatrix = SetDefaultWorldPosition();

	return DefaultCameraMatrix;
}

XMMATRIX CGraphicsSystem::ResetAimModeCameraOffset()
{

	return XMMatrixTranslationFromVector(XMVectorSet(-0.5, 1.7f, 8.5f, 1.0f));
}

XMMATRIX CGraphicsSystem::SetDefaultViewMatrix()
{
	XMMATRIX DefaultViewMatrix;

	DefaultViewMatrix = SetDefaultWorldPosition();

	return DefaultViewMatrix;
}

XMMATRIX CGraphicsSystem::SetDefaultPerspective()
{
	XMMATRIX DefaultPerspectiveMatrix;
	// the 90 is for fov if we want to implement field of view
	m_fFOV = 90.0f;

	DefaultPerspectiveMatrix.r[0].m128_f32[0] = 1 / tan(m_fFOV* 0.5f * 3.15f / 180);
	DefaultPerspectiveMatrix.r[0].m128_f32[1] = 0;
	DefaultPerspectiveMatrix.r[0].m128_f32[2] = 0;
	DefaultPerspectiveMatrix.r[0].m128_f32[3] = 0;

	DefaultPerspectiveMatrix.r[1].m128_f32[0] = 0;
	DefaultPerspectiveMatrix.r[1].m128_f32[1] = 1 / tan(m_fFOV * 0.5f * 3.15f / 180);
	DefaultPerspectiveMatrix.r[1].m128_f32[2] = 0;
	DefaultPerspectiveMatrix.r[1].m128_f32[3] = 0;

	DefaultPerspectiveMatrix.r[2].m128_f32[0] = 0;
	DefaultPerspectiveMatrix.r[2].m128_f32[1] = 0;
	DefaultPerspectiveMatrix.r[2].m128_f32[2] = FARVIEW / (FARVIEW - 0.1f);
	DefaultPerspectiveMatrix.r[2].m128_f32[3] = 1;

	DefaultPerspectiveMatrix.r[3].m128_f32[0] = 0;
	DefaultPerspectiveMatrix.r[3].m128_f32[1] = 0;
	DefaultPerspectiveMatrix.r[3].m128_f32[2] = -(FARVIEW * 0.1f) / (FARVIEW - 0.1f);
	DefaultPerspectiveMatrix.r[3].m128_f32[3] = 0;
	return DefaultPerspectiveMatrix;
}

XMMATRIX CGraphicsSystem::SetDefaultPerspective(float FOV)
{
	XMMATRIX DefaultPerspectiveMatrix;
	// the 90 is for fov if we want to implement field of view
	m_fFOV = FOV;

	DefaultPerspectiveMatrix.r[0].m128_f32[0] = 1 / tan(m_fFOV* 0.5f * 3.15f / 180);
	DefaultPerspectiveMatrix.r[0].m128_f32[1] = 0;
	DefaultPerspectiveMatrix.r[0].m128_f32[2] = 0;
	DefaultPerspectiveMatrix.r[0].m128_f32[3] = 0;

	DefaultPerspectiveMatrix.r[1].m128_f32[0] = 0;
	DefaultPerspectiveMatrix.r[1].m128_f32[1] = 1 / tan(m_fFOV * 0.5f * 3.15f / 180);
	DefaultPerspectiveMatrix.r[1].m128_f32[2] = 0;
	DefaultPerspectiveMatrix.r[1].m128_f32[3] = 0;

	DefaultPerspectiveMatrix.r[2].m128_f32[0] = 0;
	DefaultPerspectiveMatrix.r[2].m128_f32[1] = 0;
	DefaultPerspectiveMatrix.r[2].m128_f32[2] = FARVIEW / (FARVIEW- 0.1f);
	DefaultPerspectiveMatrix.r[2].m128_f32[3] = 1;

	DefaultPerspectiveMatrix.r[3].m128_f32[0] = 0;
	DefaultPerspectiveMatrix.r[3].m128_f32[1] = 0;
	DefaultPerspectiveMatrix.r[3].m128_f32[2] = -(FARVIEW * 0.1f) / (FARVIEW - 0.1f);
	DefaultPerspectiveMatrix.r[3].m128_f32[3] = 0;
	return DefaultPerspectiveMatrix;
}

XMMATRIX CGraphicsSystem::SetDefaultOffset()
{
	return XMMatrixTranslation(-0.9f, 2.0f, 7.5f);
}

XMMATRIX CGraphicsSystem::SetDefaultWorldPosition()
{
	XMMATRIX DefaultPerspectiveMatrix;
	// the 90 is for fov if we want to implement field of view
	DefaultPerspectiveMatrix.r[0].m128_f32[0] = 1;
	DefaultPerspectiveMatrix.r[0].m128_f32[1] = 0;
	DefaultPerspectiveMatrix.r[0].m128_f32[2] = 0;
	DefaultPerspectiveMatrix.r[0].m128_f32[3] = 0;

	DefaultPerspectiveMatrix.r[1].m128_f32[0] = 0;
	DefaultPerspectiveMatrix.r[1].m128_f32[1] = 1;
	DefaultPerspectiveMatrix.r[1].m128_f32[2] = 0;
	DefaultPerspectiveMatrix.r[1].m128_f32[3] = 0;

	DefaultPerspectiveMatrix.r[2].m128_f32[0] = 0;
	DefaultPerspectiveMatrix.r[2].m128_f32[1] = 0;
	DefaultPerspectiveMatrix.r[2].m128_f32[2] = 1;
	DefaultPerspectiveMatrix.r[2].m128_f32[3] = 0;

	DefaultPerspectiveMatrix.r[3].m128_f32[0] = 1.0f;
	DefaultPerspectiveMatrix.r[3].m128_f32[1] = 0;
	DefaultPerspectiveMatrix.r[3].m128_f32[2] = -10.0f;
	DefaultPerspectiveMatrix.r[3].m128_f32[3] = 1.0f;
	return DefaultPerspectiveMatrix;
}

ImporterData CGraphicsSystem::ReadMesh(const char * input_file_path)
{
#pragma region Declarations
	int * lamberts = nullptr;
	TMyAmbientColor * ambientColor = nullptr;
	TMyDiffuseColor * diffuseColor = nullptr;
	TMySpecularColor* specularColor = nullptr;
	TMyEmissiveColor* emmissiveColor = nullptr;
	TMyFileNameSizes* fileNameSizes = nullptr;
	TMyFileNames* fileNames = nullptr;
	TPBRFileNameSizes* pbrfileNameSizes = nullptr;
	TPBRFileNames* pbrfileNames = nullptr;
	double *dTransparencyOrShininess = nullptr;
	float *fMetallicness = nullptr;
	float *fRoughness = nullptr;
	int hasMaterial = 0;
	int bufferSize = 0;
	int materialCount = 0;
	int bSize = sizeof(int);
	unsigned int * indexBuf = nullptr;
	TMeshFormat * meshArray = nullptr;
	int polyVertCount = 0;
	int uniqueVertexCount = 0;
	int hasPolygon = 0;
	int animFrameCount = 0;
	int nodeCount = 0;
	double duration = 0;
	int hasPose = 0;
	double worldTranslation[3];
	double worldRotation[3];
	double worldScaling[3];
	std::vector<int> parent_Indices;
	std::vector<XMMATRIX> invBindPosesForJoints;
	TAnimationClip myAnim;

	int meshCount = 0;
#pragma endregion

	std::ifstream matFile(input_file_path, std::ios::in | std::ios::binary);
	matFile.read((char*)&meshCount, sizeof(int));

#pragma region ImportDataMemoryAllocations

	ImporterData tImportMe;
	// Has Vertex tyoe and Joints 
	tImportMe.vtMeshes = new TMeshImport[meshCount];
	tImportMe.vtMaterials = new TMaterialImport[meshCount];
	tImportMe.vtAnimations = new TAnimationImport[meshCount];
	tImportMe.meshCount = meshCount;
#pragma endregion

	for (int meshIndex = 0; meshIndex < meshCount; meshIndex++)
	{
		matFile.read((char*)&hasPose, sizeof(int));
		if (hasPose)
		{

			bSize += sizeof(int);
			matFile.read((char*)&duration, sizeof(double));
			matFile.read((char*)&nodeCount, sizeof(int));
			matFile.read((char*)&animFrameCount, sizeof(int));

			bSize += sizeof(double) + sizeof(int) + sizeof(int);
			for (int i = 0; i < animFrameCount; i++)
			{
				TKeyframe myKey;

				//current frame time offset
				double curTime = 0;
				matFile.read((char*)&curTime, sizeof(double));
				bSize += sizeof(double);
				XMMATRIX temp;
				for (int h = 0; h < nodeCount; h++)
				{
					matFile.read((char*)&temp, sizeof(float) * 16);
					bSize += sizeof(float) * 16;

					myKey.m_vd3dJointMatrices.push_back(temp);
				}

				//myKey.joints = XMLoadFloat4x4(&temp);
				myKey.dTime = curTime / 1000;
				//Holds Joints
				myAnim.m_vtKeyFrames.push_back(myKey);
			}
			for (int i = 0; i < nodeCount; i++)
			{
				int j = 0;
				matFile.read((char*)&j, sizeof(int));
				bSize += sizeof(int);
				parent_Indices.push_back(j);
			}

			XMMATRIX fTemp;
			/*			matFile.clear();
			matFile.seekg(bSize);*/
			for (int h = 0; h < nodeCount; h++)
			{
				matFile.read((char*)&fTemp, sizeof(float) * 16);
				bSize += sizeof(float) * 16;

				invBindPosesForJoints.push_back(fTemp);
			}

			myAnim.dDuration = duration;
#pragma endregion
		}
		tImportMe.vtAnimations[meshIndex].hasPose = hasPose;
		//Container of Anim Clips
		tImportMe.vtAnimations[meshIndex].animClip = myAnim;
		matFile.read((char*)&hasMaterial, sizeof(int));

		if (hasMaterial)
		{
			matFile.read((char*)&materialCount, sizeof(int));
#pragma region Material Initializations
			lamberts = new int[materialCount];
			ambientColor = new TMyAmbientColor[materialCount];
			diffuseColor = new TMyDiffuseColor[materialCount];
			specularColor = new TMySpecularColor[materialCount];
			emmissiveColor = new TMyEmissiveColor[materialCount];
			pbrfileNameSizes = new TPBRFileNameSizes[materialCount];
			pbrfileNames = new TPBRFileNames[materialCount];
			fileNameSizes = new TMyFileNameSizes[materialCount];
			fileNames = new TMyFileNames[materialCount];
			dTransparencyOrShininess = new double[materialCount];
			fRoughness = new float[materialCount];
			fMetallicness = new float[materialCount];
			dTransparencyOrShininess[0] = 0;
			fRoughness[0] = 0;
			fMetallicness[0] = 0;
#pragma endregion
		}

		if (hasMaterial)
		{
			for (int materialIndex = 0; materialIndex < materialCount; materialIndex++)
			{
#pragma region ReadMaterials
				bSize += sizeof(int);
				matFile.read((char*)&lamberts[materialIndex], sizeof(int));

				if (lamberts[materialIndex] == 2)
				{
					bufferSize = sizeof(fMetallicness[materialIndex]) + sizeof(fRoughness[materialIndex]);
				}
				else if (lamberts[materialIndex] == 1 || lamberts[materialIndex] == 3 || lamberts[materialIndex] == 4)
				{
					bufferSize = sizeof(ambientColor[materialIndex]) + sizeof(diffuseColor[materialIndex]) + sizeof(emmissiveColor[materialIndex]) + sizeof(dTransparencyOrShininess[materialIndex]);
				}
				else if (lamberts[materialIndex] == 0)
				{
					bufferSize = sizeof(ambientColor[materialIndex]) + sizeof(diffuseColor[materialIndex]) + sizeof(specularColor[materialIndex]) + sizeof(emmissiveColor[materialIndex]) + sizeof(dTransparencyOrShininess[materialIndex]);
				}

				char *matBuffer = new char[bufferSize];
				bSize += sizeof(int);
				matFile.read(matBuffer, bufferSize);
				if (lamberts[materialIndex] == 2)
				{
					memcpy(&fMetallicness[materialIndex], &matBuffer[0], sizeof(fMetallicness[materialIndex]));

					memcpy(&fRoughness[materialIndex], &matBuffer[sizeof(fMetallicness[materialIndex])], sizeof(fRoughness[materialIndex]));

					for (int i = 0; i < 9; i++)
					{
						matFile.read((char*)&pbrfileNameSizes[materialIndex][i], sizeof(pbrfileNameSizes[materialIndex][i]));
					}

					for (int i = 0; i < 9; i++)
					{
						if (pbrfileNameSizes[materialIndex][i] != NULL)
						{
							pbrfileNames[materialIndex][i] = new char[pbrfileNameSizes[materialIndex][i]];
						}
					}

					char tmp = NULL;
					for (int i = 0; i < 9; i++)
					{
						for (int j = 0; j < pbrfileNameSizes[materialIndex][i]; j++)
						{
							pbrfileNames[materialIndex][i][j] = 0;
						}
						if (pbrfileNameSizes[materialIndex][i])
						{
							for (int j = 0; j < pbrfileNameSizes[materialIndex][i]; j++)
							{
								matFile.read(&tmp, sizeof(tmp));
								if (tmp == '^')
								{
									break;
								}
								else
								{
									pbrfileNames[materialIndex][i][j] = tmp;
								}
							}
						}
					}
				}
				else if (lamberts[materialIndex] == 1 || lamberts[materialIndex] == 3 || lamberts[materialIndex] == 4)
				{
					memcpy(&ambientColor[materialIndex], &matBuffer[0], sizeof(ambientColor));

					memcpy(&diffuseColor[materialIndex], &matBuffer[sizeof(ambientColor[materialIndex])], sizeof(diffuseColor[materialIndex]));

					memcpy(&emmissiveColor[materialIndex], &matBuffer[sizeof(ambientColor[materialIndex]) + sizeof(diffuseColor[materialIndex])], sizeof(emmissiveColor[materialIndex]));

					memcpy(&dTransparencyOrShininess[materialIndex], &matBuffer[sizeof(ambientColor[materialIndex]) + sizeof(diffuseColor[materialIndex])] + sizeof(emmissiveColor[materialIndex]), sizeof(dTransparencyOrShininess[materialIndex]));

					for (int i = 0; i < 5; i++)
					{
						matFile.read((char*)&fileNameSizes[materialIndex][i], sizeof(fileNameSizes[materialIndex][i]));
					}

					for (int i = 0; i < 5; i++)
					{
						if (fileNameSizes[materialIndex][i] != NULL)
						{
							fileNames[materialIndex][i] = new char[fileNameSizes[materialIndex][i]];
						}
					}

					char tmp = NULL;
					for (int i = 0; i < 5; i++)
					{
						for (int j = 0; j < fileNameSizes[materialIndex][i]; j++)
						{
							fileNames[materialIndex][i][j] = 0;
						}
						if (fileNameSizes[materialIndex][i])
						{
							for (int j = 0; j < fileNameSizes[materialIndex][i]; j++)
							{
								matFile.read(&tmp, sizeof(tmp));
								if (tmp == '^')
								{
									break;
								}
								else
								{
									fileNames[materialIndex][i][j] = tmp;
								}
							}
						}
					}
				}
				else if (lamberts[materialIndex] == 0)
				{
					memcpy(&ambientColor[materialIndex], &matBuffer[0], sizeof(ambientColor));

					memcpy(&diffuseColor[materialIndex], &matBuffer[sizeof(ambientColor[materialIndex])], sizeof(diffuseColor[materialIndex]));

					memcpy(&specularColor[materialIndex], &matBuffer[sizeof(ambientColor[materialIndex]) + sizeof(diffuseColor[materialIndex])], sizeof(specularColor[materialIndex]));

					memcpy(&emmissiveColor[materialIndex], &matBuffer[sizeof(ambientColor[materialIndex]) + sizeof(diffuseColor[materialIndex]) + sizeof(specularColor[materialIndex])], sizeof(emmissiveColor[materialIndex]));

					memcpy(&dTransparencyOrShininess[materialIndex], &matBuffer[sizeof(ambientColor[materialIndex]) + sizeof(diffuseColor[materialIndex]) + sizeof(specularColor[materialIndex]) + sizeof(emmissiveColor[materialIndex])], sizeof(dTransparencyOrShininess[materialIndex]));

					for (int i = 0; i < 5; i++)
					{
						matFile.read((char*)&fileNameSizes[materialIndex][i], sizeof(fileNameSizes[materialIndex][i]));
					}

					for (int i = 0; i < 5; i++)
					{
						if (fileNameSizes[materialIndex][i] != NULL)
						{
							fileNames[materialIndex][i] = new char[fileNameSizes[materialIndex][i]];
						}
					}

					char tmp = NULL;
					for (int i = 0; i < 5; i++)
					{
						for (int j = 0; j < fileNameSizes[materialIndex][i]; j++)
						{
							fileNames[materialIndex][i][j] = 0;
						}
						if (fileNameSizes[materialIndex][i])
						{
							for (int j = 0; j < fileNameSizes[materialIndex][i]; j++)
							{
								matFile.read(&tmp, sizeof(tmp));
								if (tmp == '^')
								{
									break;
								}
								else
								{
									fileNames[materialIndex][i][j] = tmp;
								}
							}
						}
					}
				}

				delete[] matBuffer;
				matBuffer = nullptr;
#pragma endregion
				if (!materialIndex)//grab first material
				{
					tImportMe.vtMaterials[meshIndex].hasMaterial = hasMaterial;
					tImportMe.vtMaterials[meshIndex].dTransparencyOrShininess = dTransparencyOrShininess[materialIndex];
					tImportMe.vtMaterials[meshIndex].fMetallicness = fMetallicness[materialIndex];
					tImportMe.vtMaterials[meshIndex].fRoughness = fRoughness[materialIndex];
					tImportMe.vtMaterials[meshIndex].lambert = lamberts[materialIndex];
					tImportMe.vtMaterials[meshIndex].m_tAmbientColor = ambientColor[materialIndex];
					tImportMe.vtMaterials[meshIndex].m_tDiffuseColor = diffuseColor[materialIndex];
					tImportMe.vtMaterials[meshIndex].m_tEmissiveColor = emmissiveColor[materialIndex];
					tImportMe.vtMaterials[meshIndex].m_tSpecularColor = specularColor[materialIndex];;
					tImportMe.vtMaterials[meshIndex].m_tFileNames = fileNames[materialIndex];
					tImportMe.vtMaterials[meshIndex].m_tFileNameSizes = fileNameSizes[materialIndex];
					tImportMe.vtMaterials[meshIndex].m_tPBRFileNames = pbrfileNames[materialIndex];
					tImportMe.vtMaterials[meshIndex].m_tPBRFileNameSizes = pbrfileNameSizes[materialIndex];
				}
			}
		}

		matFile.read((char*)&hasPolygon, sizeof(int));
		if (hasPolygon)
		{
			#pragma region ReadMesh
			matFile.read((char*)&polyVertCount, sizeof(int));
			matFile.read((char*)&uniqueVertexCount, sizeof(int));
			matFile.read((char*)&worldTranslation[0], sizeof(double) * 3);
			matFile.read((char*)&worldRotation[0], sizeof(double) * 3);
			matFile.read((char*)&worldScaling[0], sizeof(double) * 3);
			int mySize = uniqueVertexCount * sizeof(TMeshFormat);
			char *buffer = new char[mySize];
			matFile.read(buffer, mySize);

			meshArray = new TMeshFormat[uniqueVertexCount];
			for (int i = 0; i < uniqueVertexCount; i++)
			{
				memcpy(&meshArray[i], &buffer[i * sizeof(TMeshFormat)], sizeof(TMeshFormat));
			}

			delete[] buffer;
			buffer = nullptr;
			indexBuf = new unsigned int[polyVertCount];
			mySize = polyVertCount * sizeof(unsigned int);
			buffer = new char[mySize];
			matFile.read(buffer, mySize);
			for (int i = 0; i < polyVertCount; i++)
			{
				memcpy(&indexBuf[i], &buffer[i * sizeof(unsigned int)], sizeof(unsigned int));
			}
			delete[] buffer;
			buffer = nullptr;
#pragma endregion
		}
		tImportMe.vtMeshes[meshIndex].hasPolygon = hasPolygon;
		tImportMe.vtMeshes[meshIndex].nUniqueVertexCount = uniqueVertexCount;
		tImportMe.vtMeshes[meshIndex].nPolygonVertexCount = polyVertCount;
		tImportMe.vtMeshes[meshIndex].meshArrays = meshArray;
		tImportMe.vtMeshes[meshIndex].indexBuffer = indexBuf;
		tImportMe.vtAnimations[meshIndex].invBindPosesForJoints = invBindPosesForJoints;
		for (int i = 0; i < 3; i++)
		{
			tImportMe.vtMeshes[meshIndex].worldTranslation[i] = worldTranslation[i];
			tImportMe.vtMeshes[meshIndex].worldRotation[i] = worldRotation[i];
			tImportMe.vtMeshes[meshIndex].worldScaling[i] = worldScaling[i];
		}
	}
	matFile.close();

	return tImportMe;
}

XMVECTOR CGraphicsSystem::GetCameraPos()
{
	XMVECTOR campos;
	campos.m128_f32[0] = m_fCameraXPosition;
	campos.m128_f32[1] = m_fCameraYPosition;
	campos.m128_f32[2] = m_fCameraZPosition;
	return campos;
}



void CGraphicsSystem::InitPrimalShaderData(ID3D11DeviceContext * pd3dDeviceContext, XMMATRIX d3dWorldMatrix, XMMATRIX d3dViewMatrix, XMMATRIX d3dProjectionMatrix, TDebugMesh tDebugMesh, XMMATRIX CameraMatrix)
{
	D3D11_MAPPED_SUBRESOURCE d3dPrimalMappedResource;
	D3D11_MAPPED_SUBRESOURCE d3dPrimalPixelMappedResource;

	TPrimalVertexBufferType* ptPrimalMatrixBufferDataPointer = nullptr;
	TPrimalPixelBufferType	*ptPrimalPixelBufferDataPointer = nullptr;

	unsigned int bufferNumber;

	d3dViewMatrix = XMMatrixInverse(nullptr, CameraMatrix);

	XMMATRIX d3dView;

	d3dView = d3dViewMatrix;

	pd3dDeviceContext->Map(m_pd3dPrimalVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dPrimalMappedResource);

	// Get a pointer to the data in the constant buffer.
	ptPrimalMatrixBufferDataPointer = (TPrimalVertexBufferType*)d3dPrimalMappedResource.pData;

	// Copy the matrices into the constant buffer.
	ptPrimalMatrixBufferDataPointer->m_d3dWorldMatrix = d3dWorldMatrix;
	ptPrimalMatrixBufferDataPointer->m_d3dViewMatrix = d3dView;
	ptPrimalMatrixBufferDataPointer->m_d3dProjectionMatrix = d3dProjectionMatrix;

	// Unlock the constant buffer.
	pd3dDeviceContext->Unmap(m_pd3dPrimalVertexBuffer, 0);

#pragma region Map To Pixel Constant Buffer
	pd3dDeviceContext->Map(m_pd3dPrimalPixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dPrimalPixelMappedResource);

	// Get a pointer to the data in the constant buffer.
	ptPrimalPixelBufferDataPointer = (TPrimalPixelBufferType*)d3dPrimalPixelMappedResource.pData;

	// Copy the matrices into the constant buffer.
	ptPrimalPixelBufferDataPointer->m_d3dCollisionColor = XMFLOAT4(0, 0, 0, 0);

	// Unlock the constant buffer.
	pd3dDeviceContext->Unmap(m_pd3dPrimalPixelBuffer, 0);

#pragma endregion

	// Position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	pd3dDeviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_pd3dPrimalPixelBuffer);
	// Set the constant buffer in the vertex shader with the updated values.
	pd3dDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_pd3dPrimalVertexBuffer);
	pd3dDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	pd3dDeviceContext->IASetVertexBuffers(0, 1, &tDebugMesh.m_pd3dVertexBuffer, &tDebugMesh.m_nVertexBufferStride, &tDebugMesh.m_nVertexBufferOffset);
}

void CGraphicsSystem::InitPrimalShaderData2(ID3D11DeviceContext * pd3dDeviceContext, TPrimalVertexBufferType d3dVertexBuffer, TPrimalPixelBufferType d3dPixelBuffer, TSimpleMesh tSimpleMesh, XMMATRIX CameraMatrix)
{
	D3D11_MAPPED_SUBRESOURCE d3dPrimalVertexMappedResource;
	D3D11_MAPPED_SUBRESOURCE d3dPrimalPixelMappedResource;

	TPrimalVertexBufferType	*ptPrimalVertexBufferDataPointer = nullptr;
	TPrimalPixelBufferType	*ptPrimalPixelBufferDataPointer = nullptr;

	d3dVertexBuffer.m_d3dViewMatrix = XMMatrixInverse(nullptr, CameraMatrix);
	unsigned int bufferNumber;
	//XMMATRIX d3dTmpViewM;

	//d3dTmpViewM = XMMatrixInverse(NULL, );
	m_fCameraXPosition = CameraMatrix.r[3].m128_f32[0];// d3dTmpViewM.r[3].m128_f32[0];
	m_fCameraYPosition = CameraMatrix.r[3].m128_f32[1];//d3dTmpViewM.r[3].m128_f32[1];
	m_fCameraZPosition = CameraMatrix.r[3].m128_f32[2];//d3dTmpViewM.r[3].m128_f32[2];

	XMMATRIX d3dView;
	XMMATRIX tempWorld = d3dVertexBuffer.m_d3dWorldMatrix;
	XMMATRIX tempProj = d3dVertexBuffer.m_d3dProjectionMatrix;
	d3dView = d3dVertexBuffer.m_d3dViewMatrix;

#pragma region Map To Vertex Constant Buffer
	pd3dDeviceContext->Map(m_pd3dPrimalVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dPrimalVertexMappedResource);

	// Get a pointer to the data in the constant buffer.
	ptPrimalVertexBufferDataPointer = (TPrimalVertexBufferType*)d3dPrimalVertexMappedResource.pData;

	// Copy the matrices into the constant buffer.
	ptPrimalVertexBufferDataPointer->m_d3dWorldMatrix = tempWorld;
	ptPrimalVertexBufferDataPointer->m_d3dViewMatrix = d3dView;
	ptPrimalVertexBufferDataPointer->m_d3dProjectionMatrix = tempProj;;

	// Unlock the constant buffer.
	pd3dDeviceContext->Unmap(m_pd3dPrimalVertexBuffer, 0);

#pragma endregion

#pragma region Map To Pixel Constant Buffer
	pd3dDeviceContext->Map(m_pd3dPrimalPixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dPrimalPixelMappedResource);

	// Get a pointer to the data in the constant buffer.
	ptPrimalPixelBufferDataPointer = (TPrimalPixelBufferType*)d3dPrimalPixelMappedResource.pData;

	// Copy the matrices into the constant buffer.
	ptPrimalPixelBufferDataPointer->m_d3dCollisionColor = d3dPixelBuffer.m_d3dCollisionColor;

	// Unlock the constant buffer.
	pd3dDeviceContext->Unmap(m_pd3dPrimalPixelBuffer, 0);

#pragma endregion

	// Position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	pd3dDeviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_pd3dPrimalPixelBuffer);
	// Set the constant buffer in the vertex shader with the updated values.
	pd3dDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_pd3dPrimalVertexBuffer);

	pd3dDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dDeviceContext->IASetVertexBuffers(0, 1, &tSimpleMesh.m_pd3dVertexBuffer, &tSimpleMesh.m_nVertexBufferStride, &tSimpleMesh.m_nVertexBufferOffset);
	pd3dDeviceContext->IASetIndexBuffer(tSimpleMesh.m_pd3dIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
}

void CGraphicsSystem::InitPrimalShaderData3(ID3D11DeviceContext * pd3dDeviceContext, TPrimalVertexBufferType d3dVertexBuffer, TPrimalPixelBufferType d3dPixelBuffer, TMesh tMesh, XMMATRIX CameraMatrix)
{
	D3D11_MAPPED_SUBRESOURCE d3dPrimalVertexMappedResource;
	D3D11_MAPPED_SUBRESOURCE d3dPrimalPixelMappedResource;

	TPrimalVertexBufferType	*ptPrimalVertexBufferDataPointer = nullptr;
	TPrimalPixelBufferType	*ptPrimalPixelBufferDataPointer = nullptr;

	d3dVertexBuffer.m_d3dViewMatrix = XMMatrixInverse(nullptr, CameraMatrix);
	unsigned int bufferNumber;
	//XMMATRIX d3dTmpViewM;

	//d3dTmpViewM = XMMatrixInverse(NULL, );
	m_fCameraXPosition = CameraMatrix.r[3].m128_f32[0];// d3dTmpViewM.r[3].m128_f32[0];
	m_fCameraYPosition = CameraMatrix.r[3].m128_f32[1];//d3dTmpViewM.r[3].m128_f32[1];
	m_fCameraZPosition = CameraMatrix.r[3].m128_f32[2];//d3dTmpViewM.r[3].m128_f32[2];

	XMMATRIX d3dView;
	XMMATRIX tempWorld = d3dVertexBuffer.m_d3dWorldMatrix;
	XMMATRIX tempProj = d3dVertexBuffer.m_d3dProjectionMatrix;
	d3dView = d3dVertexBuffer.m_d3dViewMatrix;

#pragma region Map To Vertex Constant Buffer
	pd3dDeviceContext->Map(m_pd3dPrimalVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dPrimalVertexMappedResource);

	// Get a pointer to the data in the constant buffer.
	ptPrimalVertexBufferDataPointer = (TPrimalVertexBufferType*)d3dPrimalVertexMappedResource.pData;

	// Copy the matrices into the constant buffer.
	ptPrimalVertexBufferDataPointer->m_d3dWorldMatrix = tempWorld;
	ptPrimalVertexBufferDataPointer->m_d3dViewMatrix = d3dView;
	ptPrimalVertexBufferDataPointer->m_d3dProjectionMatrix = tempProj;;

	// Unlock the constant buffer.
	pd3dDeviceContext->Unmap(m_pd3dPrimalVertexBuffer, 0);

#pragma endregion

#pragma region Map To Pixel Constant Buffer
	pd3dDeviceContext->Map(m_pd3dPrimalPixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dPrimalPixelMappedResource);

	// Get a pointer to the data in the constant buffer.
	ptPrimalPixelBufferDataPointer = (TPrimalPixelBufferType*)d3dPrimalPixelMappedResource.pData;

	// Copy the matrices into the constant buffer.
	ptPrimalPixelBufferDataPointer->m_d3dCollisionColor = d3dPixelBuffer.m_d3dCollisionColor;

	// Unlock the constant buffer.
	pd3dDeviceContext->Unmap(m_pd3dPrimalPixelBuffer, 0);

#pragma endregion

	// Position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	pd3dDeviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_pd3dPrimalPixelBuffer);
	// Set the constant buffer in the vertex shader with the updated values.
	pd3dDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_pd3dPrimalVertexBuffer);

	pd3dDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dDeviceContext->IASetVertexBuffers(0, 1, &tMesh.m_pd3dVertexBuffer, &tMesh.m_nVertexBufferStride, &tMesh.m_nVertexBufferOffset);
	pd3dDeviceContext->IASetIndexBuffer(tMesh.m_pd3dIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
}

void CGraphicsSystem::InitSkyboxShaderData(ID3D11DeviceContext * pd3dDeviceContext, TMyVertexBufferType d3dVertexBuffer, TMesh tMesh, XMMATRIX CameraMatrix)
{
	D3D11_MAPPED_SUBRESOURCE d3dSkyboxVertexMappedResource;

	TMyVertexBufferType	*ptSkyboxVertexBufferDataPointer = nullptr;

	XMMATRIX d3dView;
	d3dView = d3dVertexBuffer.m_d3dViewMatrix;
	d3dView = XMMatrixInverse(nullptr, CameraMatrix);
	unsigned int bufferNumber = 0;
	m_fCameraXPosition = CameraMatrix.r[3].m128_f32[0];
	m_fCameraYPosition = CameraMatrix.r[3].m128_f32[1];
	m_fCameraZPosition = CameraMatrix.r[3].m128_f32[2];

	XMMATRIX tempWorld = XMMatrixIdentity();
	XMMATRIX tempProj = d3dVertexBuffer.m_d3dProjectionMatrix;
	tempWorld = XMMatrixTranslation(m_fCameraXPosition, m_fCameraYPosition, m_fCameraZPosition);

	#pragma region Map To Vertex Constant Buffer
	pd3dDeviceContext->Map(m_pd3dSkyboxVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dSkyboxVertexMappedResource);

	// Get a pointer to the data in the constant buffer.
	ptSkyboxVertexBufferDataPointer = (TMyVertexBufferType*)d3dSkyboxVertexMappedResource.pData;

	// Copy the matrices into the constant buffer.
	ptSkyboxVertexBufferDataPointer->m_d3dWorldMatrix = tempWorld;
	ptSkyboxVertexBufferDataPointer->m_d3dViewMatrix = d3dView;
	ptSkyboxVertexBufferDataPointer->m_d3dProjectionMatrix = tempProj;

	// Unlock the constant buffer.
	pd3dDeviceContext->Unmap(m_pd3dSkyboxVertexBuffer, 0);
#pragma endregion

	pd3dDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_pd3dSkyboxVertexBuffer);
	pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dDeviceContext->IASetVertexBuffers(0, 1, &tMesh.m_pd3dVertexBuffer, &tMesh.m_nVertexBufferStride, &tMesh.m_nVertexBufferOffset);
	pd3dDeviceContext->IASetIndexBuffer(tMesh.m_pd3dIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	if (&tMesh.m_d3dSRVDiffuse != NULL)
	{
		pd3dDeviceContext->PSSetShaderResources(0, 1, &tMesh.m_d3dSRVDiffuse);
	}
}

void CGraphicsSystem::InitAnimShaderData(ID3D11DeviceContext * pd3dDeviceContext, TAnimatedVertexBufferType d3dVertexBuffer, TMesh tMesh, XMMATRIX CameraMatrix)
{
	D3D11_MAPPED_SUBRESOURCE d3dAnimatedVertexMappedResource;

	TAnimatedVertexBufferType *ptAnimatedVertexBufferDataPointer = nullptr;

	XMMATRIX d3dView;
	d3dView = d3dVertexBuffer.m_d3dViewMatrix;
	d3dView = XMMatrixInverse(nullptr, CameraMatrix);
	unsigned int bufferNumber = 0;
	m_fCameraXPosition = CameraMatrix.r[3].m128_f32[0];
	m_fCameraYPosition = CameraMatrix.r[3].m128_f32[1];
	m_fCameraZPosition = CameraMatrix.r[3].m128_f32[2];

	XMMATRIX tempWorld = d3dVertexBuffer.m_d3dWorldMatrix;
	XMMATRIX tempProj = d3dVertexBuffer.m_d3dProjectionMatrix;

	#pragma region Map To Vertex Constant Buffer
	pd3dDeviceContext->Map(m_pd3dAnimatedVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dAnimatedVertexMappedResource);

	// Get a pointer to the data in the constant buffer.
	ptAnimatedVertexBufferDataPointer = (TAnimatedVertexBufferType*)d3dAnimatedVertexMappedResource.pData;

	// Copy the matrices into the constant buffer.
	ptAnimatedVertexBufferDataPointer->m_d3dWorldMatrix = tempWorld;
	ptAnimatedVertexBufferDataPointer->m_d3dViewMatrix = d3dView;
	ptAnimatedVertexBufferDataPointer->m_d3dProjectionMatrix = tempProj;
	memcpy(&ptAnimatedVertexBufferDataPointer->m_d3dJointsForVS, &d3dVertexBuffer.m_d3dJointsForVS, sizeof(d3dVertexBuffer.m_d3dJointsForVS));
	// Unlock the constant buffer.
	pd3dDeviceContext->Unmap(m_pd3dAnimatedVertexBuffer, 0);

#pragma endregion

	pd3dDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_pd3dAnimatedVertexBuffer);
	pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dDeviceContext->IASetVertexBuffers(0, 1, &tMesh.m_pd3dVertexBuffer, &tMesh.m_nVertexBufferStride, &tMesh.m_nVertexBufferOffset);
	pd3dDeviceContext->IASetIndexBuffer(tMesh.m_pd3dIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	if (&tMesh.m_d3dSRVDiffuse != NULL)
	{
		pd3dDeviceContext->PSSetShaderResources(0, 1, &tMesh.m_d3dSRVDiffuse);
	}
}

void CGraphicsSystem::InitMyShaderData(ID3D11DeviceContext * pd3dDeviceContext, TMyVertexBufferType d3dVertexBuffer, TMesh tMesh, XMMATRIX CameraMatrix)
{
	D3D11_MAPPED_SUBRESOURCE d3dMyVertexMappedResource;

	TMyVertexBufferType	*ptMyVertexBufferDataPointer = nullptr;

	XMMATRIX d3dView;
	d3dView = d3dVertexBuffer.m_d3dViewMatrix;
	d3dView = XMMatrixInverse(nullptr, CameraMatrix);
	unsigned int bufferNumber = 0;
	m_fCameraXPosition = CameraMatrix.r[3].m128_f32[0];
	m_fCameraYPosition = CameraMatrix.r[3].m128_f32[1];
	m_fCameraZPosition = CameraMatrix.r[3].m128_f32[2];

	XMMATRIX tempWorld = d3dVertexBuffer.m_d3dWorldMatrix;
	XMMATRIX tempProj = d3dVertexBuffer.m_d3dProjectionMatrix;

#pragma region Map To Vertex Constant Buffer
	pd3dDeviceContext->Map(m_pd3dMyVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMyVertexMappedResource);

	// Get a pointer to the data in the constant buffer.
	ptMyVertexBufferDataPointer = (TMyVertexBufferType*)d3dMyVertexMappedResource.pData;

	// Copy the matrices into the constant buffer.
	ptMyVertexBufferDataPointer->m_d3dWorldMatrix = tempWorld;
	ptMyVertexBufferDataPointer->m_d3dViewMatrix = d3dView;
	ptMyVertexBufferDataPointer->m_d3dProjectionMatrix = tempProj;

	// Unlock the constant buffer.
	pd3dDeviceContext->Unmap(m_pd3dMyVertexBuffer, 0);

#pragma endregion

	pd3dDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_pd3dMyVertexBuffer);
	pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dDeviceContext->IASetVertexBuffers(0, 1, &tMesh.m_pd3dVertexBuffer, &tMesh.m_nVertexBufferStride, &tMesh.m_nVertexBufferOffset);
	pd3dDeviceContext->IASetIndexBuffer(tMesh.m_pd3dIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	if (&tMesh.m_d3dSRVDiffuse != NULL)
	{
		pd3dDeviceContext->PSSetShaderResources(0, 1, &tMesh.m_d3dSRVDiffuse);
	}
}

void CGraphicsSystem::InitUIShaderData(ID3D11DeviceContext * pd3dDeviceContext, TUIVertexBufferType d3dVertexBuffer, TUIPixelBufferType d3dPixelBuffer, TMesh tMesh, XMMATRIX CameraMatrix)
{
	D3D11_MAPPED_SUBRESOURCE d3dUIVertexMappedResource;
	D3D11_MAPPED_SUBRESOURCE d3dUIPixelMappedResource;

	TUIVertexBufferType	*ptUIVertexBufferDataPointer = nullptr;
	TUIPixelBufferType	*ptUIPixelBufferDataPointer = nullptr;

	//XMMATRIX d3dView;
	//d3dView = d3dVertexBuffer.m_d3dViewMatrix;
	//d3dView = XMMatrixInverse(nullptr, CameraMatrix);

	unsigned int vBufferNumber = 0;
	unsigned int pBufferNumber = 0;
	
	//XMMATRIX tempWorld = d3dVertexBuffer.m_d3dWorldMatrix;
	//XMMATRIX tempProj = d3dVertexBuffer.m_d3dProjectionMatrix;

	#pragma region Map To Vertex Constant Buffer
	pd3dDeviceContext->Map(m_pd3dUIVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dUIVertexMappedResource);


	// Get a pointer to the data in the constant buffer.
	ptUIVertexBufferDataPointer = (TUIVertexBufferType*)d3dUIVertexMappedResource.pData;

	// Copy the matrices into the constant buffer.
	//ptUIVertexBufferDataPointer->m_d3dWorldMatrix = tempWorld;
	//ptUIVertexBufferDataPointer->m_d3dViewMatrix = d3dView;
	//ptUIVertexBufferDataPointer->m_d3dProjectionMatrix = tempProj;

	ptUIVertexBufferDataPointer->start = d3dVertexBuffer.start;
	ptUIVertexBufferDataPointer->end = d3dVertexBuffer.end;
	ptUIVertexBufferDataPointer->ratio = d3dVertexBuffer.ratio;
	ptUIVertexBufferDataPointer->padding = d3dVertexBuffer.padding;

	// Unlock the constant buffer.
	pd3dDeviceContext->Unmap(m_pd3dUIVertexBuffer, 0);

#pragma endregion

	#pragma region Map To Pixel Constant Buffer
	pd3dDeviceContext->Map(m_pd3dUIPixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dUIPixelMappedResource);


	// Get a pointer to the data in the constant buffer.
	ptUIPixelBufferDataPointer = (TUIPixelBufferType*)d3dUIPixelMappedResource.pData;

	// Copy the matrices into the constant buffer.
	ptUIPixelBufferDataPointer->hoverColor = d3dPixelBuffer.hoverColor;

	// Unlock the constant buffer.
	pd3dDeviceContext->Unmap(m_pd3dUIPixelBuffer, 0);

#pragma endregion	

	pd3dDeviceContext->VSSetConstantBuffers(vBufferNumber, 1, &m_pd3dUIVertexBuffer);
	pd3dDeviceContext->PSSetConstantBuffers(pBufferNumber, 1, &m_pd3dUIPixelBuffer);
	pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dDeviceContext->IASetVertexBuffers(0, 1, &tMesh.m_pd3dVertexBuffer, &tMesh.m_nVertexBufferStride, &tMesh.m_nVertexBufferOffset);
	pd3dDeviceContext->IASetIndexBuffer(tMesh.m_pd3dIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	if (&tMesh.m_d3dSRVDiffuse != NULL)
	{
		pd3dDeviceContext->PSSetShaderResources(0, 1, &tMesh.m_d3dSRVDiffuse);
	}
}

void CGraphicsSystem::InitQuadShaderData(ID3D11DeviceContext * pd3dDeviceContext, XMMATRIX d3dWorldMatrix, XMMATRIX d3dViewMatrix, XMMATRIX d3dProjectionMatrix, TDebugMesh tDebugMesh, XMMATRIX CameraMatrix, XMFLOAT4 BackgroundColor, float fHealth)
{
	D3D11_MAPPED_SUBRESOURCE d3dQuadMappedResource;
	D3D11_MAPPED_SUBRESOURCE d3dQuadPixelMappedResource;

	TQuadGeometryBufferType* ptQuadMatrixBufferDataPointer = nullptr;
	TQuadPixelBufferType* ptQuadPixelBufferDataPointer = nullptr;


	unsigned int bufferNumber;

	d3dViewMatrix = XMMatrixInverse(nullptr, CameraMatrix);

	XMMATRIX d3dView;

	d3dView = d3dViewMatrix;

	#pragma region Map To Geometry Constant Buffer

	pd3dDeviceContext->Map(m_pd3dQuadGeometryBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dQuadMappedResource);

	// Get a pointer to the data in the constant buffer.
	ptQuadMatrixBufferDataPointer = (TQuadGeometryBufferType*)d3dQuadMappedResource.pData;

	// Copy the matrices into the constant buffer.
	ptQuadMatrixBufferDataPointer->m_d3dWorldMatrix = d3dWorldMatrix;
	ptQuadMatrixBufferDataPointer->m_d3dViewMatrix = d3dView;
	ptQuadMatrixBufferDataPointer->m_d3dProjectionMatrix = d3dProjectionMatrix;
	ptQuadMatrixBufferDataPointer->m_fHealth = fHealth;
	// Unlock the constant buffer.
	pd3dDeviceContext->Unmap(m_pd3dQuadGeometryBuffer, 0);

#pragma endregion

	#pragma region Map To Pixel Constant Buffer
	pd3dDeviceContext->Map(m_pd3dQuadPixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dQuadPixelMappedResource);

	// Get a pointer to the data in the constant buffer.
	ptQuadPixelBufferDataPointer = (TQuadPixelBufferType*)d3dQuadPixelMappedResource.pData;

	// Copy the matrices into the constant buffer.
	ptQuadPixelBufferDataPointer->m_d3dBackgroundColor = BackgroundColor;

	// Unlock the constant buffer.
	pd3dDeviceContext->Unmap(m_pd3dQuadPixelBuffer, 0);

#pragma endregion

	// Position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	pd3dDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	pd3dDeviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_pd3dQuadPixelBuffer);

	// Set the constant buffer in the Geometry shader with the updated values.
	pd3dDeviceContext->GSSetConstantBuffers(0, 1, &m_pd3dQuadGeometryBuffer);
	pd3dDeviceContext->IASetVertexBuffers(0, 1, &tDebugMesh.m_pd3dVertexBuffer, &tDebugMesh.m_nVertexBufferStride, &tDebugMesh.m_nVertexBufferOffset);
}

void CGraphicsSystem::InitLineShaderData(ID3D11DeviceContext * pd3dDeviceContext, XMMATRIX d3dWorldMatrix, XMMATRIX d3dViewMatrix, XMMATRIX d3dProjectionMatrix, TDebugMesh tDebugMesh, XMMATRIX CameraMatrix, float colorAlpha, XMFLOAT4 endPoint)
{
	D3D11_MAPPED_SUBRESOURCE d3dLineMappedResource;
	D3D11_MAPPED_SUBRESOURCE d3dLinePixelMappedResource;

	TLineGeometryBufferType* ptLineMatrixBufferDataPointer = nullptr;
	TQuadPixelBufferType* ptLinePixelBufferDataPointer = nullptr;


	unsigned int bufferNumber;

	d3dViewMatrix = XMMatrixInverse(nullptr, CameraMatrix);

	XMMATRIX d3dView;

	d3dView = d3dViewMatrix;

#pragma region Map To Geometry Constant Buffer

	pd3dDeviceContext->Map(m_pd3dQuadGeometryBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dLineMappedResource);

	// Get a pointer to the data in the constant buffer.
	ptLineMatrixBufferDataPointer = (TLineGeometryBufferType*)d3dLineMappedResource.pData;

	// Copy the matrices into the constant buffer.
	ptLineMatrixBufferDataPointer->m_d3dWorldMatrix = d3dWorldMatrix;
	ptLineMatrixBufferDataPointer->m_d3dViewMatrix = d3dView;
	ptLineMatrixBufferDataPointer->m_d3dProjectionMatrix = d3dProjectionMatrix;
	ptLineMatrixBufferDataPointer->endPoint = endPoint;
	// Unlock the constant buffer.
	pd3dDeviceContext->Unmap(m_pd3dQuadGeometryBuffer, 0);

#pragma endregion

#pragma region Map To Pixel Constant Buffer
	pd3dDeviceContext->Map(m_pd3dQuadPixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dLinePixelMappedResource);

	// Get a pointer to the data in the constant buffer.
	ptLinePixelBufferDataPointer = (TQuadPixelBufferType*)d3dLinePixelMappedResource.pData;

	// Copy the matrices into the constant buffer.
	ptLinePixelBufferDataPointer->m_d3dBackgroundColor = XMFLOAT4(0,1,1,colorAlpha);

	// Unlock the constant buffer.
	pd3dDeviceContext->Unmap(m_pd3dQuadPixelBuffer, 0);

#pragma endregion

	// Position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	pd3dDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	pd3dDeviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_pd3dQuadPixelBuffer);

	// Set the constant buffer in the Geometry shader with the updated values.
	pd3dDeviceContext->GSSetConstantBuffers(0, 1, &m_pd3dLineGeometryBuffer);
	pd3dDeviceContext->IASetVertexBuffers(0, 1, &tDebugMesh.m_pd3dVertexBuffer, &tDebugMesh.m_nVertexBufferStride, &tDebugMesh.m_nVertexBufferOffset);
}

enum
{
	Mesh = 1,
	DebugMesh,
	SimpleMesh,
	PhongMesh,
	LambertMesh,
	PBRMesh,
	UIMesh,
	Quad,
	Skybox,
	Glass,
	Projectile,
	AnimatedMesh,
	Line,

};

void CGraphicsSystem::ExecutePipeline(ID3D11DeviceContext *pd3dDeviceContext, int m_nIndexCount, int nGraphicsMask, int nShaderID)
{
	switch (nShaderID)
	{
		case Mesh:
		{
			//Set Input_Layout
			pd3dDeviceContext->IASetInputLayout(m_pd3dMyInputLayout);
			//Set Shader
			pd3dDeviceContext->VSSetShader(m_pd3dMyVertexShader, NULL, 0);
			pd3dDeviceContext->PSSetShader(m_pd3dMyPixelShader, NULL, 0);
			////Draw
			if (nGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
			{
				pd3dDeviceContext->DrawIndexed(m_nIndexCount, 0, 0);
			}
			break;
		}
		case DebugMesh:
		{
			//Set Input_Layout
			pd3dDeviceContext->IASetInputLayout(m_pd3dPrimalInputLayout);
			//Set Shader
			pd3dDeviceContext->VSSetShader(m_pd3dPrimalVertexShader, NULL, 0);
			pd3dDeviceContext->PSSetShader(m_pd3dPrimalPixelShader, NULL, 0);
			//Draw
			if (nGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
			{
				pd3dDeviceContext->Draw(m_nIndexCount, 0);
			}
			break;
		}
		case SimpleMesh:
		{
			pd3dDeviceContext->IASetInputLayout(m_pd3dPrimalInputLayout);
			//Set Shader
			pd3dDeviceContext->VSSetShader(m_pd3dPrimalVertexShader, NULL, 0);
			pd3dDeviceContext->GSSetShader(nullptr, NULL, 0);
			pd3dDeviceContext->PSSetShader(m_pd3dPrimalPixelShader, NULL, 0);
			//Draw
			if (nGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID))
			{
				pd3dDeviceContext->DrawIndexed(m_nIndexCount, 0, 0);
			}
			break;
		}
		case PhongMesh:
		{
			//Set Input_Layout
			pd3dDeviceContext->IASetInputLayout(m_pd3dMyInputLayout);
			//Set Shader
			pd3dDeviceContext->VSSetShader(m_pd3dMyVertexShader, NULL, 0);
			pd3dDeviceContext->GSSetShader(nullptr, NULL, 0);
			pd3dDeviceContext->PSSetShader(m_pd3dMyPixelShader, NULL, 0);
			////Draw
			if (nGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
			{
				pd3dDeviceContext->DrawIndexed(m_nIndexCount, 0, 0);
			}
			break;
		}
		case LambertMesh:
		{
			//Set Input_Layout
			pd3dDeviceContext->IASetInputLayout(m_pd3dMyInputLayout);
			//Set Shader
			pd3dDeviceContext->VSSetShader(m_pd3dMyVertexShader, NULL, 0);
			pd3dDeviceContext->GSSetShader(nullptr, NULL, 0);
			pd3dDeviceContext->PSSetShader(m_pd3dMyPixelShader, NULL, 0);
			////Draw
			if (nGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
			{
				pd3dDeviceContext->DrawIndexed(m_nIndexCount, 0, 0);
			}
			else if (nGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID))
			{
				pd3dDeviceContext->DrawIndexed(m_nIndexCount, 0, 0);
			}
			break;
		}
		case PBRMesh:
		{
			//Set Input_Layout
			pd3dDeviceContext->IASetInputLayout(m_pd3dMyInputLayout);
			//Set Shader
			pd3dDeviceContext->VSSetShader(m_pd3dMyVertexShader, NULL, 0);
			pd3dDeviceContext->PSSetShader(m_pd3dMyPixelShader, NULL, 0);
			////Draw
			if (nGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
			{
				pd3dDeviceContext->DrawIndexed(m_nIndexCount, 0, 0);
			}
			break;
		}	
		case UIMesh:
		{
			//Set Input_Layout
			pd3dDeviceContext->IASetInputLayout(m_pd3dUIInputLayout);
			//Set Shader
			pd3dDeviceContext->VSSetShader(m_pd3dUIVertexShader, NULL, 0);
			pd3dDeviceContext->PSSetShader(m_pd3dUIPixelShader, NULL, 0);
			////Draw
			if (nGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
			{
				pd3dDeviceContext->DrawIndexed(m_nIndexCount, 0, 0);
			}
			else if (nGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_SHADERID))
				pd3dDeviceContext->DrawIndexed(m_nIndexCount, 0, 0);
			break;
		}
		case Quad:
		{
			//Set Input_Layout
			pd3dDeviceContext->IASetInputLayout(m_pd3dQuadInputLayout);
			//Set Shader
			pd3dDeviceContext->VSSetShader(m_pd3dQuadVertexShader, NULL, 0);
			pd3dDeviceContext->GSSetShader(m_pd3dQuadGeometryShader, NULL, 0);
			pd3dDeviceContext->PSSetShader(m_pd3dQuadPixelShader, NULL, 0);
			//Draw
			if (nGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
			{
				pd3dDeviceContext->Draw(m_nIndexCount, 0);
			}
			pd3dDeviceContext->GSSetShader(nullptr, NULL, 0);
			break;
		}
		case Skybox:
		{
			pd3dDeviceContext->RSSetState(m_pd3dNoCullRasterizerState);
			pd3dDeviceContext->PSSetSamplers(0, 1, &m_pd3dSamplerState);
			//Set Input_Layout
			pd3dDeviceContext->IASetInputLayout(m_pd3dSkyboxInputLayout);
			//Set Shader
			pd3dDeviceContext->VSSetShader(m_pd3dSkyboxVertexShader, NULL, 0);
			pd3dDeviceContext->PSSetShader(m_pd3dSkyboxPixelShader, NULL, 0);

			//Draw
			if (nGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_SKYBOX | COMPONENT_TEXTURE | COMPONENT_SHADERID))
			{
				pd3dDeviceContext->DrawIndexed(m_nIndexCount, 0, 0);
			}
			//pd3dDeviceContext->RSSetState(m_pd3dCullRasterizerState);
			//pd3dDeviceContext->PSSetSamplers(0, 1, NULL);

			break;
		}
		case Glass:
		{
			//Set Input_Layout
			pd3dDeviceContext->IASetInputLayout(m_pd3dMyInputLayout);
			//Set Shader
			pd3dDeviceContext->VSSetShader(m_pd3dMyVertexShader, NULL, 0);
			pd3dDeviceContext->PSSetShader(m_pd3dMyPixelShader, NULL, 0);
			////Draw
			if (nGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
			{
				pd3dDeviceContext->DrawIndexed(m_nIndexCount, 0, 0);
			}
			break;
		}
		case Projectile:
		{
			//Set Input_Layout
			pd3dDeviceContext->IASetInputLayout(m_pd3dMyInputLayout);
			//Set Shader
			pd3dDeviceContext->VSSetShader(m_pd3dMyVertexShader, NULL, 0);
			pd3dDeviceContext->PSSetShader(m_pd3dMyPixelShader, NULL, 0);
			////Draw
			if (nGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_SHADERID))
			{
				pd3dDeviceContext->DrawIndexed(m_nIndexCount, 0, 0);
			}
			break;
		}
		case AnimatedMesh:
		{
			//Set Input_Layout
			pd3dDeviceContext->IASetInputLayout(m_pd3dAnimatedInputLayout);
			//Set Shader
			pd3dDeviceContext->VSSetShader(m_pd3dAnimatedVertexShader, NULL, 0);
			pd3dDeviceContext->PSSetShader(m_pd3dAnimatedPixelShader, NULL, 0);
			////Draw
			if (nGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_MESH | COMPONENT_TEXTURE | COMPONENT_ANIMATION | COMPONENT_SHADERID))
			{
				pd3dDeviceContext->DrawIndexed(m_nIndexCount, 0, 0);
			}
			break;
		}
		//case Line:
		//{
		//	pd3dDeviceContext->IASetInputLayout(m_pd3dQuadInputLayout);
		//	//Set Shader
		//	pd3dDeviceContext->VSSetShader(m_pd3dQuadVertexShader, NULL, 0);
		//	pd3dDeviceContext->GSSetShader(m_pd3dLineGeometryShader, NULL, 0);

		//	pd3dDeviceContext->PSSetShader(m_pd3dQuadPixelShader, NULL, 0);
		//	//Draw
		//	if (nGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
		//	{
		//		pd3dDeviceContext->Draw(m_nIndexCount, 0);
		//	}
		//	//Don't want every object to go through geometry shader
		//	pd3dDeviceContext->GSSetShader(nullptr, NULL, 0);
		//	break;
		//}
		default:
			break;
	}
}