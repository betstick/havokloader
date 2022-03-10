#pragma once
#include "hkaSkeleton.h"
#include "hkReferencedObject.h"

typedef struct
{
	unsigned int signature;
	unsigned long originalSkeletonNameStrOffset;
	unsigned long unk0C;
	Mat4x4 boneFromAttachment;
	HkReferenceObject attachment;
	unsigned long nameStrOffset;
	short boneIndex;
	unsigned long unk21;
	unsigned int unk22;
	unsigned short unk23;
} HkaBoneAttachment;