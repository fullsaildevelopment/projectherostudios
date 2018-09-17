#include "CAnimationSystem.h"

CAnimationSystem::CAnimationSystem()
{
}

CAnimationSystem::~CAnimationSystem()
{
}

//Returns XMFLOAT4X4 Array of Tweened Joint Positions
XMFLOAT4X4 * CAnimationSystem::PlayAnimation(TAnimationVariant animationVariant, TAnimation theAnimation, float realTime)
{
	CalculateFrameCount(animationVariant, theAnimation, realTime);
	static XMFLOAT4X4 jointsForVS[59];//Clayton Joints
	std::vector<XMMATRIX> thisFramesTweenJointMatrix;

	if (animationVariant.tClaytonAnim.forward)
	{
		//clear thisFramesJointVector
		//std::cout << "Current Frame:\t" << currentFrame << "\n";
		//std::cout << "Next Frame:\t" << nextFrame << "\n";
		//thisFramesJointVec.clear();//Vector of positions/normals for joint animation
		//updateNormal.clear();

		thisFramesTweenJointMatrix.clear();//Vector of XMMatrix's
		for (int i = 0; i < theAnimation.invBindPosesForJoints.size(); i++)
		{
			//clear thisFramesJointVector
			//updateNormal.clear();
			//thisFramesJointVec.clear();
			thisFramesTweenJointMatrix.clear();
			for (int i = 0; i < theAnimation.invBindPosesForJoints.size(); i++)
			{
				//Cur Frame and next Frame Position Vectors
				XMVECTOR x, y;
				//Cur Frame and next Frame Joint Matrices in 3x3 format
				XMFLOAT3X3 format;
				XMStoreFloat3x3(&format, theAnimation.m_tAnim.m_vtKeyFrames[ animationVariant.tClaytonAnim.currentFrame].m_vd3dJointMatrices[i]);
				XMMATRIX qOne = XMLoadFloat3x3(&format);
				XMStoreFloat3x3(&format, theAnimation.m_tAnim.m_vtKeyFrames[ animationVariant.tClaytonAnim.nextFrame].m_vd3dJointMatrices[i]);
				XMMATRIX qTwo = XMLoadFloat3x3(&format);
				//Converted 3x3 matrices into quaternions
				XMVECTOR qOneI = XMQuaternionRotationMatrix(qOne);
				XMVECTOR qTwoI = XMQuaternionRotationMatrix(qTwo);
				x = theAnimation.m_tAnim.m_vtKeyFrames[ animationVariant.tClaytonAnim.currentFrame].m_vd3dJointMatrices[i].r[3];
				y = theAnimation.m_tAnim.m_vtKeyFrames[ animationVariant.tClaytonAnim.nextFrame].m_vd3dJointMatrices[i].r[3];
				//get the time for the next frame
				float t = theAnimation.m_tAnim.m_vtKeyFrames[ animationVariant.tClaytonAnim.currentFrame].dTime;
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
		for (int i = 0; i < theAnimation.invBindPosesForJoints.size(); i++)
		{
			//clear thisFramesJointVector
			//updateNormal.clear();
			//thisFramesJointVec.clear();

			thisFramesTweenJointMatrix.clear();
			for (int i = 0; i < theAnimation.invBindPosesForJoints.size(); i++)
			{
				//Cur Frame and next Frame Position Vectors
				XMVECTOR x, y;
				//Cur Frame and next Frame Joint Matrices in 3x3 format
				XMFLOAT3X3 format;
				XMStoreFloat3x3(&format, theAnimation.m_tAnim.m_vtKeyFrames[ animationVariant.tClaytonAnim.currentFrame].m_vd3dJointMatrices[i]);
				XMMATRIX qOne = XMLoadFloat3x3(&format);
				XMStoreFloat3x3(&format, theAnimation.m_tAnim.m_vtKeyFrames[ animationVariant.tClaytonAnim.nextFrame].m_vd3dJointMatrices[i]);
				XMMATRIX qTwo = XMLoadFloat3x3(&format);
				//Converted 3x3 matrices into quaternions
				XMVECTOR qOneI = XMQuaternionRotationMatrix(qOne);
				XMVECTOR qTwoI = XMQuaternionRotationMatrix(qTwo);
				x = theAnimation.m_tAnim.m_vtKeyFrames[ animationVariant.tClaytonAnim.currentFrame].m_vd3dJointMatrices[i].r[3];
				y = theAnimation.m_tAnim.m_vtKeyFrames[ animationVariant.tClaytonAnim.nextFrame].m_vd3dJointMatrices[i].r[3];
				//get the time for the next frame
				float t = theAnimation.m_tAnim.m_vtKeyFrames[ animationVariant.tClaytonAnim.currentFrame].dTime;
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
	for (int i = 0; i < theAnimation.invBindPosesForJoints.size(); i++)
	{
		XMStoreFloat4x4(&jointsForVS[i], XMMatrixTranspose(theAnimation.invBindPosesForJoints[i] * thisFramesTweenJointMatrix[i]));
	}

	return jointsForVS;
}

void CAnimationSystem::CalculateFrameCount(TAnimationVariant animationVariant, TAnimation theAnimation, float realTime)
{
	int prevFrame =  animationVariant.tClaytonAnim.currentFrame;

	if ( animationVariant.tClaytonAnim.forward)
	{
		if (realTime > theAnimation.m_tAnim.m_vtKeyFrames[ animationVariant.tClaytonAnim.currentFrame].dTime)
		{
			 animationVariant.tClaytonAnim.currentFrame++;	
			 animationVariant.tClaytonAnim.nextFrame =  animationVariant.tClaytonAnim.currentFrame + 1;
		}
	}
	else if (realTime < theAnimation.m_tAnim.m_vtKeyFrames[ animationVariant.tClaytonAnim.currentFrame].dTime)
	{
		 animationVariant.tClaytonAnim.currentFrame--;
		 animationVariant.tClaytonAnim.nextFrame =  animationVariant.tClaytonAnim.currentFrame - 1;
	}

	 animationVariant.tClaytonAnim.currentFrame %= theAnimation.m_tAnim.m_vtKeyFrames.size();

	if ( animationVariant.tClaytonAnim.forward)
	{
		if (realTime > theAnimation.m_tAnim.m_vtKeyFrames[ animationVariant.tClaytonAnim.currentFrame].dTime)
		{
			 animationVariant.tClaytonAnim.nextFrame =  animationVariant.tClaytonAnim.currentFrame + 1;
			 animationVariant.tClaytonAnim.nextFrame %= theAnimation.m_tAnim.m_vtKeyFrames.size();
			if (prevFrame == theAnimation.m_tAnim.m_vtKeyFrames.size() - 1 && ! animationVariant.tClaytonAnim.currentFrame)
			{
				realTime = 0;//Reset The Timer
			}
		}
	}
	else if (realTime < theAnimation.m_tAnim.m_vtKeyFrames[ animationVariant.tClaytonAnim.currentFrame].dTime)
	{
		if (! animationVariant.tClaytonAnim.nextFrame)
		{
			 animationVariant.tClaytonAnim.nextFrame = theAnimation.m_tAnim.m_vtKeyFrames.size() - 1;
			if (!prevFrame &&  animationVariant.tClaytonAnim.currentFrame == theAnimation.m_tAnim.m_vtKeyFrames.size() - 1)
			{
				realTime = theAnimation.m_tAnim.dDuration;//Reset The Timer
			}
		}
		else
		{
			 animationVariant.tClaytonAnim.nextFrame =  animationVariant.tClaytonAnim.currentFrame - 1;
		}
	}

}

