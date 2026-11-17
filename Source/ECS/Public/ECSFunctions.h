#pragma once
#include "ECSTypes.h"

namespace ECS
{
	template <component... T>
	struct TExcludeSet {};
	
	template <component... TInclude, component... TExclude, typename Fn>
	void ForEach(FRegistry&, TExcludeSet<TExclude...>, Fn&& Func);

	template <component... TInclude, typename Fn>
	void ForEach(FRegistry&, Fn&& Func);
}

#include "ECSFunctions.inl"
