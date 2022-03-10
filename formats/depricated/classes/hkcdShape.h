#pragma once
#include "hkReferencedObject.h"

typedef struct
{
	HkReferenceObject base;
	char unk00;
	ShapeDispatchTypeEnum dispatchType;
	char bitsPerKey;
	ShapeInfoCodecTypeEnum shapeInfoCodecType;
	unsigned int unk04;
} HkcdShape;