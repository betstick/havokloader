#pragma once
#include "hkaAnimation.h"

enum BlendHint
{
	NORMAL = 0,
	ADDITIVE_DEPRECATED = 1,
	ADDITIVE = 2
};

typedef struct
{
	unsigned int signature;
	HkaAnimation animation;
	short* transformTrackToBoneIndices;
	short* floatTrackToFloatSlotIndices;
	short* partitionIndices;
	BlendHint blendHint; //unsigned byte
	unsigned int unk00;
	unsigned short unk04;
	unsigned char unk06;
} HkaAnimationBinding;