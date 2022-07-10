#pragma once

#include "Runtime\Core\Public\Stats\Stats.h"
#include "Runtime\Core\Public\Async\TaskGraphInterfaces.h"

template<class T>
struct ITask {
	ENamedThreads::Type DesiredThread;
	virtual void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent) = 0;

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(T, STATGROUP_TaskGraphTasks);
	}
};
