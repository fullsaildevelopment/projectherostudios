#pragma once
#ifndef CES_AI_COMPONENT_H
#define CES_AI_COMPONENT_H
typedef enum
{
//	COMPONENT_NONE = 0,
	COMPONENT_AIID = 1 << 1
} AI_Component;

typedef struct
{
	int AIID = 0;
} AI_ID;
#endif
