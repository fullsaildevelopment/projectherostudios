#include "stdafx.h"
#include "Collision_System.h"


CCollisionSystem::CCollisionSystem()
{
}

CCollisionSystem::~CCollisionSystem()
{
}

bool CCollisionSystem::aabb_to_frustum(TAABB & aabb, frustum_t & frustum)
{
	for (int i = 0; i < 6; ++i) 
	{
		
		if (classify_aabb_to_plane(aabb, frustum[i]) != -1) 
		{
			continue;
		}
		else 
		{
			return false;
		}
	}
	return true;
}

/*
* ContainAABB():  Determins if the list of aabbs contains the aabb index being passed in
*
* Ins:                  nIndex
*
*
* Outs:
*
* Returns:              bool
*
* Mod. Date:              07/11/2018
* Mod. Initials:          AP
*/
bool CCollisionSystem::ContainAABB(int nIndex)
{
	for (list<TAABB>::iterator ptr = m_AAbb.begin(); ptr != m_AAbb.end(); ++ptr) 
	{
		if (ptr->m_IndexLocation == nIndex) 
		{
			return true;
		}
	}
	return false;
}
int CCollisionSystem::classify_aabb_to_plane(TAABB & aabb, plane_t&  plane)
{
	aabb_t aabbs;
	float3 min;
	float3 max;

	float3 aabbmin;
	aabbmin.x = aabb.m_dMinPoint.x;
	aabbmin.y = aabb.m_dMinPoint.y;
	aabbmin.z = aabb.m_dMinPoint.z;

	float3 aabbmax;
	aabbmax.x = aabb.m_dMaxPoint.x;
	aabbmax.y = aabb.m_dMaxPoint.y;
	aabbmax.z = aabb.m_dMaxPoint.z;

	min = aabbmin;
	max = aabbmax;
	aabbs.center = (min + max)*0.5;
	aabbs.extents.x = max.x - aabbs.center.x;
	aabbs.extents.y = max.y - aabbs.center.y;
	aabbs.extents.z = max.z - aabbs.center.z;
	float r = aabbs.extents.x*(plane.normal.x) + aabbs.extents.y*(plane.normal.y) + aabbs.extents.z*(plane.normal.z);
	sphere_t myspeher;
	myspeher.center = aabbs.center;
	myspeher.radius = r;
	return classify_sphere_to_plane(myspeher, plane);
}

bool CCollisionSystem::spehreCollison(sphere_t a, sphere_t b)
{
	float distance = sqrt((a.center.x - b.center.x) * (a.center.x - b.center.x) +
		(a.center.y - b.center.y) * (a.center.y - b.center.y) +
		(a.center.z - b.center.z) * (a.center.z - b.center.z));
	if (distance < (a.radius + b.radius)) 
	{
		return true;
	}
	// A and B are touching


	else 
	{
		return false;
	}
}

int CCollisionSystem::classify_sphere_to_plane(sphere_t & sphere, plane_t & plane)
{
	if ((sphere.center.x*plane.normal.x + sphere.center.y*plane.normal.y + sphere.center.z*plane.normal.z) - plane.offset > sphere.radius) 
	{
		return 1;
	}
	else if ((sphere.center.x*plane.normal.x + sphere.center.y*plane.normal.y + sphere.center.z*plane.normal.z) - plane.offset < -sphere.radius) 
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

bool CCollisionSystem::AiVisionCheck(frustum_t eyeSight,vector<int>* index)
{
	bool seesomething=false;
	for (int i = 0; i < AiFrustumCheck.size(); ++i) 
	{
	
		if (seesomething == false) 
		{
			seesomething = aabb_to_frustum(AiFrustumCheck[i], eyeSight);
			if(seesomething==true)
			index->push_back(AiFrustumCheck[i].m_IndexLocation);
		}
		else if (aabb_to_frustum(AiFrustumCheck[i], eyeSight) == true) 
		{
			index->push_back(AiFrustumCheck[i].m_IndexLocation);
		}

	}
	return seesomething;
}

void CCollisionSystem::TestThreading(TWorld * ptWorld, int nCurrentEntity, CGraphicsSystem* pcGraphicsSystem, CGraphicsSystem::TPrimalVertexBufferType* tTempVertexBuffer, XMMATRIX& tMyVertexBufferWorldMatrix, XMMATRIX* m_d3dPlayerMatrix, CPhysicsSystem* pcPhysicsSystem, CAISystem* pcAiSystem,int PlayerStartIndex, float& playerDamage, float& pirateDamage, float& prevHealth, float& fallingHealth, float& lerpTime, float in_MasterVolume, float in_SFXVolume, float in_MusicVolume, CAudioSystem* pcAudioSystem, XMMATRIX(*doorEventListener)(int), void(*doorEventChanger)(int))
{
	ptWorld->atAABB[nCurrentEntity].theeadmade = true;
	//ptWorld->atClayton[nCurrentEntity].health = 10;
//	float x = 0;
	//Change Door values
	if ((ptWorld->atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_AABB | COMPONENT_NONSTATIC | COMPONENT_TRIGGER) || ptWorld->atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)))
	{
		vector<int> otherCollisionsIndex;
		if (AABBtoAABBCollisionCheck(ptWorld->atAABB[nCurrentEntity], &otherCollisionsIndex) == true)
		{
			for (int i = 0; i < otherCollisionsIndex.size(); ++i)
			{
				if (ptWorld->atShaderID[otherCollisionsIndex[i]].m_nShaderID >= 20
					/*&& ptWorld->atInputMask[otherCollisionsIndex[i]].m_tnInputMask == 
						(COMPONENT_CLAYTON | COMPONENT_INPUTMASK)*/)
				{
					doorEventChanger(ptWorld->atShaderID[otherCollisionsIndex[i]].m_nShaderID);
					ptWorld->atWorldMatrix[otherCollisionsIndex[i]].worldMatrix = XMMatrixMultiply(doorEventListener(ptWorld->atShaderID[otherCollisionsIndex[i]].m_nShaderID), ptWorld->atWorldMatrix[otherCollisionsIndex[i]].worldMatrix);
				}
				if (ptWorld->atRigidBody[otherCollisionsIndex[i]].ground == true
					&& ptWorld->atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))
				{
				//	SHOOTING A WALL OR ANYTHING ELSE
					if (ptWorld->atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL | COMPONENT_FRIENDLY) ||
						ptWorld->atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL | COMPONENT_ENEMY))
					{

						RemoveAABBCollider(nCurrentEntity);

						pcGraphicsSystem->CleanD3DObject(ptWorld, nCurrentEntity);

						break;
					}
				}
				if (ptWorld->atRigidBody[otherCollisionsIndex[i]].ground == true
					&& ptWorld->atCollisionMask[nCurrentEntity].m_tnCollisionMask != (COMPONENT_COLLISIONMASK | COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))
				{


					ptWorld->atRigidBody[nCurrentEntity].totalForce = -ptWorld->atRigidBody[nCurrentEntity].velocity;
					tTempVertexBuffer->m_d3dWorldMatrix = pcPhysicsSystem->ResolveForces(&ptWorld->atRigidBody[nCurrentEntity], ptWorld->atWorldMatrix[nCurrentEntity].worldMatrix, false);
					ptWorld->atWorldMatrix[nCurrentEntity].worldMatrix = WalkingThrewObjectCheck(tTempVertexBuffer->m_d3dWorldMatrix, ptWorld->atAABB[otherCollisionsIndex[i]], ptWorld->atAABB[nCurrentEntity]);
					ptWorld->atWorldMatrix[nCurrentEntity].worldMatrix = tTempVertexBuffer->m_d3dWorldMatrix;
				}
				if (ptWorld->atRigidBody[nCurrentEntity].ground == false
					&& ptWorld->atRigidBody[nCurrentEntity].wall == false
					&& ptWorld->atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)
					&& (ptWorld->atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_STATIC)
						|| (ptWorld->atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))))
				{
					tTempVertexBuffer->m_d3dWorldMatrix = WalkingThrewObjectCheck(ptWorld->atWorldMatrix[nCurrentEntity].worldMatrix, ptWorld->atAABB[otherCollisionsIndex[i]], ptWorld->atAABB[nCurrentEntity]);
					ptWorld->atWorldMatrix[nCurrentEntity].worldMatrix = tTempVertexBuffer->m_d3dWorldMatrix;
					if (ptWorld->atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK))
					{
						*m_d3dPlayerMatrix = ptWorld->atWorldMatrix[nCurrentEntity].worldMatrix;
					}

				}
				if (ptWorld->atRigidBody[nCurrentEntity].ground == false
					&& ptWorld->atRigidBody[otherCollisionsIndex[i]].ground == false
					&& ptWorld->atRigidBody[nCurrentEntity].wall == false
					&& ptWorld->atCollisionMask[nCurrentEntity].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_TRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC)
					&& (ptWorld->atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_STATIC))
					|| ptWorld->atCollisionMask[otherCollisionsIndex[i]].m_tnCollisionMask == (COMPONENT_COLLISIONMASK | COMPONENT_NONTRIGGER | COMPONENT_AABB | COMPONENT_NONSTATIC))
				{

					if (ptWorld->atAIMask[otherCollisionsIndex[i]].m_tnAIMask > 1)
					{
						// SHOOTING AN AI ONLY
						if (ptWorld->atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL | COMPONENT_FRIENDLY))
						{
							if (ptWorld->atAiHeath[otherCollisionsIndex[i]].heath <= 0)
							{
								pcAiSystem->SetNumberOfAI(pcAiSystem->GetNumberOfAI() - 1);

								if (ptWorld->atClip[nCurrentEntity].gunIndex != -1)
								{


									RemoveAABBCollider(nCurrentEntity);



									pcGraphicsSystem->CleanD3DObject(ptWorld, nCurrentEntity);


								}
								RemoveAABBCollider(otherCollisionsIndex[i]);
								pcGraphicsSystem->CleanD3DObject(ptWorld, otherCollisionsIndex[i]);
								pcGraphicsSystem->CleanD3DObject(ptWorld, ptWorld->atAIMask[otherCollisionsIndex[i]].GunIndex);

							}
							else
							{



								RemoveAABBCollider(nCurrentEntity);

								pcGraphicsSystem->CleanD3DObject(ptWorld, nCurrentEntity);
								ptWorld->atAiHeath[otherCollisionsIndex[i]].heath -= playerDamage;

								//pcAiSystem->AddAiInCombat(nCurrentEntity);
								ptWorld->atActiveAI[otherCollisionsIndex[i]].active = true;
								for (int activate = 0; activate < ptWorld->atActiveAI[otherCollisionsIndex[i]].NoctifyOtherAi.size(); ++activate) {
									ptWorld->atActiveAI[ptWorld->atActiveAI[otherCollisionsIndex[i]].NoctifyOtherAi[activate]].active = true;
									ptWorld->atAIVision[ptWorld->atActiveAI[otherCollisionsIndex[i]].NoctifyOtherAi[activate]].indexLookingAt = PlayerStartIndex;

								}
								ptWorld->atAIVision[otherCollisionsIndex[i]].indexLookingAt = PlayerStartIndex;
#if MUSIC_ON
								pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_HURT_SCYLIAN, pcAudioSystem->m_Scylian_Hurt);
								pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::SFX_VOLUME, in_SFXVolume);
#endif

								if (ptWorld->atAiHeath[otherCollisionsIndex[i]].heath <= 0)
								{
									pcAiSystem->SetNumberOfAI(pcAiSystem->GetNumberOfAI() - 1);
									RemoveAABBCollider(otherCollisionsIndex[i]);
									pcGraphicsSystem->CleanD3DObject(ptWorld, otherCollisionsIndex[i]);
									pcGraphicsSystem->CleanD3DObject(ptWorld, ptWorld->atAIMask[otherCollisionsIndex[i]].GunIndex);

									pcGraphicsSystem->CleanD3DObject(ptWorld, otherCollisionsIndex[i] + 1);
									pcGraphicsSystem->CleanD3DObject(ptWorld, otherCollisionsIndex[i] + 2);
								}

							}
						}
					}
					else if (ptWorld->atInputMask[otherCollisionsIndex[i]].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK))
					{
					//	SHOOTING THE PLAYER ONLY
						if (ptWorld->atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL | COMPONENT_ENEMY))
						{




							RemoveAABBCollider(nCurrentEntity);




							pcGraphicsSystem->CleanD3DObject(ptWorld, nCurrentEntity);

							if (prevHealth == 0)
							{
								prevHealth = ptWorld->atClayton[otherCollisionsIndex[i]].health;
							}
							else
							{
								prevHealth = fallingHealth;
							}

							lerpTime = 0;

							//	ptWorld->atClayton[otherCollisionsIndex[i]].health -= pirateDamage;
							ptWorld->atClayton[otherCollisionsIndex[i]].health -= pirateDamage;
#if MUSIC_ON
							pcAudioSystem->SendSoundsToEngine(AK::EVENTS::PLAY_HURT_HUMAN, pcAudioSystem->m_Human_Hurt);
							pcAudioSystem->SetRTPCVolume(AK::GAME_PARAMETERS::SFX_VOLUME, in_SFXVolume);
#endif
							cout << "turtle";

							if (ptWorld->atClayton[otherCollisionsIndex[i]].health <= 0)
							{
								/*	GameOver = true;
									InitializeEndScreen(false);*/
							}
						}


					}
					if (ptWorld->atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK)) 
					{
						if (ptWorld->atProjectiles[otherCollisionsIndex[i]].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL | COMPONENT_ENEMY))
						{




							RemoveAABBCollider(otherCollisionsIndex[i]);




							pcGraphicsSystem->CleanD3DObject(ptWorld, otherCollisionsIndex[i]);


							//	ptWorld->atClayton[otherCollisionsIndex[i]].health -= pirateDamage;
							ptWorld->atClayton[nCurrentEntity].health -= playerDamage;
							cout << "turtle";

							if (ptWorld->atClayton[nCurrentEntity].health <= 0)
							{
								/*	GameOver = true;
								InitializeEndScreen(false);*/
							}
						}
					 }
				//	Wasn't being hit at all so it's been commented.
					if (ptWorld->atProjectiles[nCurrentEntity].m_tnProjectileMask == (COMPONENT_PROJECTILESMASK | COMPONENT_METAL))
					{
						if (ptWorld->atClip[nCurrentEntity].gunIndex != -1)
						{


						RemoveAABBCollider(nCurrentEntity);


						pcGraphicsSystem->CleanD3DObject(ptWorld, ptWorld->atAIMask[nCurrentEntity].GunIndex);
						pcGraphicsSystem->CleanD3DObject(ptWorld, nCurrentEntity);

						}
					}

				}
				if (otherCollisionsIndex[i] == 933) {
					float x = 0;
				}
				if (ptWorld->atInputMask[nCurrentEntity].m_tnInputMask == (COMPONENT_CLAYTON | COMPONENT_INPUTMASK) && ptWorld->atAIMask[otherCollisionsIndex[i]].m_tnAIMask == (COMPONENT_AIMASK | COMPONENT_COVERTRIGGER))
				{
					pcAiSystem->MoveAiToCoverLocation(ptWorld->atCoverTrigger[otherCollisionsIndex[i]], ptWorld, PlayerStartIndex);

				}
			}
		/*	if (ptWorld->atClayton[PlayerStartIndex].health <= 0)
			{
				GameOver = true;
				InitializeEndScreen(false);
			}*/

		}
	}
	ptWorld->atAABB[nCurrentEntity].theeadmade = false;
	//std::async(removeThread, std::this_thread::get_id());
}







/*
* classify_aabb_to_aabb():  Determines if the two AABB are coliding or not
*
* Ins:                  two AABBs
*
*
* Outs:
*
* Returns:              bool
*
* Mod. Date:              07/11/2018
* Mod. Initials:          AP
*/

bool CCollisionSystem::classify_aabb_to_aabb(TAABB aabb1, TAABB aabb2)
{
	if (aabb1.m_dMinPoint.x <= aabb2.m_dMaxPoint.x) 
	{
		float x = 0;
	}
	if (aabb1.m_dMaxPoint.x >= aabb2.m_dMinPoint.x)
	{
		float x = 0;
	}
	if (aabb1.m_dMinPoint.y <= aabb2.m_dMaxPoint.y) 
	{
		float x = 0;
	}
	if (aabb1.m_dMaxPoint.y >= aabb2.m_dMinPoint.y) 
	{
		float x = 0;
	}
	if (aabb1.m_dMinPoint.z <= aabb2.m_dMaxPoint.z) 
	{
		float x = 0;
	}
	if (aabb1.m_dMaxPoint.z >= aabb2.m_dMinPoint.z) 
	{
		float x = 0;
	}
	return (aabb1.m_dMinPoint.x <= aabb2.m_dMaxPoint.x&&aabb1.m_dMaxPoint.x >= aabb2.m_dMinPoint.x) &&
		(aabb1.m_dMinPoint.y <= aabb2.m_dMaxPoint.y&&aabb1.m_dMaxPoint.y >= aabb2.m_dMinPoint.y) &&
		(aabb1.m_dMinPoint.z <= aabb2.m_dMaxPoint.z&&aabb1.m_dMaxPoint.z >= aabb2.m_dMinPoint.z);
}
bool CCollisionSystem::IsLineInBox(XMVECTOR startPoint, XMVECTOR endPoint, XMMATRIX worldMatrix, TAABB boxclider, float* distance)
{
	// Put line in box space
	XMMATRIX MInv = XMMatrixInverse(NULL, worldMatrix);
	XMVECTOR LB1 = XMVector3Transform(startPoint, MInv);
	XMVECTOR LB2 = XMVector3Transform(endPoint, MInv);

	// Get line midpoint and extent
	XMVECTOR LMid = (LB1 + LB2) * 0.5f;
	XMVECTOR L = (LB1 - LMid);
	XMVECTOR LExt;
	LExt.m128_f32[0] = fabs(L.m128_f32[0]);
	LExt.m128_f32[1] = fabs(L.m128_f32[1]);
	LExt.m128_f32[2] = fabs(L.m128_f32[2]);
	XMVECTOR CenterofBox2;
	CenterofBox2.m128_f32[0] = (boxclider.m_dMaxPoint.x + boxclider.m_dMinPoint.x) * 0.5f;
	CenterofBox2.m128_f32[1] = (boxclider.m_dMaxPoint.y + boxclider.m_dMinPoint.y) * 0.5f;
	CenterofBox2.m128_f32[2] = (boxclider.m_dMaxPoint.z + boxclider.m_dMinPoint.z) * 0.5f;
	XMVECTOR CenterofBox;
	CenterofBox.m128_f32[0] = boxclider.m_dMinPoint.x;
	CenterofBox.m128_f32[1] = boxclider.m_dMinPoint.y;
	CenterofBox.m128_f32[2] = boxclider.m_dMinPoint.z;
	CenterofBox = CenterofBox - CenterofBox2;
	CenterofBox.m128_f32[0] = fabs(CenterofBox.m128_f32[0]);
	CenterofBox.m128_f32[1] = fabs(CenterofBox.m128_f32[1]);
	CenterofBox.m128_f32[2] = fabs(CenterofBox.m128_f32[2]);

	// Use Separating Axis Test
	// Separation vector from box center to line center is LMid, since the line is in box space
	if (fabs(LMid.m128_f32[0]) > (CenterofBox.m128_f32[0] + LExt.m128_f32[0])) return false;
	if (fabs(LMid.m128_f32[1]) > (CenterofBox.m128_f32[1] + LExt.m128_f32[1])) return false;
	if (fabs(LMid.m128_f32[2]) > (CenterofBox.m128_f32[2] + LExt.m128_f32[2])) return false;
	// Crossproducts of line and each axis
	//if ( fabs( LMid.y * L.z - LMid.z * L.y)  >  (m_Extent.y * LExt.z + m_Extent.z * LExt.y) ) return false;
	if (fabs(LMid.m128_f32[1] * L.m128_f32[2] - LMid.m128_f32[2] * L.m128_f32[1]) > (CenterofBox.m128_f32[1] * LExt.m128_f32[2] + CenterofBox.m128_f32[2] * LExt.m128_f32[1])) return false;
	//	if ( fabs( LMid.x * L.z - LMid.z * L.x)  >  (m_Extent.x * LExt.z + m_Extent.z * LExt.x) ) return false;
	if (fabs(LMid.m128_f32[0] * L.m128_f32[2] - LMid.m128_f32[2] * L.m128_f32[0]) > (CenterofBox.m128_f32[0] * LExt.m128_f32[2] + CenterofBox.m128_f32[2] * LExt.m128_f32[0])) return false;
	//if ( fabs( LMid.x * L.y - LMid.y * L.x)  >  (m_Extent.x * LExt.y + m_Extent.y * LExt.x) ) return false;
	if (fabs(LMid.m128_f32[0] * L.m128_f32[1] - LMid.m128_f32[1] * L.m128_f32[0]) > (CenterofBox.m128_f32[0] * LExt.m128_f32[1] + CenterofBox.m128_f32[1] * LExt.m128_f32[0])) return false;
	// No separating axis, the line intersects
	float minDistance;
	float maxDistance;
	XMVECTOR min;
	min.m128_f32[0] = boxclider.m_dMinPoint.x;
	min.m128_f32[1] = boxclider.m_dMinPoint.y;
	min.m128_f32[2] = boxclider.m_dMinPoint.z;
	XMVECTOR max;
	max.m128_f32[0] = boxclider.m_dMaxPoint.x;
	max.m128_f32[1] = boxclider.m_dMaxPoint.y;
	max.m128_f32[2] = boxclider.m_dMaxPoint.z;

	minDistance = sqrtf(
		((min.m128_f32[0] - startPoint.m128_f32[0])*(min.m128_f32[0] - startPoint.m128_f32[0])) +
		((min.m128_f32[1] - startPoint.m128_f32[1])*(min.m128_f32[1] - startPoint.m128_f32[1])) +
		((min.m128_f32[2] - startPoint.m128_f32[2])*(min.m128_f32[2] - startPoint.m128_f32[2]))
	);
	maxDistance = sqrtf(
		((max.m128_f32[0] - startPoint.m128_f32[0])*(max.m128_f32[0] - startPoint.m128_f32[0])) +
		((max.m128_f32[1] - startPoint.m128_f32[1])*(max.m128_f32[1] - startPoint.m128_f32[1])) +
		((max.m128_f32[2] - startPoint.m128_f32[2])*(max.m128_f32[2] - startPoint.m128_f32[2]))
	);
	if (minDistance > maxDistance) {
		*distance = maxDistance;
	}
	else {
		*distance = minDistance;
	}
	return true;
}
bool CCollisionSystem::intersectRayAABox2(XMVECTOR startPoint, XMVECTOR endPoint, TAABB boxclide)
{
	XMVECTOR T_1, T_2; // vectors to hold the T-values for every direction
	double t_near = -DBL_MAX; // maximums defined in float.h
	double t_far = DBL_MAX;
	XMVECTOR raydirecon;
	XMVECTOR minopint;
	minopint.m128_f32[0] = boxclide.m_dMinPoint.x;
	minopint.m128_f32[1] = boxclide.m_dMinPoint.y;
	minopint.m128_f32[2] = boxclide.m_dMinPoint.z;
	XMVECTOR maxPoint;
	maxPoint.m128_f32[0] = boxclide.m_dMaxPoint.x;
	maxPoint.m128_f32[1] = boxclide.m_dMaxPoint.y;
	maxPoint.m128_f32[2] = boxclide.m_dMaxPoint.z;
	raydirecon = endPoint - startPoint;
	for (int i = 0; i < 3; i++) { //we test slabs in every direction
		if (raydirecon.m128_f32[i] == 0) { // ray parallel to planes in this direction
			if ((startPoint.m128_f32[i] < minopint.m128_f32[i]) || (startPoint.m128_f32[i] >maxPoint.m128_f32[i])) {
				return false; // parallel AND outside box : no intersection possible
			}
		}
		else { // ray not parallel to planes in this direction
			T_1.m128_f32[i] = (minopint.m128_f32[i] - startPoint.m128_f32[i]) / raydirecon.m128_f32[i];
			T_2.m128_f32[i] = (maxPoint.m128_f32[i] - startPoint.m128_f32[i]) / raydirecon.m128_f32[i];

			if (T_1.m128_f32[i] > T_2.m128_f32[i]) { // we want T_1 to hold values for intersection with near plane
				swap(T_1, T_2);
			}
			if (T_1.m128_f32[i] > t_near) {
				t_near = T_1.m128_f32[i];
			}
			if (T_2.m128_f32[i] < t_far) {
				t_far = T_2.m128_f32[i];
			}
			if ((t_near > t_far) || (t_far < 0)) {
				return false;
			}
		}
	}
	//tnear = t_near; tfar = t_far; // put return values in place
	return true; // if we made it here, there was an intersection - YAY
}
XMMATRIX CCollisionSystem::WalkingThrewObjectCheck(XMMATRIX worldPos, TAABB otherCollision, TAABB currentCollision)
{
	XMMATRIX D3DMatrix = worldPos;
	TAABB UpdateCollision = currentCollision;
	float xRight = abs(otherCollision.m_dMaxPoint.x - currentCollision.m_dMinPoint.x);
	float xLeft = abs(otherCollision.m_dMinPoint.x - currentCollision.m_dMaxPoint.x);
	float yTop = abs(otherCollision.m_dMinPoint.y - currentCollision.m_dMaxPoint.y);
	float yBottom = abs(otherCollision.m_dMaxPoint.y - currentCollision.m_dMinPoint.y);
	float zFar = abs(otherCollision.m_dMaxPoint.z - currentCollision.m_dMinPoint.z);
	float zClose = abs(otherCollision.m_dMinPoint.z - currentCollision.m_dMaxPoint.z);
	if (xRight < xLeft&&
		xRight < yTop&&

		xRight < yBottom&&
		xRight < zFar
		&&xRight < zClose
		) 
	{
		int numberofLoops = 0;
		while (classify_aabb_to_aabb(otherCollision, UpdateCollision) && numberofLoops<2000)
		{
			//d3d_ResultMatrix = pcGraphicsSystem->SetDefaultWorldPosition();;
			XMMATRIX moveback;
			moveback = D3DMatrix;
			moveback.r[3].m128_f32[0] += 0.01f;
			D3DMatrix = moveback;

			UpdateCollision = updateAABB(D3DMatrix, UpdateCollision);
			numberofLoops += 1;

		}
	}
	else if (xLeft < xRight
		&&xLeft < yTop
		&&xLeft < yBottom
		&&xLeft < zFar
		&&xLeft < zClose) {
		int numberofLoops = 0;
		while (classify_aabb_to_aabb(otherCollision, UpdateCollision) && numberofLoops<2000)
		{
			//d3d_ResultMatrix = pcGraphicsSystem->SetDefaultWorldPosition();;
			XMMATRIX moveback;
			moveback = D3DMatrix;
			moveback.r[3].m128_f32[0] -= 0.01f;
			D3DMatrix = moveback;
			UpdateCollision = updateAABB(D3DMatrix, UpdateCollision);
			numberofLoops += 1;

		}
	}
	else if (yTop < xLeft&&
		yTop < xRight&&
		yTop < yBottom
		&&yTop < zFar
		&&yTop < zClose) 
	{
		int numberofLoops = 0;
		while (classify_aabb_to_aabb(otherCollision, UpdateCollision) && numberofLoops<2000)
		{
			//d3d_ResultMatrix = pcGraphicsSystem->SetDefaultWorldPosition();;
			XMMATRIX moveback;
			moveback = D3DMatrix;
			moveback.r[3].m128_f32[1] -= 0.01f;
			D3DMatrix = moveback;
			UpdateCollision = updateAABB(D3DMatrix, UpdateCollision);
			numberofLoops += 1;

		
		}
	}
	else if (yBottom < xLeft
		&&yBottom < xRight
		&&yBottom < yTop
		&&yBottom < zClose
		&&yBottom < zFar) 
	{
		int numberofLoops = 0;
		while (classify_aabb_to_aabb(otherCollision, UpdateCollision) && numberofLoops<2000)
		{
			//d3d_ResultMatrix = pcGraphicsSystem->SetDefaultWorldPosition();;
			XMMATRIX moveback;
			moveback = D3DMatrix;
			moveback.r[3].m128_f32[1] += 0.01f;
			D3DMatrix = moveback;
			UpdateCollision = updateAABB(D3DMatrix, UpdateCollision);
			numberofLoops += 1;

		}
	}
	else if (zFar < zClose
		&&zFar < xLeft
		&&zFar < xRight
		&&zFar < yBottom
		&&zFar < yTop)
	{
		int numberofLoops = 0;
		while (classify_aabb_to_aabb(otherCollision, UpdateCollision) && numberofLoops<2000)
		{
			//d3d_ResultMatrix = pcGraphicsSystem->SetDefaultWorldPosition();;
			XMMATRIX moveback;
			moveback = D3DMatrix;
			moveback.r[3].m128_f32[2] += 0.01f;
			D3DMatrix = moveback;
			UpdateCollision = updateAABB(D3DMatrix, UpdateCollision);
			numberofLoops += 1;

		}
	}
	else if (zClose < zFar
		&&zClose < xLeft
		&&zClose < xRight
		&&zClose < yBottom
		&&zClose < yTop) {
		int numberofLoops = 0;
		while (classify_aabb_to_aabb(otherCollision, UpdateCollision)&&numberofLoops<2000)
		{
			//d3d_ResultMatrix = pcGraphicsSystem->SetDefaultWorldPosition();;
			XMMATRIX moveback;
			moveback = D3DMatrix;
			moveback.r[3].m128_f32[2] -= 0.01f;
			D3DMatrix = moveback;
			UpdateCollision = updateAABB(D3DMatrix, UpdateCollision);
			numberofLoops += 1;

		}
	}
	return D3DMatrix;
}
/*
* replaceAABB():  finds an AABB if able in the list and then replaces the data with the new AABB
*
* Ins:                  Index to AABB in list
*						AABB to replace it with
*
*
* Outs:
*
* Returns:              bool
*
* Mod. Date:              07/11/2018
* Mod. Initials:          AP
*/
bool CCollisionSystem::replaceAABB(int nIndex, TAABB m_AABB2)
{
	if (nIndex == 0) 
	{
		float x = 0;
	}
	for (list<TAABB>::iterator ptr = m_AAbb.begin(); ptr != m_AAbb.end(); ++ptr) 
	{
		if (nIndex == ptr->m_IndexLocation)
		{
			for (int i = 0; i < AiFrustumCheck.size(); ++i)
			{
				if (nIndex == AiFrustumCheck[i].m_IndexLocation)
				{
					AiFrustumCheck[i].m_dMaxPoint = m_AABB2.m_dMaxPoint;
					AiFrustumCheck[i].m_dMinPoint = m_AABB2.m_dMinPoint;

				}
			}
			for (list<TAABB>::iterator ptr = m_AAbb.begin(); ptr != m_AAbb.end(); ++ptr)
			{
				if (nIndex == ptr->m_IndexLocation) 
				{
					ptr->m_dMaxPoint = m_AABB2.m_dMaxPoint;
					ptr->m_dMinPoint = m_AABB2.m_dMinPoint;
					return true;
				}
			}
		}
	}

	return false;
}
int inline GetIntersection(float fDst1, float fDst2, XMVECTOR P1, XMVECTOR P2, XMVECTOR &Hit) 
{
	if ((fDst1 * fDst2) >= 0.0f) 
	{
		return 0;
	}
	if (fDst1 == fDst2) 
	{
		return 0;
	}
	Hit = P1 + (P2 - P1) * (-fDst1 / (fDst2 - fDst1));
	return 1;
}
TAABB CCollisionSystem::updateAABB(XMMATRIX worldMatrix, TAABB aabb)
{
	XMMATRIX newWorldMatrix = XMMatrixIdentity();
	newWorldMatrix.r[3].m128_f32[0] = worldMatrix.r[3].m128_f32[0];
	newWorldMatrix.r[3].m128_f32[1] = worldMatrix.r[3].m128_f32[1];
	newWorldMatrix.r[3].m128_f32[2] = worldMatrix.r[3].m128_f32[2];
	//newWorldMatrix.r[2].m128_f32[2] = worldMatrix.r[2].m128_f32[2];

	XMVECTOR max;
	max.m128_f32[0] = aabb.m_dMaxPointOrginal.x;
	max.m128_f32[1] = aabb.m_dMaxPointOrginal.y;
	max.m128_f32[2] = aabb.m_dMaxPointOrginal.z;
	max.m128_f32[3] = 1;
	max = XMVector4Transform(max, newWorldMatrix);

	XMVECTOR min;
	min.m128_f32[0] = aabb.m_dMinPointOrginal.x;
	min.m128_f32[1] = aabb.m_dMinPointOrginal.y;
	min.m128_f32[2] = aabb.m_dMinPointOrginal.z;
	min.m128_f32[3] = 1;
	min = XMVector4Transform(min, newWorldMatrix);
	TAABB newaabb;
	newaabb = aabb;
	newaabb.m_dMaxPoint.x = max.m128_f32[0];
	newaabb.m_dMaxPoint.y = max.m128_f32[1];
	newaabb.m_dMaxPoint.z = max.m128_f32[2];

	newaabb.m_dMinPoint.x = min.m128_f32[0];
	newaabb.m_dMinPoint.y = min.m128_f32[1];
	newaabb.m_dMinPoint.z = min.m128_f32[2];
	newaabb.m_dMinPointOrginal = aabb.m_dMinPointOrginal;
	newaabb.m_dMaxPointOrginal = aabb.m_dMaxPointOrginal;

	if (replaceAABB(aabb.m_IndexLocation, newaabb) == true)
	{
		return newaabb;
	}
	else 
	{
		return aabb;
	}
}
TAABB CCollisionSystem::createAABBS(std::vector<XMFLOAT3> verticies, TAABB AABBDATA)
{
	int size = (int)verticies.size();
	if (size == 0) { return TAABB(); }
	TAABB aabb;
	aabb.m_MaterialType = AABBDATA.m_MaterialType;
	aabb.m_SceneChange = AABBDATA.m_SceneChange;

	aabb.m_dMaxPoint.x = verticies[0].x;
	aabb.m_dMaxPoint.y = verticies[0].y;
	aabb.m_dMaxPoint.z = verticies[0].z;
	aabb.m_dMaxPointOrginal.x = verticies[0].x;
	aabb.m_dMaxPointOrginal.y = verticies[0].y;
	aabb.m_dMaxPointOrginal.z = verticies[0].z;
	aabb.m_dMinPointOrginal.x = verticies[0].x;
	aabb.m_dMinPointOrginal.y = verticies[0].y;
	aabb.m_dMinPointOrginal.z = verticies[0].z;

	aabb.m_dMinPoint.x = verticies[0].x;
	aabb.m_dMinPoint.y = verticies[0].y;
	aabb.m_dMinPoint.z = verticies[0].z;

	for (int i = 1; i < size; ++i) 
	{
		if (verticies[i].x > aabb.m_dMaxPoint.x) 
		{
			aabb.m_dMaxPoint.x = verticies[i].x;
			aabb.m_dMaxPointOrginal.x = aabb.m_dMaxPoint.x;
		}
		if (verticies[i].x < aabb.m_dMinPoint.x) 
		{
			aabb.m_dMinPoint.x = verticies[i].x;
			aabb.m_dMinPointOrginal.x = aabb.m_dMinPoint.x;
		}

		if (verticies[i].y > aabb.m_dMaxPoint.y) 
		{
			aabb.m_dMaxPoint.y = verticies[i].y;
			aabb.m_dMaxPointOrginal.y = aabb.m_dMaxPoint.y;
		}
		if (verticies[i].y < aabb.m_dMinPoint.y) 
		{
			aabb.m_dMinPoint.y = verticies[i].y;
			aabb.m_dMinPointOrginal.y = aabb.m_dMinPoint.y;
		}

		if (verticies[i].z > aabb.m_dMaxPoint.z) 
		{
			aabb.m_dMaxPoint.z = verticies[i].z;
			aabb.m_dMaxPointOrginal.z = aabb.m_dMaxPoint.z;
		}
		if (verticies[i].z < aabb.m_dMinPoint.z) 
		{
			aabb.m_dMinPoint.z = verticies[i].z;
			aabb.m_dMinPointOrginal.z = aabb.m_dMinPoint.z;
		}
	}
	return aabb;
}

/*
* AddAABBCollider():  Adds an AABB to the list if it is not already in the list
*
* Ins:                  Index to AABB in list
*						AABB to add
*
*
* Outs:
*
* Returns:              bool
*
* Mod. Date:              07/11/2018
* Mod. Initials:          AP
*/
int CCollisionSystem::AddAABBCollider(TAABB m_AABB2, int nIndex)
{
	if (ContainAABB(nIndex) == false) 
	{
		m_AABB2.m_IndexLocation = nIndex;
		m_AAbb.push_back(m_AABB2);

		return (int)m_AAbb.size();
	}
	else
	{
		return 0;
	}
}
int CCollisionSystem::AddAiVisioNCheck(TAABB m_AABB2, int nIndex)
{
	m_AABB2.m_IndexLocation = nIndex;
	AiFrustumCheck.push_back(m_AABB2);
	return 1;
}
/*
* RemoveAABBCollider():  Adds an AABB to the list if it is not already in the list
*
* Ins:                  Index to AABB in list
*						AABB to add
*
*
* Outs:
*
* Returns:              bool
*
* Mod. Date:              07/11/2018
* Mod. Initials:          AP
*/
bool CCollisionSystem::RemoveAABBCollider(int nIndex)
{
	for (list<TAABB>::iterator ptr = m_AAbb.begin(); ptr != m_AAbb.end(); ++ptr) 
	{
		if (ptr->m_IndexLocation == nIndex) 
		{
			m_AAbb.erase(ptr);
			cout << "cat";
			return true;
		}
	}
	return false;
}

bool CCollisionSystem::AABBtoAABBCollisionCheck(TAABB m_AABB2, vector<int>* m_OtherColision)
{
	if (m_AABB2.m_IndexLocation == 928) 
	{
		float x = 0;
	}

	for (list<TAABB>::iterator ptr = m_AAbb.begin(); ptr != m_AAbb.end(); ++ptr) 
	{
		if (m_AABB2.m_IndexLocation != ptr->m_IndexLocation) 
		{
			if (ptr->m_IndexLocation == 933)
			{
				float x = 0;
			}
			if (classify_aabb_to_aabb(m_AABB2, *ptr) == true) 
			{
				
				m_OtherColision->push_back(ptr->m_IndexLocation);
			}
		}
	}
	if (m_OtherColision->size() != 0)
	{
		return true;
	}

	return false;
}