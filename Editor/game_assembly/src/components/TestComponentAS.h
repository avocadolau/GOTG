#pragma once
#include <Wiwa/utilities/Reflection.h>

struct TestComponentAS {
	int data1;
	int x, y;
};

REFLECTION_BEGIN(TestComponentAS)
	REFLECT_MEMBER(data1)
	REFLECT_MEMBER(x)
	REFLECT_MEMBER(y)
REFLECTION_END;