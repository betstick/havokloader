#pragma once
#include "hkReferencedObject.h"
#include "hkpLinkedCollidable.h"

enum MtChecks
{
	MULTI_THREADING_CHECKS_ENABLE = 0,
    MULTI_THREADING_CHECKS_IGNORE = 1
};

enum BroadPhaseType
{
	BROAD_PHASE_INVALID = 0,
	BROAD_PHASE_ENTITY = 1,
	BROAD_PHASE_PHANTOM = 2,
	BROAD_PHASE_BORDER = 3,
	BROAD_PHASE_MAX_ID = 4
};

typedef struct
{
	HkReferenceObject base;
	unsigned long unk00;
	unsigned long userData;
	HkpLinkedCollidable collidable;

} HkpWorldObject;