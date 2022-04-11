#pragma once
#include "cmem.h"
#include <stdio.h>

class hkbPhysicsInterface
{
	public:
	char data[24];

	hkbPhysicsInterface* hkbPhysicsInterfaceRead(MEM* src);
};