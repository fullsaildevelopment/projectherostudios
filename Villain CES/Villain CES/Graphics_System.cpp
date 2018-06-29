#include "stdafx.h"
#include "Graphics_System.h"

CGraphicsSystem::CGraphicsSystem()
{
}

CGraphicsSystem::~CGraphicsSystem()
{
}

void CGraphicsSystem::InitD3D(HWND cTheWindow)
{
	#pragma region Window Stuff

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
	d3dSwapchainDescription.OutputWindow = cTheWindow;                                // the window to be used
	d3dSwapchainDescription.SampleDesc.Count = 1;                               // how many multisamples
	d3dSwapchainDescription.Windowed = TRUE;                                    // windowed/full-screen mode
	d3dSwapchainDescription.BufferDesc.Width = cRectangle.right - cRectangle.left;
	d3dSwapchainDescription.BufferDesc.Height = cRectangle.bottom - cRectangle.top;
	d3dSwapchainDescription.BufferDesc.RefreshRate.Numerator = 60;
	d3dSwapchainDescription.BufferDesc.RefreshRate.Denominator = 1;
	unsigned int nDeviceAndSwapchainFlag = 0;
#ifdef _DEBUG
	flag = D3D11_CREATE_DEVICE_DEBUG;
#endif // DEBUG
	// create a device, device context and swap chain using the information in the scd struct
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


#pragma region RenderTargetView And Viewport
	D3D11_TEXTURE2D_DESC d3dTextureDescription;
	ZeroMemory(&d3dTextureDescription, sizeof(d3dTextureDescription));

	ID3D11Texture2D *pd3dRenderTargetTexture;

	m_pd3dSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pd3dRenderTargetTexture);
	m_pd3dDevice->CreateRenderTargetView(pd3dRenderTargetTexture, NULL, &m_pd3dRenderTargetView);

	float fViewportWidth = 0;
	float fViewportHeight = 0;

	pd3dRenderTargetTexture->GetDesc(&d3dTextureDescription);
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
	d3dDepthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS;

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
	//for (int i = 0; i < ENTITY_COUNT; i++)
	//{
	//	//Check planet's mask at [i] to see what needs to be released
	//	if (planet->mask[i] == )
	//	{
	//		planet->mesh[i].indexBuffer->Release();
	//		planet->mesh[i].vertexBuffer->Release();
	//	}
	//	destroyEntity(planet, i);
	//}
	m_pd3dSwapchain->Release();
	m_pd3dDevice->Release();
	m_pd3dDeviceContext->Release();
	m_pd3dDepthStencilState->Release();
	m_pd3dDepthStencilView->Release();
	m_pd3dRenderTargetView->Release();
}

void CGraphicsSystem::CreateShaders(ID3D11Device * device)
{
	//D3D11_BUFFER_DESC matrixBufferDesc;
	//D3D11_BUFFER_DESC bpBufferDesc;

	//device->CreateVertexShader(MyVertexShader, sizeof(MyVertexShader), NULL, &m_vertexShader);
	//device->CreatePixelShader(MyPixelShader, sizeof(MyPixelShader), NULL, &m_pixelShader);
	//Input Layout Setup
	//Now setup the layout of the data that goes into the shader.

	// Get a count of the elements in the layout.
	//int	numElements = sizeof(m_layoutDesc) / sizeof(m_layoutDesc[0]);

	// Create the vertex input layout.
	//device->CreateInputLayout(m_layoutDesc, numElements, MyVertexShader,
	//	sizeof(MyVertexShader), &m_layout);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	//matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	//matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	//matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//matrixBufferDesc.MiscFlags = 0;
	//matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	//device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	//bpBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	//bpBufferDesc.ByteWidth = sizeof(BlinnPhongType);
	//bpBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//bpBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//bpBufferDesc.MiscFlags = 0;
	//bpBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	//device->CreateBuffer(&bpBufferDesc, NULL, &m_blinnPhong);
}

void CGraphicsSystem::CreateBuffers(TWorld *ptPlanet)//init first frame
{
	CreateShaders(m_pd3dDevice);
	//for (int i = 0; i < ENTITY_COUNT; i++)
	//{
	//	//Check Mask to see what buffers need to get created
	//	if (planet->mask[i] == ())
	//	{
	//		if (planet->mesh[i].vertexCount)
	//			dev->CreateBuffer(&planet->mesh[i].vertexBufferDesc, &planet->mesh[i].vertexData, &planet->mesh[i].vertexBuffer);
	//		if (planet->mesh[i].indexCount)
	//			dev->CreateBuffer(&planet->mesh[i].indexBufferDesc, &planet->mesh[i].indexData, &planet->mesh[i].indexBuffer);

	//	}
	
	//}

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

void CGraphicsSystem::InitShaderData(ID3D11DeviceContext * pd3dDeviceContext, XMMATRIX d3dWorldMatrix, XMMATRIX d3dViewMatrix, XMMATRIX d3dProjectionMatrix, bool bCollide, int nMask, XMFLOAT3 d3dLightPosition, XMFLOAT3 d3dCameraPosition, XMFLOAT4X4 *pd3dJointsForVS)
{
	//HRESULT result;
	
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	//MatrixBufferType* dataPtr;

	//D3D11_MAPPED_SUBRESOURCE bpResource;
	//BlinnPhongType* bpPtr;

	//unsigned int bufferNumber;
	//Transpose the matrices to prepare them for the shader.

	//Map Matrix Constant Buffer

	// Get a pointer to the data in the constant buffer.
	// Copy the matrices into the constant buffer.

	//Unmap Matrix Constant Buffer
	// Position of the constant buffer in the vertex shader.
	// Set the constant buffer in the vertex shader with the updated values.

}

void CGraphicsSystem::ExecutePipeline(TWorld * ptWorld, ID3D11DeviceContext *pd3dDeviceContext, int m_nIndexCount, int nEntity)
{
	//Set Input_Layout

	//Set Shader

	//Draw
}

