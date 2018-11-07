#pragma once
#include "Entity.h"
#include "Graphics_Component.h"
#include "MyVertexShader.csh"
#include "MyPixelShader.csh"
#include "PrimalVertexShader.csh"
#include "PrimalPixelShader.csh"
#include "UIVertexShader.csh"
#include "UIPixelShader.csh"
#include "QuadGeometryShader.csh"
#include "QuadVertexShader.csh"
#include "QuadPixelShader.csh"
#include "SkyboxVertexShader.csh"
#include "SkyboxPixelShader.csh"
#include "AnimatedVertexShader.csh"
#include "AnimatedPixelShader.csh"
#include "LinePixelShader.csh"
#include "LineVertexShader.csh"

class CGraphicsSystem
{
public:
	// global declarations
	IDXGISwapChain * m_pd3dSwapchain;             // the pointer to the swap chain interface
	ID3D11Device *m_pd3dDevice;                     // the pointer to our Direct3D device interface
	ID3D11DeviceContext *m_pd3dDeviceContext;           // the pointer to our Direct3D device context
	ID3D11RenderTargetView *m_pd3dRenderTargetView;
	ID3D11RenderTargetView *m_pd3dOutsideRenderTargetView;
	ID3D11Texture2D		*m_pd3dOutsideGlassRenderToTexture;
	ID3D11ShaderResourceView * m_pd3dOutsideGlassSRV;
	ID3D11ShaderResourceView * m_pd3dCurrentBulletTexture;
	ID3D11DepthStencilState* m_pd3dDepthStencilState;
	ID3D11DepthStencilView * m_pd3dDepthStencilView;
	ID3D11RasterizerState* m_pd3dNoCullRasterizerState;
	ID3D11RasterizerState* m_pd3dCullRasterizerState;
	ID3D11SamplerState * m_pd3dSamplerState;

	ID3D11Texture2D* m_pd3dDepthStencil;
	D3D11_VIEWPORT m_d3dViewport;

	struct TLineVertexBufferType
	{
		XMMATRIX m_d3dWorldMatrix;
		XMMATRIX m_d3dViewMatrix;
		XMMATRIX m_d3dProjectionMatrix;
		//XMFLOAT4 endPoint;
	};
	
	struct TQuadGeometryBufferType
	{
		XMMATRIX m_d3dWorldMatrix;
		XMMATRIX m_d3dViewMatrix;
		XMMATRIX m_d3dProjectionMatrix;
		float m_fHealth;
	};
	
	struct TQuadPixelBufferType
	{
		/*Nothing*/
		XMFLOAT4 m_d3dBackgroundColor;
	};

	struct TMyVertexBufferType
	{
		XMMATRIX m_d3dWorldMatrix;
		XMMATRIX m_d3dViewMatrix;
		XMMATRIX m_d3dProjectionMatrix;

		XMFLOAT4 m_d3dColor;
	};

	struct TAnimatedVertexBufferType
	{
		XMMATRIX m_d3dWorldMatrix;
		XMMATRIX m_d3dViewMatrix;
		XMMATRIX m_d3dProjectionMatrix;
		XMFLOAT4X4 m_d3dJointsForVS[MAX_JOINTS];
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

	struct TUIVertexBufferType
	{
		//XMMATRIX m_d3dWorldMatrix;
		//XMMATRIX m_d3dViewMatrix;
		//XMMATRIX m_d3dProjectionMatrix;

		float start;
		float end;
		float ratio;
		float padding;
	};

	struct TUIPixelBufferType
	{
		XMFLOAT4 hoverColor;
	};

	

	struct TLightBufferType
	{
		TLightMaterials m_Proprties;
		TLights m_allLights[2];
		XMFLOAT4 Ambience;
		XMFLOAT4 lightEyePos;

	};

	//First Frame
	CGraphicsSystem();
	~CGraphicsSystem();

	void InitD3D(HWND cTheWindow);
	void CleanD3D(TWorld *ptPlanet);
	/*
		Call create WIC textures from file on all textures with the associated file.
		Get file name
		Get number of files there
		loop number of file times
			CreateWicTextureFromFile on current filename index			
	*/
	//TTextureOptimization CreateTexturesFromFile(TMaterialImport* arrayOfMaterials, int numberOfEntities);
	TMaterialOptimized CreateTexturesFromFile(TMaterialImport* arrayOfMaterials, int numberOfEntities);
	ID3D11ShaderResourceView* TexturesToCubeMap(ID3D11DeviceContext * pd3dDeviceContext, ID3D11Resource* srcText[6]);
	void UpdateD3D_RenderToTexture();
	void CreateBuffers(TWorld * ptWorld);
	void CreateEntityBuffer(TWorld * ptWorld, int nEnityIndex);
	void CreateShaders(ID3D11Device* pd3dDevice);

	XMMATRIX SetDefaultViewMatrix();
	XMMATRIX SetDefaultWorldPosition();
	XMMATRIX SetDefaultPerspective();
	XMMATRIX SetDefaultPerspective(float FOV);
	XMMATRIX SetDefaultOffset();
	//Every Frame
	void UpdateD3D();
	
	void InitQuadShaderData(ID3D11DeviceContext * pd3dDeviceContext, XMMATRIX d3dWorldMatrix, XMMATRIX d3dViewMatrix, XMMATRIX d3dProjectionMatrix, TDebugMesh tDebugMesh, XMMATRIX CameraMatrix, XMFLOAT4 BackgroundColor, float fHealth);

	void InitPrimalShaderData(ID3D11DeviceContext * pd3dDeviceContext, XMMATRIX d3dWorldMatrix, XMMATRIX d3dViewMatrix, XMMATRIX d3dProjectionMatrix, TDebugMesh tDebugMesh, XMMATRIX CameraMatrix);
	void InitPrimalShaderData2(ID3D11DeviceContext * pd3dDeviceContext, TPrimalVertexBufferType d3dVertexBuffer, TPrimalPixelBufferType d3dPixelBuffer, TSimpleMesh tSimpleMesh, XMMATRIX CameraMatrix);
	void InitPrimalShaderData3(ID3D11DeviceContext * pd3dDeviceContext, TPrimalVertexBufferType d3dVertexBuffer, TPrimalPixelBufferType d3dPixelBuffer, TMesh tMesh, XMMATRIX CameraMatrix);
	void InitSkyboxShaderData(ID3D11DeviceContext * pd3dDeviceContext, TMyVertexBufferType d3dVertexBuffer, TMesh tSimpleMesh, XMMATRIX CameraMatrix);
	void InitAnimShaderData(ID3D11DeviceContext * pd3dDeviceContext, TAnimatedVertexBufferType d3dVertexBuffer, TMesh tSimpleMesh, TLightMaterials tMaterials, XMMATRIX CameraMatrix);
	void InitMyShaderData(ID3D11DeviceContext * pd3dDeviceContext, TMyVertexBufferType d3dVertexBuffer, TMesh tSimpleMesh, XMMATRIX CameraMatrix);
	void InitUIShaderData(ID3D11DeviceContext * pd3dDeviceContext, TUIVertexBufferType d3dVertexBuffer, TUIPixelBufferType d3dPixelBuffer, TMesh tMesh, XMMATRIX CameraMatrix);
	void InitLineShaderData(ID3D11DeviceContext * pd3dDeviceContext, XMMATRIX d3dWorldMatrix, XMMATRIX d3dViewMatrix, XMMATRIX d3dProjectionMatrix, TDebugMesh tDebugMesh, XMMATRIX CameraMatrix, std::vector<TPrimalVert> m_verts, ID3D11ShaderResourceView * ParticleTexture);
	void ExecutePipeline(ID3D11DeviceContext *pd3dDeviceContext, int m_nIndexCount, int nGraphicsMask, int nShaderID);
	void UpdateBuffer(TWorld * ptWorld, std::vector<TSimpleMesh> vtVertexVector, int nEntity, int nMask);
	void UpdateLineVTBuffer(D3D11_BUFFER_DESC* bufDesc, ID3D11Buffer* &vertexBuffer, std::vector<TPrimalVert> lineVector);
	void StoreBeamPoints(XMFLOAT3 startPoint, XMFLOAT3 endPoint, std::vector<TPrimalVert> &BeamPoints, float timeScroll);
	ImporterData ReadMesh(const char * input_file_path);
	ImporterData ReadMesh2(const char * input_file_path, int thingBeingTextured = 0, ImporterData* tImportMe2 = nullptr, int numberOfPaths = 1);
	XMVECTOR GetCameraPos();
	XMMATRIX SetDefaultCameraMatrix();
	XMMATRIX ResetAimModeCameraOffset();
	void UpdateAnimationInfo(TAnimation &vtAnimation, TAnimationVariant &vtAnimationVariant, bool &forward, bool &backward, bool &left, bool &right);
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

	//ID3D11VertexShader  *m_pd3dLineVertexShader;
	//ID3D11GeometryShader*m_pd3dLineGeometryShader;
	ID3D11Buffer		*m_pd3dLineVTConstantBuffer;
	ID3D11InputLayout	*m_pd3dLineInputLayout;
	ID3D11PixelShader	*m_pd3dLinePixelShader;
	ID3D11VertexShader	*m_pd3dLineVertexShader;

	ID3D11Buffer		*m_pd3dLightPixelBuffer;

	ID3D11VertexShader	*m_pd3dMyVertexShader;
	ID3D11PixelShader	*m_pd3dMyPixelShader;
	ID3D11InputLayout	*m_pd3dMyInputLayout;
	ID3D11Buffer		*m_pd3dMyVertexBuffer;

	ID3D11VertexShader	*m_pd3dUIVertexShader;
	ID3D11PixelShader	*m_pd3dUIPixelShader;
	ID3D11InputLayout	*m_pd3dUIInputLayout;
	ID3D11Buffer		*m_pd3dUIVertexBuffer;
	ID3D11Buffer		*m_pd3dUIPixelBuffer;

	ID3D11VertexShader	*m_pd3dSkyboxVertexShader;
	ID3D11PixelShader	*m_pd3dSkyboxPixelShader;
	ID3D11InputLayout	*m_pd3dSkyboxInputLayout;
	ID3D11Buffer		*m_pd3dSkyboxVertexBuffer;


	ID3D11BlendState	*m_pd3dBlendState;
	ID3D11VertexShader	*m_pd3dAnimatedVertexShader;
	ID3D11PixelShader	*m_pd3dAnimatedPixelShader;
	ID3D11InputLayout	*m_pd3dAnimatedInputLayout;
	ID3D11Buffer		*m_pd3dAnimatedVertexBuffer;

	ID3D11Buffer		*m_pd3dBlinnPhongBuffer;
	ID3D11Debug			*debug;
	ID3D11Texture2D		*m_pd3dRenderTargetTexture;
	float				m_fCameraXPosition;
	float				m_fCameraYPosition;
	float				m_fCameraZPosition;
	float				m_fAspectRatio;
	float				m_fFOV;
	float				m_fNewX;
	float				m_fNewY;
	float				m_fMouseRotationSpeed;
	float				m_fMouseMovementSpeed;

	/* 
		Helper function that reads the texture filepaths for Jonah's exporter.

		textureNumber - where the path gets saved in the array of filepaths
		thingBeingTextured - determines if the object has its own .fbm folder or if it's a dulicate of another object
	*/
	void GetTexturePathHelper(fstream& file, ImporterData& tImportMe, int textureNumber, int thingBeingTextured = 0);
};