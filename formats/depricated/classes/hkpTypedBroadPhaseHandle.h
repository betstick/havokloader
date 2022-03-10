#pragma once
#include "hkpBroadPhaseHandle.h"

typedef struct
{
	HkpBraoadPhaseHandle base;
	signed char type;
	signed char unk00;
	signed char objectQualityType;
	signed char unk01;
	unsigned int collisionFilterInfo;
};