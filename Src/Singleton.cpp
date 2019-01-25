
#include "Singleton.h"
#include <vector>

DEFINE_SINGLETON(SingletonStack);
static std::vector< SingletonStack::TFunc > funcs;

void SingletonStack::registerSingleton(SingletonStack::TFunc func)
{
	funcs.push_back(func);
}

void SingletonStack::onExit()
{
	for (int i = funcs.size() - 1; i >= 0; --i)
	{
		TFunc func = funcs[i];
		func();
	}

	funcs.clear();
}