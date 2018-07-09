#include "stdafx.h"
#include "Graphics_System.h"

CGraphicsSystem::CGraphicsSystem()
{
	m_ncameraXPosition = 0;
	m_ncameraYPosition = 0.5;
	m_ncameraZPosition = -10;


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
	int nFlag = D3D11_CREATE_DEVICE_DEBUG;
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
	for (int nEntityIndex = 0; nEntityIndex < ENTITYCOUNT; nEntityIndex++)
	{
		//Check planet's mask at [i] to see what needs to be released
		if (ptPlanet->atGraphicsMask[nEntityIndex].m_tnGraphicsMask == (COMPONENT_GRAPHICSMASK | COMPONENT_DEBUGMESH | COMPONENT_SHADERID))
		{
			ptPlanet->atDebugMesh[nEntityIndex].m_pd3dVertexBuffer->Release();
		}

		destroyEntity(ptPlanet, nEntityIndex);
	}
	m_pd3dSwapchain->Release();
	m_pd3dDevice->Release();
	m_pd3dDeviceContext->Release();
	m_pd3dDepthStencilState->Release();
	m_pd3dDepthStencilView->Release();
	m_pd3dRenderTargetView->Release();
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
	device->CreateVertexShader(PrimalVertexShader, sizeof(PrimalVertexShader), NULL, &m_pd3dPrimalVertexShader);
	device->CreatePixelShader(PrimalPixelShader, sizeof(PrimalPixelShader), NULL, &m_pd3dPrimalPixelShader);
	//Input Layout Setup
	//Now setup the layout of the data that goes into the shader.
	D3D11_INPUT_ELEMENT_DESC m_d3dPrimalLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "POSITION", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	//Get a count of the elements in the layout.
	int nElements = sizeof(m_d3dPrimalLayoutDesc) / sizeof(m_d3dPrimalLayoutDesc[0]);

	//Create the vertex input layout.
	device->CreateInputLayout(m_d3dPrimalLayoutDesc, nElements, PrimalVertexShader,
		sizeof(PrimalVertexShader), &m_pd3dPrimalInputLayout);

	//Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	d3dPrimalMatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dPrimalMatrixBufferDesc.ByteWidth = sizeof(TPrimalMatrixBufferType);
	d3dPrimalMatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dPrimalMatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dPrimalMatrixBufferDesc.MiscFlags = 0;
	d3dPrimalMatrixBufferDesc.StructureByteStride = 0;

	//Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	device->CreateBuffer(&d3dPrimalMatrixBufferDesc, NULL, &m_pd3dPrimalMatrixBuffer);
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

XMMATRIX CGraphicsSystem::SetDefaultViewMatrix()
{
	/*DefaultViewMatrix.view._11 = 1;

	DefaultViewMatrix.view._22 = cos((-18 * 3.14) / 180.0f);

	DefaultViewMatrix.view._23 = -sin((-18 * 3.14) / 180.0f);

	DefaultViewMatrix.view._32 = sin((-18 * 3.14) / 180.0f);

	DefaultViewMatrix.view._33 = cos((-18 * 3.14) / 180.0f);

	DefaultViewMatrix.view._44 = 1;*/
	XMMATRIX DefaultViewMatrix;
	DefaultViewMatrix.r[0].m128_f32[0] = 1;
	DefaultViewMatrix.r[0].m128_f32[1] = 0;
	DefaultViewMatrix.r[0].m128_f32[2] = 0;
	DefaultViewMatrix.r[0].m128_f32[3] = 0;

	DefaultViewMatrix.r[1].m128_f32[0] = 0;
	DefaultViewMatrix.r[1].m128_f32[1] = cos((-18 * 3.14) / 180.0f);
	DefaultViewMatrix.r[1].m128_f32[2] = -sin((-18 * 3.14) / 180.0f);;
	DefaultViewMatrix.r[1].m128_f32[3] = 0;

	DefaultViewMatrix.r[2].m128_f32[0] = 0;
	DefaultViewMatrix.r[2].m128_f32[1] = sin((-18 * 3.14) / 180.0f);;
	DefaultViewMatrix.r[2].m128_f32[2] = cos((-18 * 3.14) / 180.0f);;
	DefaultViewMatrix.r[2].m128_f32[3] = 0;

	DefaultViewMatrix.r[3].m128_f32[0] = m_ncameraXPosition;
	DefaultViewMatrix.r[3].m128_f32[1] = m_ncameraYPosition;
	DefaultViewMatrix.r[3].m128_f32[2] = m_ncameraZPosition;
	DefaultViewMatrix.r[3].m128_f32[3] = 1;

	DefaultViewMatrix.r[0].m128_f32[0] = DefaultViewMatrix.r[0].m128_f32[0];
	//DefaultViewMatrix.view._21 = DefaultViewMatrix.view._12;
	DefaultViewMatrix.r[1].m128_f32[0] = DefaultViewMatrix.r[0].m128_f32[1];
//	DefaultViewMatrix.view._31 = DefaultViewMatrix.view._13;
	DefaultViewMatrix.r[2].m128_f32[0] = DefaultViewMatrix.r[0].m128_f32[2];
	//DefaultViewMatrix.view._12 = DefaultViewMatrix.view._21;
	DefaultViewMatrix.r[0].m128_f32[1] = DefaultViewMatrix.r[1].m128_f32[0];
	//DefaultViewMatrix.view._22 = DefaultViewMatrix.view._22;
	DefaultViewMatrix.r[1].m128_f32[1] = DefaultViewMatrix.r[1].m128_f32[1];
//	DefaultViewMatrix.view._32 = DefaultViewMatrix.view._23;
	DefaultViewMatrix.r[2].m128_f32[1] = DefaultViewMatrix.r[1].m128_f32[2];
//	DefaultViewMatrix.view._13 = DefaultViewMatrix.view._31;
	DefaultViewMatrix.r[0].m128_f32[2] = DefaultViewMatrix.r[2].m128_f32[0];
	DefaultViewMatrix.r[1].m128_f32[2] = DefaultViewMatrix.r[2].m128_f32[1];
	DefaultViewMatrix.r[2].m128_f32[2] = DefaultViewMatrix.r[2].m128_f32[2];
	XMFLOAT4 pos; //pos.x = toShader.view._41;
	pos.x = m_ncameraXPosition;
	pos.y = m_ncameraYPosition;
	pos.z = m_ncameraZPosition;
	pos.w = 0;
	XMVECTOR temptpos;
	temptpos.m128_f32[0] = pos.x;
	temptpos.m128_f32[1] = pos.y;
	temptpos.m128_f32[2] = pos.z;
	temptpos.m128_f32[3] = pos.w;

	temptpos = XMVector4Transform(temptpos, DefaultViewMatrix);
	temptpos.m128_f32[0] *= -1;
	temptpos.m128_f32[1] *= -1;
	temptpos.m128_f32[2] *= -1;
	DefaultViewMatrix.r[3].m128_f32[0] = temptpos.m128_f32[0];
	DefaultViewMatrix.r[3].m128_f32[1] = temptpos.m128_f32[1];
	DefaultViewMatrix.r[3].m128_f32[2] = temptpos.m128_f32[2];
	
	/*DefaultViewMatrix.view._23 = DefaultViewMatrix.view._32;
	DefaultViewMatrix.view._33 = DefaultViewMatrix.view._33;*/




	return DefaultViewMatrix;
}

XMMATRIX CGraphicsSystem::SetDefaultPerspective()
{
	/*DefaultViewMatrix.perspective._22 = 1 / tan(fov * 0.5 * 3.15f / 180);
	DefaultViewMatrix.perspective._11 = DefaultViewMatrix.perspective._22;
	DefaultViewMatrix.perspective._34 = 1;
	DefaultViewMatrix.perspective._33 = farplane / (farplane - nearplane);
	DefaultViewMatrix.perspective._43 = -(farplane * nearplane) / (farplane - nearplane);
	DefaultViewMatrix.perspective = DefaultViewMatrix.perspective;*/

	XMMATRIX DefaultPerspectiveMatrix;
	// the 90 is for fov if we want to implament field of view
	DefaultPerspectiveMatrix.r[0].m128_f32[0] = 1 / tan(90* 0.5 * 3.15f / 180);
	DefaultPerspectiveMatrix.r[0].m128_f32[1] = 0;
	DefaultPerspectiveMatrix.r[0].m128_f32[2] = 0;
	DefaultPerspectiveMatrix.r[0].m128_f32[3] = 0;

	DefaultPerspectiveMatrix.r[1].m128_f32[0] = 0;
	DefaultPerspectiveMatrix.r[1].m128_f32[1] = 1 / tan(90 * 0.5 * 3.15f / 180);
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

	DefaultPerspectiveMatrix.r[3].m128_f32[0] = 0;
	DefaultPerspectiveMatrix.r[3].m128_f32[1] = 0.2;
	DefaultPerspectiveMatrix.r[3].m128_f32[2] = -9;
	DefaultPerspectiveMatrix.r[3].m128_f32[3] = 1;
	return DefaultPerspectiveMatrix;
}


XMMATRIX CGraphicsSystem::DebugCamera(XMMATRIX tWVP)
{
	XMMATRIX d3dTmpViewM, d3dTmpProjectionM, m_d3dTmpWorldM, d3dMovementM, d3dRotation;


	m_d3dTmpWorldM.r[0].m128_f32[0] = 0;




	//Forward && Back Movement

	// up key movement
	
	if (InputCheck(G_KEY_UP) == 1) {
		m_ncameraZPosition += 0.01;
	}
	// down key movement
	if (InputCheck(G_KEY_DOWN) == 1) {
		m_ncameraZPosition -= 0.01;
	}
	// left key movement
	if (InputCheck(G_KEY_LEFT) == 1) {
		m_ncameraXPosition -= 0.01;
	}
	// right key movement
	if (InputCheck(G_KEY_RIGHT) == 1) {
		m_ncameraXPosition += 0.01;
	}
	if (InputCheck(G_KEY_SPACE) == 1) {
		m_ncameraYPosition += 0.01;
	}
	if (InputCheck(G_KEY_RIGHTSHIFT) == 1) {
		m_ncameraYPosition -= 0.01;
	}

	
	//{
	//if (GetAsyncKeyState('W') && GetAsyncKeyState('S'))
	//{
	//}
	//else if (true)
	//{

	//}
	//else {

	//}
	//}

	////Right && Left Movement
	//if (true)
	//{
	//	if (GetAsyncKeyState('A') && GetAsyncKeyState('D'))
	//	{
	//	}
	//	else if (true)
	//	{

	//	}
	//	else {

	//	}
	//}

	//// Up && Down Rotation(keybord implemented, soon to be changed in the mouse)
	//if (true)
	//{

	//}

	////Right && Left Rotation(keybord implemented, soon to be changed in the mouse)
	//if (true)
	//{

	//}



	return m_d3dTmpWorldM;
}

GReturn CGraphicsSystem::InitlizeGInput(HWND cTheWindow)
{
	// returns the results of function
	return CreateGInput(cTheWindow, sizeof(cTheWindow), &m_pcMyInput);

}

int CGraphicsSystem::InputCheck(int GInputKey)
{
	float fInputReturnValue = -1;
	m_pcMyInput->GetState(GInputKey, fInputReturnValue);
	return fInputReturnValue;
}

void CGraphicsSystem::InitMyShaderData(ID3D11DeviceContext * pd3dDeviceContext, XMMATRIX d3dWorldMatrix, XMMATRIX d3dViewMatrix, XMMATRIX d3dProjectionMatrix, int nMask, XMFLOAT3 d3dLightPosition, XMFLOAT3 d3dCameraPosition, XMFLOAT4X4 *pd3dJointsForVS)
{

}

void CGraphicsSystem::InitPrimalShaderData(ID3D11DeviceContext * pd3dDeviceContext, XMMATRIX d3dWorldMatrix, XMMATRIX d3dViewMatrix, XMMATRIX d3dProjectionMatrix, TDebugMesh tDebugMesh)
{
	D3D11_MAPPED_SUBRESOURCE d3dPrimalMappedResource;
	TPrimalMatrixBufferType* ptPrimalMatrixBufferDataPointer = nullptr;

	unsigned int bufferNumber;

	XMMATRIX d3dView;

	d3dView = d3dViewMatrix;
	//Transpose the matrices to prepare them for the shader.
//	d3dWorldMatrix = DirectX::XMMatrixTranspose(d3dWorldMatrix);
//	d3dView = DirectX::XMMatrixTranspose(d3dView);


	/*d3dView = XMMatrixInverse(NULL, d3dView);
	d3dView.r[3].m128_f32[0] = 0;
	d3dView.r[3].m128_f32[1] = 0;
	d3dView.r[3].m128_f32[2] = -10;
	d3dView.r[3].m128_f32[0] = 1;*/


	//d3dView = XMMatrixTranspose(d3dView);
	//d3dProjectionMatrix = DirectX::XMMatrixTranspose(d3dProjectionMatrix);

	pd3dDeviceContext->Map(m_pd3dPrimalMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dPrimalMappedResource);


	// Get a pointer to the data in the constant buffer.
	ptPrimalMatrixBufferDataPointer = (TPrimalMatrixBufferType*)d3dPrimalMappedResource.pData;

	// Copy the matrices into the constant buffer.
	ptPrimalMatrixBufferDataPointer->m_d3dWorldMatrix = d3dWorldMatrix;
	ptPrimalMatrixBufferDataPointer->m_d3dViewMatrix = d3dView;
	ptPrimalMatrixBufferDataPointer->m_d3dProjectionMatrix = d3dProjectionMatrix;

	// Unlock the constant buffer.
	pd3dDeviceContext->Unmap(m_pd3dPrimalMatrixBuffer, 0);

	// Position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Set the constant buffer in the vertex shader with the updated values.
	pd3dDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_pd3dPrimalMatrixBuffer);
	pd3dDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	pd3dDeviceContext->IASetVertexBuffers(0, 1, &tDebugMesh.m_pd3dVertexBuffer, &tDebugMesh.m_nVertexBufferStride, &tDebugMesh.m_nVertexBufferOffset);

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
		}
		default:
			break;
	}
}

