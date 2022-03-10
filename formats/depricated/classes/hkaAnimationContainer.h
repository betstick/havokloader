#pragma once
#include "hkaSkeleton.h"
#include "hkaAnimation.h"
#include "hkaAnimationBindings.h"
#include "hkaBoneAttachment.h"
#include "hkaMeshBinding.h"

typedef struct
{
	unsigned int signature;
	HkaSkeleton* skeletons;
	HkaAnimation* animations;
	HkaAnimationBinding* bindings;
	HkaBoneAttachment* attachments;
	HkaMeshBinding* skins;
};