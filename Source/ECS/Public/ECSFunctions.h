#pragma once

#include "ECS/Private/ECSUtils.h"

class FRegistry;

namespace ECS
{
	template <component... TExcluded>
	struct ECS_API TExclude {};
	
	template <component... TInclude, component... TExcluded, typename Fn>
	void ForEach(FRegistry& Registry, TExclude<TExcluded...>, Fn&& Func);

	template <component... TInclude, typename Fn>
	void ForEach(FRegistry& Registry, Fn&& Func);	
}

#include "ECS/Private/ECSView.inl"
