#pragma once
#ifndef CES_UI_COMPONENT_H
#define CES_UI_COMPONENT_H
typedef enum
{
//	COMPONENT_NONE  = 0,
	COMPONENT_BAR	= 1 << 1,
	COMPONENT_TEXT  = 1 << 2,
	COMPONENT_LABEL = 1 << 3
} UI_Component;

typedef struct
{
	 /*
	 TODO:
		 Fill out Bar Struct
	 */
} Bar;

typedef struct
{
	/*
	TODO:
		Fill out Text Struct
	*/
} Text;

typedef struct
{
	/*
	TODO:
		Fill out Label Struct
	*/
} Label;
#endif