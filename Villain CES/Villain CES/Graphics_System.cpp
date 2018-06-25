#include "stdafx.h"
#include "Graphics_System.h"

Graphics_System::Graphics_System()
{
}

Graphics_System::~Graphics_System()
{
}

void Graphics_System::InitD3D(HWND hWnd)
{
	#pragma region Window Stuff

	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	RECT rect;
	GetClientRect(hWnd, &rect);

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = 1;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode
	scd.BufferDesc.Width = rect.right - rect.left;
	scd.BufferDesc.Height = rect.bottom - rect.top;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	unsigned int flag = 0;
#ifdef _DEBUG
	flag = D3D11_CREATE_DEVICE_DEBUG;
#endif // DEBUG
	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		flag,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&dev,
		NULL,
		&devcon);


#pragma region RenderTargetView And Viewport
	D3D11_TEXTURE2D_DESC textDesc;
	ZeroMemory(&textDesc, sizeof(textDesc));

	ID3D11Texture2D *renderTargetText;

	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&renderTargetText);
	dev->CreateRenderTargetView(renderTargetText, NULL, &rtv);

	float width = 0;
	float height = 0;
	D3D11_TEXTURE2D_DESC textdesc;

	renderTargetText->GetDesc(&textdesc);
	width = (float)textdesc.Width;
	height = (float)textdesc.Height;

	viewport = {
		0,
		0,
		width,
		height,
		0,
		1
	};
#pragma endregion

	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = (UINT)width;
	descDepth.Height = (UINT)height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	dev->CreateTexture2D(&descDepth, NULL, &pDepthStencil);


	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	descDSV.Flags = 0;
	dev->CreateDepthStencilView(pDepthStencil, NULL, &depthStencilView);

	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	dev->CreateDepthStencilState(&dsDesc, &depthStencilState);


	D3D11_RASTERIZER_DESC RasterizerStateWireframe
	{
		D3D11_FILL_WIREFRAME,
		D3D11_CULL_NONE,
		FALSE,
		0,
		0.0f,
		0.0f,
		TRUE,
		FALSE,
		FALSE,
		FALSE
	};
	D3D11_RASTERIZER_DESC RasterizerStateNotWireframe
	{
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK,
		FALSE,
		0,
		0.0f,
		0.0f,
		TRUE,
		FALSE,
		FALSE,
		FALSE
	};

	dev->CreateRasterizerState(&RasterizerStateWireframe, &pRSWireFrame);
	dev->CreateRasterizerState(&RasterizerStateNotWireframe, &pRSNotWireFrame);
#pragma endregion

}

void Graphics_System::UpdateD3D()
{
	const float purple[] = { .5f, .05f, .5f, 1 };

	devcon->OMSetRenderTargets(1, &rtv, depthStencilView);
	devcon->ClearRenderTargetView(rtv, purple);
	devcon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1, 0); // clear it to Z exponential Far.
	devcon->RSSetViewports(1, &viewport);
}

void Graphics_System::CleanD3D(World *planet)
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
	swapchain->Release();
	dev->Release();
	devcon->Release();
	depthStencilState->Release();
	depthStencilView->Release();
	rtv->Release();
}

void Graphics_System::CreateShaders(ID3D11Device * device)
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

void Graphics_System::CreateBuffers(World *planet)//init first frame
{
	CreateShaders(dev);
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

void Graphics_System::UpdateBuffer(World * wurld, std::vector<simple_mesh> vertVector, int entity, int mask)
{
	
	bool isVertexBufferSet = false;
		if (isVertexBufferSet)
		{
			// This is where it copies the new vertices to the buffer.
			// but it's causing flickering in the entire screen...
			D3D11_MAPPED_SUBRESOURCE resource;
			if (mask == COMPONENT_DEBUGMESH)
			{
				devcon->Map(wurld->debug_mesh[entity].vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
				memcpy(resource.pData, &vertVector[0], sizeof(vertVector));
				devcon->Unmap(wurld->debug_mesh[entity].vertexBuffer, 0);
			}
		}
		else
		{			
			UINT stride = sizeof(simple_mesh);
			UINT offset = 0;

			D3D11_SUBRESOURCE_DATA resourceData;
			ZeroMemory(&resourceData, sizeof(resourceData));
			resourceData.pSysMem = &vertVector[0];
			if (mask == COMPONENT_DEBUGMESH)
			{
				// This is run in the first frame. But what if new vertices are added to the scene?
				wurld->debug_mesh[entity].vertexBufferDesc.ByteWidth = sizeof(simple_mesh) * wurld->debug_mesh[entity].vertexCount;

				if (wurld->debug_mesh[entity].vertexBuffer)
					wurld->debug_mesh[entity].vertexBuffer->Release();
				dev->CreateBuffer(&wurld->debug_mesh[entity].vertexBufferDesc, &resourceData, &wurld->debug_mesh[entity].vertexBuffer);
				devcon->IASetVertexBuffers(0, 1, &wurld->debug_mesh[entity].vertexBuffer, &stride, &offset);
			}
			isVertexBufferSet = true;
		}
}

void Graphics_System::InitShaderData(ID3D11DeviceContext * deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, bool collide, int mask, XMFLOAT3 lightPos, XMFLOAT3 camPos, XMFLOAT4X4 *x)
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

void Graphics_System::ExecutePipeline(World * wurld, ID3D11DeviceContext *deviceContext, int indexCount, int entity)
{
	//Set Input_Layout

	//Set Shader

	//Draw
}

