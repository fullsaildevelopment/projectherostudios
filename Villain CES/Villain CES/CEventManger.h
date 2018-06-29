#pragma once
/***********************************************
* Filename:  		CEventManger.h
* Date:      		06/28/2018
* Mod. Date: 		06/28/2018
* Mod. Initials:	AP
* Author:    		Arthur Parker
* Purpose:   		To Handle Communcation Between Systems	
*			
************************************************/

static class CEventManger
{
	public:
		
		/*****************************************************************
		* ReleaseCamera() To set the Camera to Free Movement Mode
		*
		* Ins:
		*
		* Outs: 
		*
		* Returns:	return Value of value of sucuesss false == failure true == sucuess
		*
		* Mod. Date: 		06/28/2018
        * Mod. Initials:	AP
		*****************************************************************/
		static bool ReleaseCamera();
		/*****************************************************************
		* SwitchCharecter() To set the Camera to Free Movement Mode
		*
		* Ins: bool pass in true for switching to Charecter to the right pass in false to switch to the charecter to the left of the list Array

		*
		* Outs:
		*
		* Returns:	return Value of value of sucuesss false == failure true == sucuess
		*
		* Mod. Date: 		06/28/2018
		* Mod. Initials:	AP
		*****************************************************************/
		static int SwitchCharecter(bool nDirectionofSwitch);
		// when we have math in the game will take a vector
		/*****************************************************************
		* MoveCamera() Sends in a Vector3 to move Camera based on vector x ,y ,z
		*
		* Ins: a vector that is based on how much it needs to move in the x, y ,z 

		*
		* Outs:
		*
		* Returns:	return Value of value of sucuesss false == failure true == sucuess
		*
		* Mod. Date: 		06/28/2018
		* Mod. Initials:	AP
		*****************************************************************/
		static int MoveCamera();
		// when we have a match library Will pass in a 3x3 matrix to have the camera rotate to it
		/*****************************************************************
		* MoveCamera() Sends in a 3x3 Matrix to have the Camera rotate 
		*
		* Ins: a vector that is based on how much it needs to move in the x, y ,z

		*
		* Outs:
		*
		* Returns:	return Value of value of sucuesss false == failure true == sucuess
		*
		* Mod. Date: 		06/28/2018
		* Mod. Initials:	AP
		*****************************************************************/
		static int LerpCamera();


};

