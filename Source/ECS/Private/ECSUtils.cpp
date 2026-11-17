#include "ECSUtils.h"

uint32 ECS::NextTypeId()
{
	static uint32 Next = 0;
	return Next++;
}
