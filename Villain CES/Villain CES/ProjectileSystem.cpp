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

XMVECTOR CProjectileSystem::FindBeamEndPoint(XMVECTOR pointInScreenSpace, XMMATRIX in_WorldMatrix, XMMATRIX in_ViewMatrix, XMMATRIX in_ProjectionMatrix, HWND in_WindowHandle ,D3D11_VIEWPORT m_d3dViewport)
{   
	//Vector of Position in the middle of screen in screen space

	XMVECTOR endPoint;
	// puts screen space into object space  & the reason i pass a identity matrix is that way this coordinate stays in world space because if you pass it your changed world matrix then it would usually output the point in object space
	endPoint = XMVector3Unproject(pointInScreenSpace, 
		m_d3dViewport.TopLeftX, m_d3dViewport.TopLeftY,
		m_d3dViewport.Width, m_d3dViewport.Height, 
		0, 1, 
		in_ProjectionMatrix, in_ViewMatrix, XMMatrixIdentity());

	//endPoint = XMVector3Normalize(endPoint);
	//additional notes:
	//next step is to draw from start point from gun matrix to this returned point and check out directx intersect method to see if it hits a drawn triangle
	// next would be to run through abbs to try and get entity's srv texture

	return endPoint;
}
