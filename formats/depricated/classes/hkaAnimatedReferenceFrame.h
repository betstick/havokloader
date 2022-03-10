#include "hkReferencedObject.h"

enum HkaReferenceFrameTypeEnum
{
	REFERENCE_FRAME_UNKNOWN = 0,
	REFERENCE_FRAME_DEFAULT = 1,
	REFERENCE_FRAME_PARAMETRIC = 2
};

typedef struct
{
	HkReferenceObject base;
	unsigned long unk08;
} HkaAnimatedReferenceFrame;