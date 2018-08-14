
#include "AI_System.h"

CAISystem::CAISystem()
	:open([](PlannerNode* const& c, PlannerNode* const& t) {return c->finalCost > t->finalCost; })
{

}

CAISystem::~CAISystem()
{
}






int CAISystem::calculate_frustum(TWorld * ptWorld,frustum_t & frustum, float4x4 camera_transform, float fov, float aspect_ratio, float near_offset, float far_offset, int parentWorldMatrixIndex, float xoffset, float yoffset, float zoffset)
{
	float4 nearcenteTEMPTr = camera_transform.row4 + camera_transform.row3*near_offset;
	float4 farcenterTEMPT = camera_transform.row4 + camera_transform.row3*far_offset;
	float3 nearcenter;
	nearcenter.x = nearcenteTEMPTr.x;
	nearcenter.y = nearcenteTEMPTr.y;
	nearcenter.z = nearcenteTEMPTr.z;
	float3 farcenter;
	farcenter.x = farcenterTEMPT.x;
	farcenter.y = farcenterTEMPT.y;
	farcenter.z = farcenterTEMPT.z;
	float Hnear = 2 * tan(fov / 2.0f)*near_offset;
	float Hfar = 2 * tan(fov / 2.0f)*far_offset;
	float Wnear = Hnear * aspect_ratio;
	float Wfar = Hfar * aspect_ratio;
	float3 fartopleft;
	float3 fartopright;
	float3 farbottomleft;
	float3 farbottomright;
	float3 neartopleft;
	float3 neartopright;
	float3 nearbottomleft;
	float3 nearbottomright;
	float3 black;
	black.x = 0;
	black.y = 0;
	black.z = 0;
	float3 camY;
	camY.x = camera_transform.row2.x;
	camY.y = camera_transform.row2.y;
	camY.z = camera_transform.row2.z;
	float3 camX;
	camX.x = camera_transform.row1.x;
	camX.y = camera_transform.row1.y;
	camX.z = camera_transform.row1.z;

	fartopleft = farcenter + camY * (Hfar*0.5f) - camX * (Wfar*0.5);
	fartopright = farcenter + camY * (Hfar*0.5f) + camX * (Wfar*0.5);
	farbottomleft = farcenter - camY * (Hfar*0.5f) - camX * (Wfar*0.5);
	farbottomright = farcenter - camY * (Hfar*0.5f) + camX * (Wfar*0.5);


	neartopleft = nearcenter + camY * (Hnear*0.5f) - camX * (Wnear*0.5);
	neartopright = nearcenter + camY * (Hnear*0.5f) + camX * (Wnear*0.5);
	nearbottomleft = nearcenter - camY * (Hnear*0.5f) - camX * (Wnear*0.5);
	nearbottomright = nearcenter - camY * (Hnear*0.5f) + camX * (Wnear*0.5);
	frustum[0] = calculate_plane(farbottomright, farbottomleft, fartopleft);
	float3 center = farbottomright + farbottomleft + fartopright + fartopleft;
	center.x /= 4.0f;
	center.y /= 4.0f;
	center.z /= 4.0f;
	float3 point2;

	point2 = center + (frustum[0].normal * 5);
	float3 color;
	color.x = 1;
	//render->add_debug_line(center, point2, color);



	frustum[1] = calculate_plane(nearbottomright, farbottomright, fartopright);
	center = farbottomright + fartopright + nearbottomright + neartopright;
	center.x /= 4.0f;
	center.y /= 4.0f;
	center.z /= 4.0f;
	point2;
	point2 = center + (frustum[1].normal * 5);

	//render->add_debug_line(center, point2, color);
	frustum[2] = calculate_plane(nearbottomleft, nearbottomright, neartopright);
	center = nearbottomright + nearbottomleft + neartopleft + neartopright;
	center.x /= 4.0f;
	center.y /= 4.0f;
	center.z /= 4.0f;
	point2;
	point2 = center + (frustum[2].normal * 2);

//	render->add_debug_line(center, point2, color);
	frustum[3] = calculate_plane(farbottomleft, nearbottomleft, neartopleft);
	//center = neartopleft + nearbottomleft + fartopleft + farbottomleft;
	//center.x /= 4.0f;
	//center.y /= 4.0f;
	//center.z /= 4.0f;
	//point2;
	//point2 = center + (frustum[3].normal * 2);

	//render->add_debug_line(center, point2, color);
	frustum[4] = calculate_plane(fartopright, fartopleft, neartopleft);
	//center = fartopleft + fartopright + neartopright + neartopleft;
	//center.x /= 4.0f;
	//center.y /= 4.0f;
	//center.z /= 4.0f;
	//point2;
	//point2 = center + (frustum[4].normal * 2);

//	render->add_debug_line(center, point2, color);
	frustum[5] = calculate_plane(nearbottomright, nearbottomleft, farbottomleft);
	//center = nearbottomleft + nearbottomright + farbottomleft + farbottomright;
	//center.x /= 4.0f;
	//center.y /= 4.0f;
	//center.z /= 4.0f;
	//point2;
	//point2 = center + (frustum[5].normal * 2);
	//XMFLOAT3 fartopleft, XMFLOAT3 nearbottomleft, XMFLOAT3 neartopright, XMFLOAT3 nearbottomright, XMFLOAT3 fartopright, 
	//XMFLOAT3 farbottomleft, XMFLOAT3 farbottomright, XMFLOAT3 neartopleft
	XMFLOAT3 farTopLeft0;
	farTopLeft0.x = fartopleft.x;
	farTopLeft0.y = fartopleft.y;
	farTopLeft0.z = fartopleft.z;
	XMFLOAT3 nearbottomleft0;
	nearbottomleft0.x = nearbottomleft.x;
	nearbottomleft0.y = nearbottomleft.y;
	nearbottomleft0.z = nearbottomleft.z;
	XMFLOAT3 neartopRight0;
	neartopRight0.x = neartopright.x;
	neartopRight0.y = neartopright.y;
	neartopRight0.z = neartopright.z;
	XMFLOAT3 nearbottomRight0;
	nearbottomRight0.x = nearbottomright.x;
	nearbottomRight0.y = nearbottomright.y;
	nearbottomRight0.z = nearbottomright.z;
	XMFLOAT3 farTopRight0;
	farTopRight0.x = fartopright.x;
	farTopRight0.y = fartopright.y;
	farTopRight0.z = fartopright.z;
	XMFLOAT3 farBottomLeft0;
	farBottomLeft0.x = farbottomleft.x;
	farBottomLeft0.y = farbottomleft.y;
	farBottomLeft0.z = farbottomleft.z;
	XMFLOAT3 farbottomright0;
	farbottomright0.x = farbottomright.x;
	farbottomright0.y = farbottomright.y;
	farbottomright0.z = farbottomright.z;
	XMFLOAT3 nearTopRIghtLeft0;
	nearTopRIghtLeft0.x = neartopright.x;
	nearTopRIghtLeft0.y = neartopright.y;
	nearTopRIghtLeft0.z = neartopright.z;

	XMFLOAT3 Center;
	XMFLOAT3 Point2;
	Center.x = center.x;
	Center.y = center.y;
	Center.z = center.z;
	Point2.x = point2.x;
	Point2.y = point2.y;
	Point2.z = point2.z;





	
	//render->add_debug_line(center, point2, color);

	//render->add_debug_line(neartopleft, fartopleft, black);
	//render->add_debug_line(neartopleft, nearbottomleft, black);
	//render->add_debug_line(neartopleft, neartopright, black);

	//render->add_debug_line(neartopright, nearbottomright, black);
	//render->add_debug_line(neartopright, fartopright, black);

	//render->add_debug_line(nearbottomleft, farbottomleft, black);
	//render->add_debug_line(nearbottomleft, nearbottomright, black);

	//render->add_debug_line(nearbottomright, farbottomright, black);

	//// far starting
	//render->add_debug_line(farbottomleft, fartopleft, black);
	//render->add_debug_line(farbottomleft, farbottomright, black);
	//render->add_debug_line(farbottomright, fartopright, black);
	//render->add_debug_line(fartopright, fartopleft, black);
	return CreateFrustumLines(ptWorld, farTopLeft0, nearbottomleft0, neartopRight0, nearbottomRight0, farTopRight0, farBottomLeft0, farbottomright0, nearTopRIghtLeft0, parentWorldMatrixIndex,xoffset,yoffset,zoffset,Center,Point2);

}

void CAISystem::UpdateFrustum(frustum_t & frustum, float4x4 camera_transform, float fov, float aspect_ratio, float near_offset, float far_offset)
{
	float4 nearcenteTEMPTr = camera_transform.row4 + camera_transform.row3*near_offset;
	float4 farcenterTEMPT = camera_transform.row4 + camera_transform.row3*far_offset;
	float3 nearcenter;
	nearcenter.x = nearcenteTEMPTr.x;
	nearcenter.y = nearcenteTEMPTr.y;
	nearcenter.z = nearcenteTEMPTr.z;
	float3 farcenter;
	farcenter.x = farcenterTEMPT.x;
	farcenter.y = farcenterTEMPT.y;
	farcenter.z = farcenterTEMPT.z;
	float Hnear = 2 * tan(fov / 2.0f)*near_offset;
	float Hfar = 2 * tan(fov / 2.0f)*far_offset;
	float Wnear = Hnear * aspect_ratio;
	float Wfar = Hfar * aspect_ratio;
	float3 fartopleft;
	float3 fartopright;
	float3 farbottomleft;
	float3 farbottomright;
	float3 neartopleft;
	float3 neartopright;
	float3 nearbottomleft;
	float3 nearbottomright;
	float3 black;
	black.x = 0;
	black.y = 0;
	black.z = 0;
	float3 camY;
	camY.x = camera_transform.row2.x;
	camY.y = camera_transform.row2.y;
	camY.z = camera_transform.row2.z;
	float3 camX;
	camX.x = camera_transform.row1.x;
	camX.y = camera_transform.row1.y;
	camX.z = camera_transform.row1.z;

	fartopleft = farcenter + camY * (Hfar*0.5f) - camX * (Wfar*0.5);
	fartopright = farcenter + camY * (Hfar*0.5f) + camX * (Wfar*0.5);
	farbottomleft = farcenter - camY * (Hfar*0.5f) - camX * (Wfar*0.5);
	farbottomright = farcenter - camY * (Hfar*0.5f) + camX * (Wfar*0.5);


	neartopleft = nearcenter + camY * (Hnear*0.5f) - camX * (Wnear*0.5);
	neartopright = nearcenter + camY * (Hnear*0.5f) + camX * (Wnear*0.5);
	nearbottomleft = nearcenter - camY * (Hnear*0.5f) - camX * (Wnear*0.5);
	nearbottomright = nearcenter - camY * (Hnear*0.5f) + camX * (Wnear*0.5);
	frustum[0] = calculate_plane(farbottomright, farbottomleft, fartopleft);
	float3 center = farbottomright + farbottomleft + fartopright + fartopleft;
	center.x /= 4.0f;
	center.y /= 4.0f;
	center.z /= 4.0f;
	float3 point2;

	point2 = center + (frustum[0].normal * 5);
	float3 color;
	color.x = 1;
	//render->add_debug_line(center, point2, color);



	frustum[1] = calculate_plane(nearbottomright, farbottomright, fartopright);
	center = farbottomright + fartopright + nearbottomright + neartopright;
	center.x /= 4.0f;
	center.y /= 4.0f;
	center.z /= 4.0f;
	point2;
	point2 = center + (frustum[1].normal * 5);

	//render->add_debug_line(center, point2, color);
	frustum[2] = calculate_plane(nearbottomleft, nearbottomright, neartopright);
	center = nearbottomright + nearbottomleft + neartopleft + neartopright;
	center.x /= 4.0f;
	center.y /= 4.0f;
	center.z /= 4.0f;
	point2;
	point2 = center + (frustum[2].normal * 2);

	//	render->add_debug_line(center, point2, color);
	frustum[3] = calculate_plane(farbottomleft, nearbottomleft, neartopleft);
	//center = neartopleft + nearbottomleft + fartopleft + farbottomleft;
	//center.x /= 4.0f;
	//center.y /= 4.0f;
	//center.z /= 4.0f;
	//point2;
	//point2 = center + (frustum[3].normal * 2);

	//render->add_debug_line(center, point2, color);
	frustum[4] = calculate_plane(fartopright, fartopleft, neartopleft);
	//center = fartopleft + fartopright + neartopright + neartopleft;
	//center.x /= 4.0f;
	//center.y /= 4.0f;
	//center.z /= 4.0f;
	//point2;
	//point2 = center + (frustum[4].normal * 2);

	//	render->add_debug_line(center, point2, color);
	frustum[5] = calculate_plane(nearbottomright, nearbottomleft, farbottomleft);
}

void CAISystem::FollowObject(XMMATRIX thingToFollow, XMMATRIX * AIMatrix)
{
	XMMATRIX beforeitChanges = *AIMatrix;
	*AIMatrix = XMMatrixLookAtLH(AIMatrix->r[3],
		thingToFollow.r[3], XMVectorSet(0, 1, 0, 0));
	*AIMatrix = XMMatrixInverse(NULL, *AIMatrix);

	XMVECTOR direction = thingToFollow.r[3] - AIMatrix->r[3];
	direction.m128_f32[0] = 0;
	direction.m128_f32[1] = 0;
	direction.m128_f32[2] = 1;
	direction.m128_f32[3] = 0;
	direction = XMVector3Normalize(direction);
	direction *= 0.001f;//Frame Dependent
	XMMATRIX localMatrix2 = XMMatrixTranslationFromVector(direction);

	*AIMatrix = XMMatrixMultiply(localMatrix2, *AIMatrix);
}

void CAISystem::ShootGun(Clips* AIGun)
{
	AIGun->tryToShoot = true;
}

void CAISystem::SetNumberOfAI(int aiCount)
{
	numberofAI = aiCount;
}
//
//bool CAISystem::LookForPlayer(XMVECTOR start, XMVECTOR end, XMMATRIX boxWorldMaxtrix, TAABB abbcolider, float * distance, CCollisionSystem * pcCollisionPointer)
//{
//	if (abbcolider.m_IndexLocation == 1) {
//		float x = 0;
//	}
//	return pcCollisionPointer->IsLineInBox(start, end, boxWorldMaxtrix, abbcolider, distance);
//}

XMMATRIX CAISystem::LookBackLeftToRight(XMMATRIX AiMatrix, bool leftorRight)
{
	XMMATRIX d3dTmpViewM, d3dMovementM, d3dRotation;
	float fXchange = 0, fYchange = 0, fXEnd = 0, fYEnd = 0;
	if (leftorRight == true)
		d3dRotation = XMMatrixRotationY(0.0001);
	else {
		d3dRotation = XMMatrixRotationY(-0.0001);
	}
	d3dTmpViewM = AiMatrix;
	XMVECTOR d3d_newX, d3d_newY, d3d_existingZ;
	d3dTmpViewM = XMMatrixMultiply(d3dRotation, d3dTmpViewM);

	d3d_existingZ = d3dTmpViewM.r[2];
	d3d_newX = XMVector3Cross(AiMatrix.r[1], d3d_existingZ);
	d3d_newY = XMVector3Cross(d3d_existingZ, d3d_newX);

	d3d_newX = XMVector3Normalize(d3d_newX);
	d3d_newY = XMVector3Normalize(d3d_newY);

	d3d_existingZ = XMVector3Normalize(d3d_existingZ);

	d3dTmpViewM.r[0] = d3d_newX;
	d3dTmpViewM.r[1] = d3d_newY;
	d3dTmpViewM.r[2] = d3d_existingZ;
	//d3dRotation = XMMatrixRotationX(0);

//	d3dTmpViewM = XMMatrixMultiply(d3dRotation, d3dTmpViewM);

	//d3dTmpViewM = XMMatrixMultiply(d3dRotation, d3dTmpViewM);
	return d3dTmpViewM;
}

int CAISystem::GetNumberOfAI()
{
	return numberofAI;
<<<<<<< HEAD
}
=======
}

void CAISystem::FindBestPath(int start,int end,vector<XMVECTOR>* directions)
{
	if (open.size() == 0) {
		SearchNode* beiningingnode = Nodes[start];
		PlannerNode* firstplan = new PlannerNode();
		firstplan->state = beiningingnode;
		open.push(firstplan);
	}
	while (!open.empty()) {
		PlannerNode* current = open.front();
		open.pop();
		if (current->state == Nodes[end]) {
			MakeDirections(directions, current);
			return;
		}
		for (int i = 0; i < current->state->edges.size(); ++i) {
			SearchNode* successor = current->state->edges[i];
			float temptcost = calculateTest(current, successor);
			if (visited[successor] != nullptr) {
				PlannerNode* babynode = visited[successor];
				if (temptcost < babynode->givenCost) {
					open.remove(babynode);
					babynode->givenCost = temptcost;
					babynode->finalCost = GetFinalCost(babynode);
					babynode->parent = current;
					open.push(babynode);
				}
			}
			else
			{
				PlannerNode* babynode = new PlannerNode();
				babynode->state = successor;
				babynode->givenCost = temptcost;
				babynode->heuristicCost = CalcualteDistance(successor->tile, Nodes[end]->tile);
				babynode->finalCost = GetFinalCost(babynode);
				babynode->parent = current;
				visited[successor] = babynode;
				open.push(babynode);
			}
		}
	//	time -= 1;
	}
}

void CAISystem::AddNodeToPathFinding(int index, XMFLOAT3 pos, float weight)
{
	SearchNode* newNode = new SearchNode();
	newNode->tile = new tiledata();
	newNode->weight = weight;
	newNode->tile->pos = pos;
	Nodes[index] = newNode;
	
	//newNode->edges
}

void CAISystem::AddEdgestoNode(int nodeyouAreChanging, vector<int> edges)
{
	for (int i = 0; i < edges.size(); ++i) {
		Nodes[nodeyouAreChanging]->edges.push_back(Nodes[edges[i]]);
	}
}

void CAISystem::PathPlaningMovement(TAIPathFinding* path, XMMATRIX* worldMatrix)
{
	if (path->index <path->directions.size()) {
		XMVECTOR direction =path->directions[path->index] - worldMatrix->r[3];
		direction = XMVector3Normalize(direction);
		direction *= 0.001f;//Frame Dependent
		XMMATRIX localMatrix2 = XMMatrixTranslationFromVector(direction);

		*worldMatrix = XMMatrixMultiply(localMatrix2, *worldMatrix);
		if (sqrtf(
			((path->directions[path->index].m128_f32[0] - worldMatrix->r[3].m128_f32[0])*(path->directions[path->index].m128_f32[0] - worldMatrix->r[3].m128_f32[0])) +
			((path->directions[path->index].m128_f32[1] - worldMatrix->r[3].m128_f32[1])*(path->directions[path->index].m128_f32[1] - worldMatrix->r[3].m128_f32[1])) +
			((path->directions[path->index].m128_f32[2] - worldMatrix->r[3].m128_f32[2])*(path->directions[path->index].m128_f32[2] - worldMatrix->r[3].m128_f32[2]))
		) < 1) {
			path->index++;
		}
	}
}

float CAISystem::CalcualteDistance(tiledata * _search, tiledata * goal)
{
	return sqrtf(
		((_search->pos.x - goal->pos.x)*(_search->pos.x - goal->pos.x)) +
		((_search->pos.y - goal->pos.y)*(_search->pos.y - goal->pos.y)) +
		((_search->pos.z - goal->pos.z)*(_search->pos.z - goal->pos.z))
	);
}

void CAISystem::MakeDirections(vector<XMVECTOR>* directions, PlannerNode* current)
{
	if (current != nullptr) {
		XMVECTOR pos;
		pos.m128_f32[0] = current->state->tile->pos.x;
		pos.m128_f32[1] = current->state->tile->pos.y;
		pos.m128_f32[2] = current->state->tile->pos.z;

		directions->insert(directions->begin(), pos);
		MakeDirections(directions, current->parent);
	}
}
>>>>>>> master
