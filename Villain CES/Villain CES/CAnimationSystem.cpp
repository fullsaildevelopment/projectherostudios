#include "CAnimationSystem.h"

CAnimationSystem::CAnimationSystem()
{
}

CAnimationSystem::~CAnimationSystem()
{
}

//Returns XMFLOAT4X4 Array of Tweened Joint Positions
XMFLOAT4X4 * CAnimationSystem::PlayAnimation(TAnimationVariant& animationVariant, TAnimation& theAnimation, float& realTime)
{
	CalculateFrameCount(animationVariant, theAnimation, realTime);
	static XMFLOAT4X4 jointsForVS[63];//Clayton Joints
	std::vector<XMMATRIX> thisFramesTweenJointMatrix;

	if (animationVariant.tClaytonAnim.forward)
	{
		//clear thisFramesJointVector
		//std::cout << "Current Frame:\t" << currentFrame << "\n";
		//std::cout << "Next Frame:\t" << nextFrame << "\n";
		//thisFramesJointVec.clear();//Vector of positions/normals for joint animation
		//updateNormal.clear();

		float t = (realTime - theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames[animationVariant.tClaytonAnim.currentFrame].dTime) / (theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames[animationVariant.tClaytonAnim.nextFrame].dTime - theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames[animationVariant.tClaytonAnim.currentFrame].dTime);
		if (realTime <= theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames[animationVariant.tClaytonAnim.nextFrame].dTime)
		{
			if (realTime > theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames[animationVariant.tClaytonAnim.currentFrame].dTime)
				t = (realTime - theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames[animationVariant.tClaytonAnim.currentFrame].dTime) / (theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames[animationVariant.tClaytonAnim.nextFrame].dTime + theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].dDuration - theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames[animationVariant.tClaytonAnim.currentFrame].dTime);
			else
				t = (realTime + theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].dDuration - theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames[animationVariant.tClaytonAnim.currentFrame].dTime) / (theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames[animationVariant.tClaytonAnim.nextFrame].dTime + theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].dDuration - theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames[animationVariant.tClaytonAnim.currentFrame].dTime);
		}

		thisFramesTweenJointMatrix.clear();//Vector of XMMatrix's
		for (int i = 0; i < theAnimation.invBindPosesForJoints.size(); i++)
		{
			//clear thisFramesJointVector
			//updateNormal.clear();
			//thisFramesJointVec.clear();
			//thisFramesTweenJointMatrix.clear();
			//for (int j = 0; j < theAnimation.invBindPosesForJoints.size(); j++)
			//{
				//Cur Frame and next Frame Position Vectors
			XMVECTOR x, y;
			//Cur Frame and next Frame Joint Matrices in 3x3 format
			XMFLOAT4X4 format;
			XMStoreFloat4x4(&format, theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames[animationVariant.tClaytonAnim.currentFrame].m_vd3dJointMatrices[i]);
			XMMATRIX qOne = XMLoadFloat4x4(&format);
			XMStoreFloat4x4(&format, theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames[animationVariant.tClaytonAnim.nextFrame].m_vd3dJointMatrices[i]);
			XMMATRIX qTwo = XMLoadFloat4x4(&format);

			//Converted 3x3 matrices into quaternions
			XMVECTOR qOneI = XMQuaternionRotationMatrix(qOne);
			XMVECTOR qTwoI = XMQuaternionRotationMatrix(qTwo);
			x = theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames[animationVariant.tClaytonAnim.currentFrame].m_vd3dJointMatrices[i].r[3];
			y = theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames[animationVariant.tClaytonAnim.nextFrame].m_vd3dJointMatrices[i].r[3];
			//get the time for the next frame
			//float t = theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames[ animationVariant.tClaytonAnim.currentFrame].dTime - realTime;
			//Convert tween quaternion back to a matrix
			XMMATRIX tween = XMMatrixRotationQuaternion(XMQuaternionSlerp(qOneI, qTwoI, t));
			//Set tween position
			tween.r[3] = XMVectorLerp(x, y, t);
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
		//}
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
				XMStoreFloat3x3(&format, theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames[animationVariant.tClaytonAnim.currentFrame].m_vd3dJointMatrices[i]);
				XMMATRIX qOne = XMLoadFloat3x3(&format);
				XMStoreFloat3x3(&format, theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames[animationVariant.tClaytonAnim.nextFrame].m_vd3dJointMatrices[i]);
				XMMATRIX qTwo = XMLoadFloat3x3(&format);
				//Converted 3x3 matrices into quaternions
				XMVECTOR qOneI = XMQuaternionRotationMatrix(qOne);
				XMVECTOR qTwoI = XMQuaternionRotationMatrix(qTwo);
				x = theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames[animationVariant.tClaytonAnim.currentFrame].m_vd3dJointMatrices[i].r[3];
				y = theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames[animationVariant.tClaytonAnim.nextFrame].m_vd3dJointMatrices[i].r[3];
				//get the time for the next frame
				float t = theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames[animationVariant.tClaytonAnim.currentFrame].dTime;
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
		XMStoreFloat4x4(&jointsForVS[i], XMMatrixMultiply(theAnimation.invBindPosesForJoints[i], thisFramesTweenJointMatrix[i]));

		/*jointsForVS[i]._11 *= .01;
		jointsForVS[i]._12 *= .01;
		jointsForVS[i]._13 *= .01;
		jointsForVS[i]._14 *= .01;
		jointsForVS[i]._21 *= .01;
		jointsForVS[i]._22 *= .01;
		jointsForVS[i]._23 *= .01;
		jointsForVS[i]._24 *= .01;
		jointsForVS[i]._31 *= .01;
		jointsForVS[i]._32 *= .01;
		jointsForVS[i]._33 *= .01;
		jointsForVS[i]._34 *= .01;
		jointsForVS[i]._41 *= .01;
		jointsForVS[i]._42 *= .01;
		jointsForVS[i]._43 *= .01;
		jointsForVS[i]._44 *= .01;*/
	}

	return jointsForVS;
}

void CAnimationSystem::CalculateFrameCount(TAnimationVariant& animationVariant, TAnimation& theAnimation, float& realTime)
{
	int prevFrame = animationVariant.tClaytonAnim.currentFrame;

	if (animationVariant.tClaytonAnim.forward)
	{
		if (realTime > theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames[animationVariant.tClaytonAnim.currentFrame].dTime)
		{
			animationVariant.tClaytonAnim.currentFrame++;
			animationVariant.tClaytonAnim.nextFrame = animationVariant.tClaytonAnim.currentFrame + 1;
		}
	}
	else if (realTime < theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames[animationVariant.tClaytonAnim.currentFrame].dTime)
	{
		animationVariant.tClaytonAnim.currentFrame--;
		animationVariant.tClaytonAnim.nextFrame = animationVariant.tClaytonAnim.currentFrame - 1;
	}

	animationVariant.tClaytonAnim.currentFrame %= theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames.size();

	if (animationVariant.tClaytonAnim.forward)
	{
		if (realTime > theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames[animationVariant.tClaytonAnim.currentFrame].dTime)
		{
			animationVariant.tClaytonAnim.nextFrame = animationVariant.tClaytonAnim.currentFrame + 1;
			//animationVariant.tClaytonAnim.nextFrame %= theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames.size();
			if (prevFrame == theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames.size() - 1 && !animationVariant.tClaytonAnim.currentFrame)
			{
				realTime = 0;//Reset The Timer
			}
		}
	}
	else if (realTime < theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames[animationVariant.tClaytonAnim.currentFrame].dTime)
	{
		if (!animationVariant.tClaytonAnim.nextFrame)
		{
			animationVariant.tClaytonAnim.nextFrame = theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames.size() - 1;
			if (!prevFrame &&  animationVariant.tClaytonAnim.currentFrame == theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames.size() - 1)
			{
				realTime = theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].dDuration;//Reset The Timer
			}
		}
		else
		{
			animationVariant.tClaytonAnim.nextFrame = animationVariant.tClaytonAnim.currentFrame - 1;
		}
	}

	animationVariant.tClaytonAnim.nextFrame %= theAnimation.m_tAnim[animationVariant.tClaytonAnim.animType].m_vtKeyFrames.size();
}

XMMATRIX CAnimationSystem::NLerp(XMMATRIX m1, XMMATRIX m2, double timeRatio)
{
	XMMATRIX resultMatrix;
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t p = 0; p < 4; p++)
		{
			resultMatrix.r[i].m128_f32[p] = (m2.r[i].m128_f32[p] - m1.r[i].m128_f32[p]) * timeRatio + m1.r[i].m128_f32[p];
		}

	}

	return resultMatrix;
}
