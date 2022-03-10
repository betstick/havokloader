#pragma once
#include "hkaSkeleton.h"
#include "hkaMeshBindingMapping.h"

typedef struct
{
	unsigned int signature;
	unsigned long unk00;
	unsigned long originalSkeletonNameStrOffset;
	unsigned long nameStrOffset;
	HkaSkeleton skeleton;
	HkaMeshBindingMapping* mappings;
	Mat4x4* boneFromSkinMeshTransforms;
} HkaMeshBinding;