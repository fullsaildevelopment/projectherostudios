#include "stdafx.h"
#include "Graphics_System.h"

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

	m_fAspectRatio = cRectangle.bottom - cRectangle.top / cRectangle.right - cRectangle.left;
#ifdef _DEBUG
	unsigned int nDeviceAndSwapchainFlag = D3D11_CREATE_DEVICE_DEBUG;
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
	pd3dRenderTargetTexture->Release();
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
		destroyEntity(ptPlanet, nEntityIndex);
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
	
	//m_pcMyInput->DecrementCount();
}

void CGraphicsSystem::CreateShaders(ID3D11Device * device)
{	
//	#pragma region MyShaders
//	D3D11_BUFFER_DESC d3dMyMatrixBufferDesc;
//	device->CreateVertexShader(MyVertexShader, sizeof(MyVertexShader), NULL, &m_pd3dMyVertexShader);
//	device->CreatePixelShader(MyPixelShader, sizeof(MyPixelShader), NULL, &m_pd3dMyPixelShader);
//	//Input Layout Setup
//	//Now setup the layout of the data that goes into the shader.
//	D3D11_INPUT_ELEMENT_DESC m_d3dMyLayoutDesc[] =
//	{
//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	{ "POSITION", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	};
//	//Get a count of the elements in the layout.
//	int	nElements = sizeof(m_d3dMyLayoutDesc) / sizeof(m_d3dMyLayoutDesc[0]);
//
//	//Create the vertex input layout.
//	device->CreateInputLayout(m_d3dMyLayoutDesc, nElements, MyVertexShader,
//		sizeof(MyVertexShader), &m_pd3dMyInputLayout);
//
//	//Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
//	d3dMyMatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
//	d3dMyMatrixBufferDesc.ByteWidth = sizeof(TMyMatrixBufferType);
//	d3dMyMatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	d3dMyMatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	d3dMyMatrixBufferDesc.MiscFlags = 0;
//	d3dMyMatrixBufferDesc.StructureByteStride = 0;
//
//	//Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
//	device->CreateBuffer(&d3dMyMatrixBufferDesc, NULL, &m_pd3dMyMatrixBuffer);
//#pragma endregion

	#pragma region PrimalShaders
	D3D11_BUFFER_DESC d3dPrimalMatrixBufferDesc;
	D3D11_BUFFER_DESC d3dPrimalPixelBufferDesc;
	device->CreateVertexShader(PrimalVertexShader, sizeof(PrimalVertexShader), NULL, &m_pd3dPrimalVertexShader);
	device->CreatePixelShader(PrimalPixelShader, sizeof(PrimalPixelShader), NULL, &m_pd3dPrimalPixelShader);
	//Input Layout Setup
	//Now setup the layout of the data that goes into the shader.
	D3D11_INPUT_ELEMENT_DESC m_d3dPrimalLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "POSITION", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	//Get a count of the elements in the layout.
	int nElements = sizeof(m_d3dPrimalLayoutDesc) / sizeof(m_d3dPrimalLayoutDesc[0]);

	//Create the vertex input layout.
	device->CreateInputLayout(m_d3dPrimalLayoutDesc, nElements, PrimalVertexShader,
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
XMMATRIX CGraphicsSystem::SetDefaultViewMatrix()
{
	XMMATRIX DefaultViewMatrix;

	DefaultViewMatrix = SetDefaultWorldPosition();

	return DefaultViewMatrix;
}

//XMMATRIX CGraphicsSystem::SetPlayerViewMatrix(XMMATRIX d3d_ViewM, XMMATRIX d3d_playerM)
//{
//	XMMATRIX DefaultViewMatrix;
//
//	DefaultViewMatrix = XMMatrixLookAtLH(d3d_ViewM.r[3],d3d_playerM.r[3], XMVectorSet(0, 1.0f, 0, 1.0f));
//
//	return DefaultViewMatrix;
//}

XMMATRIX CGraphicsSystem::SetDefaultPerspective()
{

	XMMATRIX DefaultPerspectiveMatrix;
	// the 90 is for fov if we want to implement field of view
	m_fFOV = 90;

	DefaultPerspectiveMatrix.r[0].m128_f32[0] = 1 / tan(m_fFOV* 0.5 * 3.15f / 180);
	DefaultPerspectiveMatrix.r[0].m128_f32[1] = 0;
	DefaultPerspectiveMatrix.r[0].m128_f32[2] = 0;
	DefaultPerspectiveMatrix.r[0].m128_f32[3] = 0;

	DefaultPerspectiveMatrix.r[1].m128_f32[0] = 0;
	DefaultPerspectiveMatrix.r[1].m128_f32[1] = 1 / tan(m_fFOV * 0.5 * 3.15f / 180);
	DefaultPerspectiveMatrix.r[1].m128_f32[2] = 0;
	DefaultPerspectiveMatrix.r[1].m128_f32[3] = 0;

	DefaultPerspectiveMatrix.r[2].m128_f32[0] = 0;
	DefaultPerspectiveMatrix.r[2].m128_f32[1] = 0;
	DefaultPerspectiveMatrix.r[2].m128_f32[2] = 100 / (100 - 0.1);
	DefaultPerspectiveMatrix.r[2].m128_f32[3] = 1;

	DefaultPerspectiveMatrix.r[3].m128_f32[0] = 0;
	DefaultPerspectiveMatrix.r[3].m128_f32[1] = 0;
	DefaultPerspectiveMatrix.r[3].m128_f32[2] = -(100 * 0.1) / (100 - 0.1);
	DefaultPerspectiveMatrix.r[3].m128_f32[3] = 0;
	return DefaultPerspectiveMatrix;
}

XMMATRIX CGraphicsSystem::SetDefaultOffset()
{
	return XMMatrixTranslation(0, 1.5f, 5.5f);
}

XMMATRIX CGraphicsSystem::SetDefaultWorldPosition()
{

	XMMATRIX DefaultPerspectiveMatrix;
	// the 90 is for fov if we want to implament field of view
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

void CGraphicsSystem::InitMyShaderData(ID3D11DeviceContext * pd3dDeviceContext, XMMATRIX d3dWorldMatrix, XMMATRIX d3dViewMatrix, XMMATRIX d3dProjectionMatrix, int nMask, XMFLOAT3 d3dLightPosition, XMFLOAT3 d3dCameraPosition, XMFLOAT4X4 *pd3dJointsForVS) 
{

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

	d3dViewMatrix = XMMatrixInverse(NULL, CameraMatrix);

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
	ptPrimalPixelBufferDataPointer->m_d3dCollisionColor = XMFLOAT4(0,0,0,0);

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
	
	d3dVertexBuffer.m_d3dViewMatrix = XMMatrixInverse(NULL, CameraMatrix);
	unsigned int bufferNumber;
	XMMATRIX d3dTmpViewM;

	//d3dTmpViewM = XMMatrixInverse(NULL, );
		 m_fCameraXPosition = CameraMatrix.r[3].m128_f32[0];// d3dTmpViewM.r[3].m128_f32[0];
		m_fCameraYPosition = CameraMatrix.r[3].m128_f32[1];//d3dTmpViewM.r[3].m128_f32[1];
		m_fCameraZPosition = CameraMatrix.r[3].m128_f32[2];//d3dTmpViewM.r[3].m128_f32[2];

	XMMATRIX d3dView;

	d3dView = d3dVertexBuffer.m_d3dViewMatrix;
	#pragma region Map To Vertex Constant Buffer
	pd3dDeviceContext->Map(m_pd3dPrimalVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dPrimalVertexMappedResource);


	// Get a pointer to the data in the constant buffer.
	ptPrimalVertexBufferDataPointer = (TPrimalVertexBufferType*)d3dPrimalVertexMappedResource.pData;

	// Copy the matrices into the constant buffer.
	ptPrimalVertexBufferDataPointer->m_d3dWorldMatrix = d3dVertexBuffer.m_d3dWorldMatrix;
	ptPrimalVertexBufferDataPointer->m_d3dViewMatrix = d3dView;
	ptPrimalVertexBufferDataPointer->m_d3dProjectionMatrix = d3dVertexBuffer.m_d3dProjectionMatrix;

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

void CGraphicsSystem::ExecutePipeline(ID3D11DeviceContext *pd3dDeviceContext, int m_nIndexCount, int nGraphicsMask, int nShaderID)
{
	switch (nShaderID)
	{
		case 1:	
		{
			//Set Input_Layout
			pd3dDeviceContext->IASetInputLayout(m_pd3dMyInputLayout);
			//Set Shader
			pd3dDeviceContext->VSSetShader(m_pd3dMyVertexShader, NULL, 0);
			pd3dDeviceContext->PSSetShader(m_pd3dMyPixelShader, NULL, 0);
			////Draw
			//if (nGraphicsMask == (/*COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID*/))
			//{
			//	pd3dDeviceContext->Draw(m_nIndexCount, 0);
			//}
			break;
		}
		case 2:
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
		case 3:
		{
			pd3dDeviceContext->IASetInputLayout(m_pd3dPrimalInputLayout);
			//Set Shader
			pd3dDeviceContext->VSSetShader(m_pd3dPrimalVertexShader, NULL, 0);
			pd3dDeviceContext->PSSetShader(m_pd3dPrimalPixelShader, NULL, 0);
			//Draw
			if (nGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_SIMPLEMESH | COMPONENT_SHADERID))
			{
				pd3dDeviceContext->DrawIndexed(m_nIndexCount, 0,0);
			}
			break;
		}
		default:
			break;
	}
}
