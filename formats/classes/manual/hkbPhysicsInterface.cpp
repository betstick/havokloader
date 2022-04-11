#include "hkbPhysicsInterface.h"

hkbPhysicsInterface* hkbPhysicsInterface::hkbPhysicsInterfaceRead(MEM* src)
{
	hkbPhysicsInterface* x;
	mread(x->data,24,1,src);
	return x;
};