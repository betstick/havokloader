#include "hkaAnimatedReferenceFrame.h"
#include "hkaAnnotationTrack.h"

enum HkaAnimationType
{
	HK_UNKOWN_ANIMATION = 0,
	HK_INTERLEAVED_ANIMATION = 1,
	HK_MIRRORED_ANIMATION = 2,
	HK_SPLINE_COMPRESSED_ANIMATION = 3,
	HK_QUANTIZED_COMPRESSED_ANIMATION = 4,
	HK_PREDICTIVE_COMPRESSED_ANIMATION = 5,
	HK_REFERENCE_POSE_ANIMATION = 6
};

typedef struct
{
	unsigned int signature;
	HkaAnimationType type;
	float duration;
	int transformTracksCount;
	int floatTracksCount;
	HkaAnimatedReferenceFrame extractedMotion;
	HkaAnnotationTrack* annotionTracks;
} HkaAnimation;