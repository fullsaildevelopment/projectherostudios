#include "ProjectileSystem.h"
CProjectileSystem::CProjectileSystem()
{
}

CProjectileSystem::~CProjectileSystem()
{
}

int CProjectileSystem::CreateBulletProjectile(int indexofBullet, Clips* Gun)
{
	Gun->fShootingCoolDown = Gun->FValueOfCoolDown;

	Gun->nBulletsAvailables.pop_front();
	
	cout << Gun->nBulletsAvailables.size();

	return 1;
}

bool CProjectileSystem::Reload(Clips * Gun)
{
	if (Gun->nBulletsAvailables.size() != Gun->nSizeofClipl) {
		for (size_t i = Gun->nBulletsAvailables.size(); i < Gun->nSizeofClipl; ++i) {
			Gun->nBulletsAvailables.push_back(true);
		}
		return true;
	}
	return false;
}

int CProjectileSystem::ExtractMaterial(TWorld *tThisWorld, Clips* Gun, int currentEntity)
{
	int tmp_Material = Gun->currentMaterial;
//	int extracted_Material = tThisWorld->atMesh[currentEntity].m_d3dSRVDiffuse;

	switch (tmp_Material)
	{
	case COMPONENT_WOOD:
		//Wood
		break;

	case COMPONENT_METAL:
		//Metal
	default:
		break;
	}


	return tmp_Material;
}

XMVECTOR CProjectileSystem::FindBeamEndPoint( XMMATRIX in_ViewMatrix, XMMATRIX in_ProjectionMatrix, HWND in_WindowHandle)
{
	// Get Current Mouse Position
	
	RECT tempRect;
	GetWindowRect(in_WindowHandle, &tempRect);
	// Make a temp projection matrix to put point in
	XMMATRIX tempProjection_Matrix = XMMatrixMultiply(in_ViewMatrix, in_ProjectionMatrix);
	tempProjection_Matrix = XMMatrixInverse(NULL ,tempProjection_Matrix);

	float currentZDepth = in_ProjectionMatrix.r[2].m128_f32[2];
	// put points into respective spots 
	XMFLOAT4 nonClippedEndPoint;
	nonClippedEndPoint.x = ((2.0f * ((tempRect.right - tempRect.left) / 2.0f) / ((tempRect.right - tempRect.left) / 2.0f)) - 1.0f) / tempProjection_Matrix.r[0].m128_f32[0];
	nonClippedEndPoint.y = (2.0f * ((tempRect.bottom - tempRect.top) / 2) / ((tempRect.bottom - tempRect.top) / 2) - 1.0f) / tempProjection_Matrix.r[1].m128_f32[1];
	nonClippedEndPoint.z = 2.0f * currentZDepth - 1.0f;
	nonClippedEndPoint.w = 1.0f;

	XMVECTOR clipPoint = XMVectorSet(nonClippedEndPoint.x, nonClippedEndPoint.y, nonClippedEndPoint.z, nonClippedEndPoint.w);
	// puts points into world space
	XMVECTOR endPoint = XMVector4Transform(clipPoint, tempProjection_Matrix);
	
	endPoint.m128_f32[3] = 1.0f / endPoint.m128_f32[3];

	endPoint.m128_f32[0] /= endPoint.m128_f32[3];
	endPoint.m128_f32[1] /= endPoint.m128_f32[3];
	endPoint.m128_f32[2] /= endPoint.m128_f32[3];


	return endPoint;
}
