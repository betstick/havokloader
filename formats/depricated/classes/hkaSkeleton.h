typedef struct
{
	float m11,m12,m13,m14;
	float m21,m22,m23,m24;
	float m31,m32,m33,m34;
	float m41,m42,m43,m44;
} Mat4x4;

typedef struct
{
	unsigned int signature;
} HkLocalFrame;

typedef struct
{
	HkLocalFrame localFrame;
	short boneIndex;
	int unk00;
	short unk02;
} HkaSkeletonLocalFrameOnBone;

typedef struct
{
	long namePtr;
	bool lockTranslation;
	int unk00;
	int unk04;
	bool unk05;
} HkaBone;

struct HkaSkeleton
{
	long nameOffset;
	short* parentIndices;
	HkaBone* bones;
	Mat4x4* referencePos;
	float* referenceFloats;
	long floatSlotPtr;
};