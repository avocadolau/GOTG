#pragma once
#include <Wiwa/utilities/Reflection.h>

struct Test2Comp {
	float data;
	int x, y;
};

REFLECTION_BEGIN(Test2Comp)
	REFLECT_MEMBER(data)
	REFLECT_MEMBER(x)
	REFLECT_MEMBER(y)
REFLECTION_END;