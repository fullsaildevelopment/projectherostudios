#include "CAnimationSystem.h"

CAnimationSystem::CAnimationSystem()
{
}


CAnimationSystem::~CAnimationSystem()
{
}

XMFLOAT4X4 * CAnimationSystem::PlayAnimation(TClaytonAnim claytonsAnimation, TAnimation theAnimation, std::vector<XMMATRIX> inverseBindPoseJointMatrices, float realTime)
{
	XMFLOAT4X4 jointsForVS[62];
	std::vector<XMMATRIX> thisFramesTweenJointMatrix;

	if (claytonsAnimation.forward)
	{
		//clear thisFramesJointVector
		//std::cout << "Current Frame:\t" << currentFrame << "\n";
		//std::cout << "Next Frame:\t" << nextFrame << "\n";
		//thisFramesJointVec.clear();//Vector of positions/normals for joint animation
		//updateNormal.clear();

		thisFramesTweenJointMatrix.clear();//Vector of XMMatrix's
		for (int i = 0; i < theAnimation.m_vnParentIndices.size(); i++)
		{
			//clear thisFramesJointVector
			//updateNormal.clear();
			//thisFramesJointVec.clear();
			thisFramesTweenJointMatrix.clear();
			for (int i = 0; i < theAnimation.m_vnParentIndices.size(); i++)
			{
				//Cur Frame and Nxt Frame Position Vectors
				XMVECTOR x, y;
				//Cur Frame and Nxt Frame Joint Matrices in 3x3 format
				XMFLOAT3X3 format;
				XMStoreFloat3x3(&format, theAnimation.m_tAnim.m_vtKeyFrames[claytonsAnimation.currentFrame].m_vd3dJointMatrices[i]);
				XMMATRIX qOne = XMLoadFloat3x3(&format);
				XMStoreFloat3x3(&format, theAnimation.m_tAnim.m_vtKeyFrames[claytonsAnimation.nextFrame].m_vd3dJointMatrices[i]);
				XMMATRIX qTwo = XMLoadFloat3x3(&format);
				//Converted 3x3 matrices into quaternions
				XMVECTOR qOneI = XMQuaternionRotationMatrix(qOne);
				XMVECTOR qTwoI = XMQuaternionRotationMatrix(qTwo);
				x = theAnimation.m_tAnim.m_vtKeyFrames[claytonsAnimation.currentFrame].m_vd3dJointMatrices[i].r[3];
				y = theAnimation.m_tAnim.m_vtKeyFrames[claytonsAnimation.nextFrame].m_vd3dJointMatrices[i].r[3];
				//get the time for the next frame
				float t = theAnimation.m_tAnim.m_vtKeyFrames[claytonsAnimation.currentFrame].dTime;
				//Convert tween quaternion back to a matrix
				XMMATRIX tween = XMMatrixRotationQuaternion(XMQuaternionSlerp(qOneI, qTwoI, t - realTime));
				//Set tween position
				tween.r[3] = XMVectorLerp(x, y, t - realTime);
				//Set simpleMesh position now that it's been tweened
				thisFramesTweenJointMatrix.push_back(tween);

				//memcpy(&temp.pos, &tween.r[3].m128_f32, sizeof(temp.pos));
				//Set simpleMesh normal now that it's been tweened
				//memcpy(&temp.norm, &XMQuaternionSlerp(qOneI, qTwoI, t - realTime).m128_f32, sizeof(temp.norm));
				//thisFramesJointVec.push_back(temp);
				//for (int j = 0; j < 3; j++)
				//{
				//	memcpy(&temp2.pos, &tween.r[j].m128_f32, sizeof(temp2.pos));
				//	updateNormal.push_back(temp2);
				//}
			}
		}
	}
	else
	{
		//clear thisFramesJointVector
		//std::cout << "Current Frame:\t" << currentFrame << "\n";
		//std::cout << "Next Frame:\t" << nextFrame << "\n";
		//thisFramesJointVec.clear();//Vector of positions/normals for joint animation
		//updateNormal.clear();

		thisFramesTweenJointMatrix.clear();//Vector of XMMatrix's
		for (int i = 0; i < theAnimation.m_vnParentIndices.size(); i++)
		{
			//clear thisFramesJointVector
			//updateNormal.clear();
			//thisFramesJointVec.clear();

			thisFramesTweenJointMatrix.clear();
			for (int i = 0; i < theAnimation.m_vnParentIndices.size(); i++)
			{
				//Cur Frame and Nxt Frame Position Vectors
				XMVECTOR x, y;
				//Cur Frame and Nxt Frame Joint Matrices in 3x3 format
				XMFLOAT3X3 format;
				XMStoreFloat3x3(&format, theAnimation.m_tAnim.m_vtKeyFrames[claytonsAnimation.currentFrame].m_vd3dJointMatrices[i]);
				XMMATRIX qOne = XMLoadFloat3x3(&format);
				XMStoreFloat3x3(&format, theAnimation.m_tAnim.m_vtKeyFrames[claytonsAnimation.nextFrame].m_vd3dJointMatrices[i]);
				XMMATRIX qTwo = XMLoadFloat3x3(&format);
				//Converted 3x3 matrices into quaternions
				XMVECTOR qOneI = XMQuaternionRotationMatrix(qOne);
				XMVECTOR qTwoI = XMQuaternionRotationMatrix(qTwo);
				x = theAnimation.m_tAnim.m_vtKeyFrames[claytonsAnimation.currentFrame].m_vd3dJointMatrices[i].r[3];
				y = theAnimation.m_tAnim.m_vtKeyFrames[claytonsAnimation.nextFrame].m_vd3dJointMatrices[i].r[3];
				//get the time for the next frame
				float t = theAnimation.m_tAnim.m_vtKeyFrames[claytonsAnimation.currentFrame].dTime;
				//Convert tween quaternion back to a matrix
				XMMATRIX tween = XMMatrixRotationQuaternion(XMQuaternionSlerp(qOneI, qTwoI, realTime - t));
				//Set tween position
				tween.r[3] = XMVectorLerp(x, y, realTime - t);
				//Set simpleMesh position now that it's been tweened
				thisFramesTweenJointMatrix.push_back(tween);

				//memcpy(&temp.pos, &tween.r[3].m128_f32, sizeof(temp.pos));
				//Set simpleMesh normal now that it's been tweened
				//memcpy(&temp.norm, &XMQuaternionSlerp(qOneI, qTwoI, t - realTime).m128_f32, sizeof(temp.norm));
				//thisFramesJointVec.push_back(temp);
				//for (int j = 0; j < 3; j++)
				//{
				//	memcpy(&temp2.pos, &tween.r[j].m128_f32, sizeof(temp2.pos));
				//	updateNormal.push_back(temp2);
				//}
			}
		}
	}

	//Transpose joints to send to shader
	for (int i = 0; i < theAnimation.m_vnParentIndices.size(); i++)
	{
		XMStoreFloat4x4(&jointsForVS[i], XMMatrixTranspose(inverseBindPoseJointMatrices[i] * thisFramesTweenJointMatrix[i]));
	}

	return jointsForVS;
}
