#pragma once
#include "stdafx.h"
#include "Animation_Component.h"
#include "Graphics_Component.h"
#include <chrono>
class CAnimationSystem
{
public:
	CAnimationSystem();
	~CAnimationSystem();

	/*
	Parameters:
		frame count
		start time
		joint data
		forward or back
	*/
	XMFLOAT4X4* PlayAnimation(TAnimationVariant& claytonsAnimation, TAnimation& theAnimation, float& realTime, bool scale);

private:
	void CalculateFrameCount(TAnimationVariant& claytonsAnimation, TAnimation& theAnimation, float& realTime);

	XMMATRIX NLerp(XMMATRIX m1, XMMATRIX m2, double timeRatio);
		/*
		int frameCount = myAnim.frames.size();
		double animLength = myAnim.duration / 1000;
		//Start Clock
		start = std::chrono::system_clock::now();
		//entity for loop
	*/
	/*
	if (thisWorld.mask[currentEntity] == (COMPONENT_MESH | COMPONENT_MODEL))
	{
		/*
		transform a vertex by the inverse of the bind-pose joint transform
		this Moves vertex from model-space into the joint’s object-space
		(AKA bone-space)
		i.e., gives the vertex relative to the joint
		We then can transform this new bone-space vertex by
		the ‘tween’ transform
		that will move it from bone-space into model-space

		To cut down on the number of per-vertex matrix multiplications
		and the amount of data needed GPU side,
		we can pre-combine these transforms
			Float4x4 joints_for_vs[NumJoints]
			for i = 0; i < NumJoints; ++i joints_for_vs[i] = multiply( invBind[i], tweenJoint[i] )
			upload_joints_to_gpu( joints_for_vs )
		*/
		/*

		//If we are looping
		if (looping || stepForward)
		{
			//clear thisFramesJointVector
			//std::cout << "Current Frame:\t" << currentFrame << "\n";
			//std::cout << "Next Frame:\t" << nextFrame << "\n";

			thisFramesJointVec.clear();//Vector of simple mesh's
			thisFramesTweenJointMatrix.clear();//Vector of XMMatrix's
			updateNormal.clear();
			for (int i = 0; i < thisWorld.model[currentEntity].anim.parent_Indicies.size(); i++)
			{
				//clear thisFramesJointVector
				updateNormal.clear();
				thisFramesJointVec.clear();
				thisFramesTweenJointMatrix.clear();
				for (int i = 0; i < thisWorld.model[currentEntity].anim.parent_Indicies.size(); i++)
				{
					//Cur Frame and Nxt Frame Position Vectors
					XMVECTOR x, y;
					//Cur Frame and Nxt Frame Joint Matrices in 3x3 format
					XMFLOAT3X3 format;
					XMStoreFloat3x3(&format, thisWorld.model[currentEntity].anim.frames[currentFrame].joints[i]);
					XMMATRIX qOne = XMLoadFloat3x3(&format);
					XMStoreFloat3x3(&format, thisWorld.model[currentEntity].anim.frames[nextFrame].joints[i]);
					XMMATRIX qTwo = XMLoadFloat3x3(&format);
					//Converted 3x3 matrices into quaternions
					XMVECTOR qOneI = XMQuaternionRotationMatrix(qOne);
					XMVECTOR qTwoI = XMQuaternionRotationMatrix(qTwo);
					x = thisWorld.model[currentEntity].anim.frames[currentFrame].joints[i].r[3];
					y = thisWorld.model[currentEntity].anim.frames[nextFrame].joints[i].r[3];
					//get the time for the next frame
					float t = thisWorld.model[currentEntity].anim.frames[currentFrame].time;
					//Convert tween quaternion back to a matrix
					XMMATRIX tween = XMMatrixRotationQuaternion(XMQuaternionSlerp(qOneI, qTwoI, t - realTime));
					//Set tween position
					tween.r[3] = XMVectorLerp(x, y, t - realTime);
					//Set simpleMesh position now that it's been tweened
					thisFramesTweenJointMatrix.push_back(tween);
					memcpy(&temp.pos, &tween.r[3].m128_f32, sizeof(temp.pos));
					//Set simpleMesh normal now that it's been tweened
					memcpy(&temp.norm, &XMQuaternionSlerp(qOneI, qTwoI, t - realTime).m128_f32, sizeof(temp.norm));
					thisFramesJointVec.push_back(temp);
					for (int j = 0; j < 3; j++)
					{
						memcpy(&temp2.pos, &tween.r[j].m128_f32, sizeof(temp2.pos));
						updateNormal.push_back(temp2);
					}
				}
			}
		}
		else if (stepBack)
		{
			//clear thisFramesJointVector
			updateNormal.clear();
			thisFramesJointVec.clear();
			thisFramesTweenJointMatrix.clear();
			for (int i = 0; i < thisWorld.model[currentEntity].anim.parent_Indicies.size(); i++)
			{
				//Cur Frame and Nxt Frame Position Vectors
				XMVECTOR x, y;
				//Cur Frame and Nxt Frame Joint Matrices in 3x3 format
				XMFLOAT3X3 format;
				XMStoreFloat3x3(&format, thisWorld.model[currentEntity].anim.frames[currentFrame].joints[i]);
				XMMATRIX qOne = XMLoadFloat3x3(&format);
				XMStoreFloat3x3(&format, thisWorld.model[currentEntity].anim.frames[nextFrame].joints[i]);
				XMMATRIX qTwo = XMLoadFloat3x3(&format);
				//Converted 3x3 matrices into quaternions
				XMVECTOR qOneI = XMQuaternionRotationMatrix(qOne);
				XMVECTOR qTwoI = XMQuaternionRotationMatrix(qTwo);
				x = thisWorld.model[currentEntity].anim.frames[currentFrame].joints[i].r[3];
				y = thisWorld.model[currentEntity].anim.frames[nextFrame].joints[i].r[3];
				//get the time for the next frame
				float t = thisWorld.model[currentEntity].anim.frames[currentFrame].time;
				//Convert tween quaternion back to a matrix
				XMMATRIX tween = XMMatrixRotationQuaternion(XMQuaternionSlerp(qOneI, qTwoI, realTime - t));
				//Set tween position
				tween.r[3] = XMVectorLerp(x, y, realTime - t);
				//Set simpleMesh position now that it's been tweened
				thisFramesTweenJointMatrix.push_back(tween);
				memcpy(&temp.pos, &tween.r[3].m128_f32, sizeof(temp.pos));
				//Set simpleMesh normal now that it's been tweened
				memcpy(&temp.norm, &XMQuaternionSlerp(qOneI, qTwoI, realTime - t).m128_f32, sizeof(temp.norm));
				thisFramesJointVec.push_back(temp);
				for (int j = 0; j < 3; j++)
				{
					memcpy(&temp2.pos, &tween.r[j].m128_f32, sizeof(temp2.pos));
					updateNormal.push_back(temp2);
				}
			}
		}

		worldMatrix = XMMatrixIdentity();

		for (int i = 0; i < thisWorld.model[currentEntity].inverseBindPoseJointMatrices.size(); i++)
		{
			XMStoreFloat4x4(&jointsForVS[i], XMMatrixTranspose(thisWorld.model[currentEntity].inverseBindPoseJointMatrices[i] * thisFramesTweenJointMatrix[i]));
		}
		//worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixScaling(1, 1, 1));
		sys.devcon->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// Set the vertex buffer to active in the input assembler so it can be rendered.
		sys.devcon->IASetVertexBuffers(0, 1, &thisWorld.mesh[currentEntity].vertexBuffer, &thisWorld.mesh[currentEntity].vertexBufferStride, &thisWorld.mesh[currentEntity].vertexBufferOffset);
		ID3D11ShaderResourceView * arrayText[3] = { thisWorld.mesh[currentEntity].shaderResourceView1, thisWorld.mesh[currentEntity].shaderResourceView2, thisWorld.mesh[currentEntity].shaderResourceView3 };
		sys.devcon->PSSetShaderResources(0, 3, arrayText);
		// Set the index buffer to active in the input assembler so it can be rendered.
		sys.InitShaderData(sys.devcon, worldMatrix, viewMatrix, projectionMatrix, false, thisWorld.mask[currentEntity], &thisWorld.model[currentEntity], lightPos, cameraPos, jointsForVS);
		sys.ExecutePipeline(&thisWorld, sys.devcon, thisWorld.mesh[currentEntity].vertexCount, currentEntity);
		worldMatrix = XMMatrixIdentity();
	}
		*/

		/*

	#pragma region Calculate FrameCount Time
		//End Clock
		end = std::chrono::system_clock::now();
		//Get the Delta Time
		deltaTime = sys.getFrameCount(start, end);
		//If it's looping, add deltaTime
		if (looping)
		{
			realTime += deltaTime;
		}
		//If stepping forward, add delta time and set stepForward to false
		if (stepForward)
		{
			realTime += deltaTime;
			stepForward = false;
		}
		//If stepping backward, add delta time and set stepBackto false
		if (stepBack)
		{
			realTime -= deltaTime;
			stepBack = false;
		}
		//If realTime is greater than the animation length, reset realTime
		if (realTime > animLength)
			realTime = 0;

	#pragma endregion

	#pragma region Changing Frame Count
		//If the end of the current frame's time is less than the real time
		//and we are playing forward the animation
		if (myAnim.frames[currentFrame].time <= realTime && playingForward && playing)
		{
			std::cout << "Real Time:\t" << realTime << "\n";
			std::cout << "Current Frame:\t" << currentFrame << "\n";
			std::cout << "Next Frame:\t" << nextFrame << "\n";
			currentFrame++;
			currentFrame %= frameCount;
			if (currentFrame == 0)
			{
				currentFrame = 1;
				realTime = myAnim.frames[currentFrame].time;
			}
			nextFrame = (currentFrame + 1) % frameCount;

			if (currentFrame == myAnim.frames.size()-1)
			{
				nextFrame = 1;
			}

			//nextFrame %= frameCount;
		}
		//Else If the end of the current frame's time is greater than the real time
		//and we are reversing the animation
		else if (myAnim.frames[currentFrame].time >= realTime && !playingForward && playing)
		{
			//clear thisFramesJointVector
			//if cf = 0
				// cf = end frame
				// nf = end frame - 1
			//else
				// cf--;
				// nf--;
			std::cout << "Real Time:\t" << realTime << "\n";
			std::cout << "Current Frame:\t" << currentFrame << "\n";
			std::cout << "Next Frame:\t" << nextFrame << "\n";
			if (currentFrame == 1)
			{
				currentFrame = frameCount - 1;
				nextFrame = frameCount - 1;
				realTime = animLength;
			}
			else
			{
				currentFrame--;
				nextFrame = currentFrame - 1;
			}
	}

	#pragma endregion
	*/
};
