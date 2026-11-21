#include "System/ECSScheduler.h"

namespace ECS
{
	ISystem& FScheduler::Add(TUniquePtr<ISystem> System)
	{
		check(System.IsValid());
		return *Systems.Add_GetRef(MoveTemp(System));
	}
}
